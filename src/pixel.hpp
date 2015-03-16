#ifndef __PIXEL_H__
#define __PIXEL_H__
#include "material.hpp"
#include "algebra.hpp"

struct Pixel
{
	Pixel():z_buffer(0), material(NULL), normal(Vector3D(0.0, 0.0, 0.0)){}
	
	Pixel(double z_buffer, Material* material, Vector3D normal): z_buffer(z_buffer), material(material), normal(normal){}

	double z_buffer;
	Material *material;
	Vector3D normal;

	Pixel& operator =(const Pixel& p)
	{
		z_buffer = p.z_buffer;
		material = p.material;		
		normal = p.normal;
		return *this;
	}
};

typedef struct Pixel pixel;

#endif
