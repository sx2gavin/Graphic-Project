#include "a4.hpp"
#include "image.hpp"
#include "pixel.hpp"
#include "primitive.hpp"
#include "material.hpp"
#include <thread>
#include <ctime>

#define RECURSION_LEVEL 3
#define NUM_THREADS 6

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

int rayTracing(std::list<Primitive*> &objects, Point3D ray_org, Vector3D ray_dir, const Colour& ambient, const std::list<Light*>& lights, int recursion_level, Colour &final_color)
{
	pixel temp;
	pixel p;
	int retVal = 0;
	Primitive* hitObject;
	for (std::list<Primitive*>::const_iterator i = objects.begin(); i != objects.end(); i++) {
		if ((*i)->rayTracing(ray_org, ray_dir, temp)) {
			retVal = 1;
			if (temp.z_buffer < p.z_buffer) {
				p = temp;
				hitObject = (*i);
			}
		}	
	}
	// if the ray didn't hit anything, return 0 and abort.
	
	if (!retVal) return retVal;

	// Adding Phong shading.
	double distance;
	double attenuation;
	float cosTheta;
	Vector3D reflection;
	float cosAlpha;

	Point3D hitPoint = ray_org + (p.z_buffer - 0.0001) * ray_dir;
	Vector3D camera = ray_org - hitPoint;
	camera.normalize();
	final_color  = ambient * p.material->getDiffuseColor();
	Vector3D lightDirection;

	// secondary ray, adding shadows and shade.
	for (std::list<Light*>::const_iterator I = lights.begin(); I != lights.end(); I++) { 

		lightDirection = (*I)->position - hitPoint;
		distance = lightDirection.length();
		lightDirection.normalize();

		if (rayTracingHit(objects, hitPoint, lightDirection)) {
			continue;
		}	
		attenuation = 1 / ((*I)->falloff[0] + distance* (*I)->falloff[1] + distance * distance * (*I)->falloff[2]);

		// normal correction	
		if (p.normal.dot(camera) < 0) {
			p.normal = -p.normal;
		}
		// diffuse
		p.normal.normalize();
		cosTheta = clamp(p.normal.dot(lightDirection), 0, 1);

		reflection = - lightDirection - 2 * ((-lightDirection).dot(p.normal)) * p.normal;
		reflection.normalize();
		cosAlpha = clamp(camera.dot(reflection), 0, 1);

		final_color =
			// ambient color
			final_color + 
			// diffuse color
			p.material->getDiffuseColor() * (cosTheta * (*I)->colour) * attenuation +
			// specular color
			p.material->getSpecularColor() * (std::pow(cosAlpha, p.material->getShininess()) * (*I)->colour) * attenuation;
	}

	// recursive ray, adding reflection or refraction
	if ( recursion_level > 0 ) {
		if ( p.material->getReflectionRate() > 0.0 ) {
			Colour reflected_color(0, 0, 0);
			reflection = (- camera) - 2 * ((-camera).dot(p.normal)) * p.normal; 
			reflection.normalize();
			if (rayTracing(objects, hitPoint, reflection, ambient, lights, recursion_level - 1,  reflected_color)) {
				final_color = final_color + reflected_color * p.material->getReflectionRate();	
			}
		}

		Point3D out;
		Vector3D out_normal;
		hitPoint = ray_org + p.z_buffer * ray_dir;	
		if (hitObject->refractiveRay(hitPoint, ray_dir, p.normal, out, out_normal)) {
			Colour refracted_color(0.0, 0.0, 0.0);
			if (rayTracing(objects, out + 0.01 * out_normal, out_normal, ambient, lights, 0, refracted_color)) {
				final_color = final_color + refracted_color * p.material->getRefractionRate();
			}
		}	
	}
	return retVal;
}

int rayTracingHit(std::list<Primitive*> &objects, Point3D ray_org, Vector3D ray_dir)
{
	pixel temp;
	int retVal = 0;
	for (std::list<Primitive*>::const_iterator i = objects.begin(); i != objects.end(); i++) {
		if ((*i)->rayTracing(ray_org, ray_dir, temp)) {
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
		std::cerr << "Rendering in progress: " << (y - from) * 100 / (to - from) << "% finished." << std::endl;
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
			Vector3D ray_direction = p_world - eye;
			ray_direction.normalize();
			if (rayTracing(objects, eye, ray_direction, ambient, lights, RECURSION_LEVEL, pixel_color)) {
				(*img)(x, y, 0) = pixel_color.R();
				(*img)(x, y, 1) = pixel_color.G();
				(*img)(x, y, 2) = pixel_color.B();
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
	int from_height = 0;
	int to_height = 0; 
	std::thread *threads;
	time_t start, end;
	time(&start);
	
	if (NUM_THREADS != 1) {
		threads = new std::thread[NUM_THREADS - 1];
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

	for ( int i = 0; i < NUM_THREADS; i++) {
		from_height = i * height / NUM_THREADS;
		to_height = (i + 1) * height / NUM_THREADS - 1;
		if ( i == NUM_THREADS - 1 ) {
			multiProcessing( from_height, to_height, height, width, &img, objects, eye, view, up, fov, ambient, lights);
		} else {
			threads[i] = std::thread( multiProcessing, from_height, to_height, height, width, &img, objects, eye, view, up, fov, ambient, lights);
		}
	}

	for (int i = 0; i < NUM_THREADS - 1; i++) {
		threads[i].join();
	}

	img.savePng(filename);  
	time(&end);
	std::cerr << difftime(end, start) << " seconds" << std::endl;
	
	// Garbage collection
	for (std::list<Primitive*>::const_iterator i = objects.begin(); i != objects.end(); i++) {
		delete (*i);
	} 
	if (NUM_THREADS != 1) {
		delete [] threads;
	}
}
