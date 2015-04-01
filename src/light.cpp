#include "light.hpp"
#include <iostream>

Light::Light()
  : colour(0.0, 0.0, 0.0),
    position(0.0, 0.0, 0.0)
{
  falloff[0] = 1.0;
  falloff[1] = 0.0;
  falloff[2] = 0.0;
}

std::ostream& operator<<(std::ostream& out, const Light& l)
{
  out << "L[" << l.colour << ", " << l.position << ", ";
  for (int i = 0; i < 3; i++) {
    if (i > 0) out << ", ";
    out << l.falloff[i];
  }
  out << "]";
  return out;
}

AreaLight::AreaLight() : colour(0.0, 0.0, 0.0), position(0.0, 0.0, 0.0), u(0.0, 0.0, 0.0), v(0.0, 0.0, 0.0), u_length(0.0), v_length(0.0)
{
	falloff[0] = 1.0;
	falloff[1] = 0.0;
	falloff[2] = 0.0;
}

int AreaLight::rayTracing(Point3D ray_org, Vector3D ray_dir, double& distance, Colour& color)
{
	float x1;
	float x2;
	float x3;
	float r1;
	float r2;
	float r3;
	float y1;
	float y2;
	float y3;
	float z1;
	float z2;
	float z3;
	float d;
	float d1;
	float d2;
	float d3;
	float beta;
	float gamma;
	float t;
	float den;

	Vector3D n = u.cross(v);
	n.normalize();
	den = n.dot(ray_dir);

	if (den == 0) {
		return 0;
	} else if (den > 0) {
		n = -n;
	}

	x1 = u[0];
	x2 = v[0];
	x3 = - ray_dir[0];
	r1 = ray_org[0] - position[0];

	y1 = u[1];
	y2 = v[1];
	y3 = - ray_dir[1];
	r2 = ray_org[1] - position[1];

	z1 = u[2];
	z2 = v[2];
	z3 = - ray_dir[2];
	r3 = ray_org[2] - position[2];

	d = det(x1, x2, x3, y1, y2, y3, z1, z2, z3);
	d1 = det(r1, x2, x3, r2, y2, y3, r3, z2, z3);
	d2 = det(x1, r1, x3, y1, r2, y3, z1, r3, z3);
	d3= det(x1, x2, r1, y1, y2, r2, z1, z2, r3);

	beta = d1 / d;
	gamma = d2 / d;
	t = d3 / d;

	if ( beta >= 0 && beta <= u_length && gamma >= 0 && gamma <= v_length && t > 0.0) {
		distance = t;
		color = colour; 
		return 1;
	} else {
		return 0;	
	}
}
