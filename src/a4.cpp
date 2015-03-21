#include "a4.hpp"
#include "image.hpp"
#include "pixel.hpp"
#include "primitive.hpp"
#include <thread>
#include <ctime>

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

int rayTracing(std::list<Primitive*> &objects, Point3D eye, Point3D p_world, pixel& p)
{
	pixel temp;
	int retVal = 0;
	for (std::list<Primitive*>::const_iterator i = objects.begin(); i != objects.end(); i++) {
		if ((*i)->rayTracing(eye, p_world, temp)) {
			retVal = 1;
			if ( p.z_buffer == 0 || p.z_buffer > temp.z_buffer) {
				p = temp;
			}
		}	
	}
	return retVal;
}

int rayTracingHit(std::list<Primitive*> &objects, Point3D eye, Point3D p_world, pixel& p)
{
	pixel temp;
	int retVal = 0;
	for (std::list<Primitive*>::const_iterator i = objects.begin(); i != objects.end(); i++) {
		if ((*i)->rayTracing(eye, p_world, temp)) {
			retVal = 1;
			break;
		}	
	}
	return retVal;
}

void multiProcessing(int from, 
			         int to, 
					 int height, 
					 int width, 
					 Image *img, 
					 std::list<Primitive*> &objects,
					 const Point3D& eye, 
					 const Vector3D& view, 
					 const Vector3D& up, 
					 double fov, 
					 const Colour& ambient, 
					 const std::list<Light*>& lights) 
{

	Point3D p_world;
	for (int y = from; y <= to; y++) {
		// std::cerr << "y = " << y << std::endl;
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

			Colour pixel_color(0, 0, 0);

			// primary ray
			if (rayTracing(objects, eye, p_world, p)) {
				// Adding Phong shading.
				Point3D hitPoint = eye + (p.z_buffer - 0.001) * (p_world - eye);
				pixel_color  = ambient * p.material->getDiffuseColor();

				for (std::list<Light*>::const_iterator I = lights.begin(); I != lights.end(); I++) { 
					pixel temp;
					// secondary ray, adding shadows.
					if (rayTracingHit(objects, hitPoint, (*I)->position, temp)) {
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

				(*img)(x, y, 0) = pixel_color.R();
				(*img)(x, y, 1) = pixel_color.G();
				(*img)(x, y, 2) = pixel_color.B();
				// std::cerr << pixel_color.R() << "," << pixel_color.G() << "," << pixel_color.B() << std::endl;
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
	// defines threads.
	int num_threads = 6;
	int from_height = 0;
	int to_height = 0; 
	std::thread *threads;
	time_t start, end;
	time(&start);
	
	if (num_threads != 1) {
		threads = new std::thread[num_threads - 1];
	}

	std::cerr << "Stub: a4_render(" << root << ",\n     "
		<< filename << ", " << width << ", " << height << ",\n     "
		<< eye << ", " << view << ", " << up << ", " << fov << ",\n     "
		<< ambient << ",\n     {";

	for (std::list<Light*>::const_iterator I = lights.begin(); I != lights.end(); ++I) {
		if (I != lights.begin()) std::cerr << ", ";
		std::cerr << **I;
	}
	std::cerr << "});" << std::endl;

	std::list<Primitive*> objects;
	root->collectPrimitives(objects);

	Image img(width, height, 3);
	render_background(width, height, &img);

	for ( int i = 0; i < num_threads; i++) {
		from_height = i * height / num_threads;
		to_height = (i + 1) * height / num_threads - 1;
		if ( i == num_threads - 1 ) {
			multiProcessing( from_height, to_height, height, width, &img, objects, eye, view, up, fov, ambient, lights);
		} else {
			threads[i] = std::thread( multiProcessing, from_height, to_height, height, width, &img, objects, eye, view, up, fov, ambient, lights);
		}
	}
	// std::thread first( multiProcessing, 0, height/2 - 1, height, width, &img, objects, eye, view, up, fov, ambient, lights);
	// std::thread second( multiProcessing, height/2, height-1, height, width, &img, objects, eye, view, up, fov, ambient, lights);

	for (int i = 0; i < num_threads - 1; i++) {
		threads[i].join();
	}

	img.savePng(filename);  
	time(&end);
	std::cerr << difftime(end, start) << " seconds" << std::endl;
	
	// Garbage collection
	for (std::list<Primitive*>::const_iterator i = objects.begin(); i != objects.end(); i++) {
		delete (*i);
	} 
	if (num_threads != 1) {
		delete [] threads;
	}
}
