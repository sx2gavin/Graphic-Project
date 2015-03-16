#include "a4.hpp"
#include "image.hpp"
#include "pixel.hpp"

void render_background(int width, int height, Image *img) 
{
	// size of the grid
	int l = std::min(width, height) / 10;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			if ( ((x / l) % 2 && (y / l) % 2) || (!((x / l) % 2) && !((y / l) % 2))) {
				(*img)(x, y, 0) = 1.0;
				(*img)(x, y, 1) = 1.0;
				(*img)(x, y, 2) = 1.0;
			} else {
				(*img)(x, y, 0) = 0.0;
				(*img)(x, y, 1) = 0.0;
				(*img)(x, y, 2) = 0.0;
			}
		}
	}	
}
void a4_render(// What to render
               SceneNode* root,
               // Where to output the image
               const std::string& filename,
               // Image size
               int width, int height,
               // Viewing parameters
               const Point3D& eye, const Vector3D& view,
               const Vector3D& up, double fov,
               // Lighting parameters
               const Colour& ambient,
               const std::list<Light*>& lights
               )
{
  // Fill in raytracing code here.

  std::cerr << "Stub: a4_render(" << root << ",\n     "
            << filename << ", " << width << ", " << height << ",\n     "
            << eye << ", " << view << ", " << up << ", " << fov << ",\n     "
            << ambient << ",\n     {";

  for (std::list<Light*>::const_iterator I = lights.begin(); I != lights.end(); ++I) {
    if (I != lights.begin()) std::cerr << ", ";
    std::cerr << **I;
  }
  std::cerr << "});" << std::endl;

  Image img(width, height, 3);

  render_background(width, height, &img);

  Point3D p_world;

  for (int y = 0; y < height; y++) {
	 for (int x = 0; x < width; x++) {
		// for each pixel, find the p_world that is corresponding to the pixel (x, y).
		pixel p;
		Point3D p_screen(x, y, 0);

		// step 1. translate
		p_screen[0] = (double)width / 2 - (double)x ;
		p_screen[1] = (double)height / 2 - (double)y;
		p_screen[2] = (double)1 / (double)std::tan(fov * M_PI / 180.0);


		// step 2. Scale
		p_screen[0] = p_screen[0] / (double)height / 2;
		p_screen[1] = p_screen[1] / (double)height / 2;


		// step 3. Rotate
		double l;
		// w
		Vector3D w = view;
		w.normalize();

		// m
		Vector3D m = up.cross(w);
		m.normalize();

		// v
		Vector3D v = w.cross(m);
		v.normalize();
		
		Vector4D w_4d(w[0], w[1], w[2], 0);
		Vector4D m_4d(m[0], m[1], m[2], 0);
		Vector4D v_4d(v[0], v[1], v[2], 0);

		Matrix4x4 rotation(m_4d, v_4d, w_4d, Vector4D(0, 0, 0, 1));
		rotation = rotation.transpose();

		p_screen = rotation * p_screen;

		// step 4.translate
		p_world[0] = p_screen[0] + eye[0];
		p_world[1] = p_screen[1] + eye[1];
		p_world[2] = p_screen[2] + eye[2];

		// std::cerr << "p_world = " << p_world << std::endl;

		Colour pixel_color(0, 0, 0);

		// primary ray
		if (root->rayTracing(eye, p_world, p)) {
			// Adding Phong shading.
			Point3D hitPoint = eye + (p.z_buffer - 0.001) * (p_world - eye);
			pixel_color  = ambient * p.material->getDiffuseColor();

			for (std::list<Light*>::const_iterator I = lights.begin(); I != lights.end(); I++) { 
				pixel temp;
				// secondary ray, adding shadows.
				if (root->rayTracing(hitPoint, (*I)->position, temp)) {
					continue;
				}	
				Vector3D lightDirection = (*I)->position - hitPoint;
				double distance = lightDirection.length();
				double attenuation = 1 / ((*I)->falloff[0] + distance* (*I)->falloff[1] + distance * distance * (*I)->falloff[2]);
				Vector3D camera = eye - hitPoint;
				
				// normal correction	
				if (p.normal.dot(camera) < 0) {
					p.normal = -p.normal;
				}
				// diffuse
				p.normal.normalize();
				lightDirection.normalize();
				float cosTheta = clamp(p.normal.dot(lightDirection), 0, 1);

				Vector3D reflection = -lightDirection - 2 * ((-lightDirection).dot(p.normal)) * p.normal;
				reflection.normalize();
				camera.normalize();
				float cosAlpha = clamp(camera.dot(reflection), 0, 1);

				pixel_color =
					// ambient color
					pixel_color + 
					// diffuse color
					p.material->getDiffuseColor() * ( cosTheta * (*I)->colour ) * attenuation +
					// specular color
					p.material->getSpecularColor() * (std::pow(cosAlpha, p.material->getShininess()) * (*I)->colour) * attenuation;
			}

			img(x, y, 0) = pixel_color.R();
			img(x, y, 1) = pixel_color.G();
			img(x, y, 2) = pixel_color.B();
		}
	 }
  }
  img.savePng(filename);  
}
