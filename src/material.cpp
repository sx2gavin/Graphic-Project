#include "material.hpp"

Material::~Material()
{
}

Material::Material(const Colour& kd, const Colour& ks, double shininess, double reflection_rate, double refraction_rate, double refraction_index)
  : m_kd(kd), 
	m_ks(ks), 
	m_shininess(shininess), 
	m_reflection_rate(reflection_rate),
	m_refraction_rate(refraction_rate),
	m_refraction_index(refraction_index)
{
}
