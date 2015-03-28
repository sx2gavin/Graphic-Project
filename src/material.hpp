#ifndef CS488_MATERIAL_HPP
#define CS488_MATERIAL_HPP

#include "algebra.hpp"

class Material {
	public:
		Material(const Colour& kd, const Colour& ks, double shininess, double reflection_rate, double refraction_rate, double refraction_index);
		~Material();

		Colour getDiffuseColor() const
		{
			return m_kd;
		}

		Colour& getDiffuseColor()
		{
			return m_kd;
		}

		Colour getSpecularColor() const
		{
			return m_ks;
		}

		Colour& getSpecularColor()
		{
			return m_ks;
		}

		double getShininess() const
		{
			return m_shininess;
		}

		double& getShininess()
		{
			return m_shininess;
		}

		double getReflectionRate() const
		{
			return m_reflection_rate;
		}

		double& getReflectionRate()
		{
			return m_reflection_rate;
		}

		double getRefractionRate() const
		{
			return m_refraction_rate;
		}
		
		double& getRefractionRate()
		{
			return m_refraction_rate;
		}

		double getRefractionIndex() const
		{
			return m_refraction_index;
		}

		double& getRefractionIndex()
		{
			return m_refraction_index;
		}

	private:
		Colour m_kd;
		Colour m_ks;
		double m_shininess;
		double m_reflection_rate;
		double m_refraction_rate;
		double m_refraction_index;
};

#endif
