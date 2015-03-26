#ifndef CS488_A4_HPP
#define CS488_A4_HPP

#include <string>
#include "algebra.hpp"
#include "scene.hpp"
#include "light.hpp"

void render_background(int width, int height, Image *img);

int rayTracing(std::list<Primitive*> &objects, 
			   Point3D ray_org, 
			   Vector3D ray_dir, 
			   const Colour& ambient, 
			   const std::list<Light*>& lights, 
			   int recursion_level,
			   Colour &final_color);

int rayTracingHit(std::list<Primitive*> &objects, 
				  Point3D ray_org, 
				  Vector3D ray_dir 
				  );

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
					 const std::list<Light*>& lights); 

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
               );

#endif
