#ifndef CS488_LIGHT_HPP
#define CS488_LIGHT_HPP

#include "algebra.hpp"
#include <iosfwd>

// Represents a simple point light.
struct Light {
  Light();
  
  Colour colour;
  Point3D position;
  double falloff[3];
};

std::ostream& operator<<(std::ostream& out, const Light& l);

// Represents an area light.
struct AreaLight {
	AreaLight();

	Colour colour;
	Point3D position;
	Vector3D u;
	double u_length;
	Vector3D v;
	double v_length;
	double falloff[3];
	
	int rayTracing(Point3D ray_org, Vector3D ray_dir, double& distance, Colour& color);
};
#endif
