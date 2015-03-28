#ifndef __PIXEL_H__
#define __PIXEL_H__
#include "material.hpp"
#include "algebra.hpp"
#include <cfloat>

struct Pixel
{
	Pixel():z_buffer(DBL_MAX), material(NULL), textureColor(Colour(0.0, 0.0, 0.0)), normal(Vector3D(0.0, 0.0, 0.0)){}
	
	Pixel(const double z_buffer, Material* material, Colour color,  Vector3D normal): z_buffer(z_buffer), material(material), textureColor(color), normal(normal){}

	double z_buffer;
	Material *material;
	Colour textureColor;
	Vector3D normal;

	Pixel& operator =(const Pixel& p)
	{
		z_buffer = p.z_buffer;
		material = p.material;		
		textureColor = p.textureColor;
		normal = p.normal;
		return *this;
	}
};

typedef struct Pixel pixel;

#endif
