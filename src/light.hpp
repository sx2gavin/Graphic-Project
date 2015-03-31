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
	Vector3D v;
	double falloff[3];
};
#endif
