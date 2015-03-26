#ifndef CS488_MATERIAL_HPP
#define CS488_MATERIAL_HPP

#include "algebra.hpp"

class Material {
	public:
		virtual ~Material();
		virtual void apply_gl() const = 0;
		virtual Colour getDiffuseColor() const = 0;
		virtual Colour getSpecularColor() const = 0;
		virtual double getShininess() const = 0;
		virtual double getReflectionRate() const = 0;
		virtual double getRefractionRate() const = 0;
		virtual double getRefractionIndex() const = 0;

	protected:
		Material()
		{
		}
};

class PhongMaterial : public Material {
	public:
		PhongMaterial(const Colour& kd, const Colour& ks, double shininess, double reflection_rate, double refraction_rate, double refraction_index);
		virtual ~PhongMaterial();

		virtual void apply_gl() const;

		Colour getDiffuseColor() const
		{
			return m_kd;
		}
		Colour getSpecularColor() const
		{
			return m_ks;
		}
		double getShininess() const
		{
			return m_shininess;
		}

		double getReflectionRate() const
		{
			return m_reflection_rate;
		}

		double getRefractionRate() const
		{
			return m_refraction_rate;
		}

		double getRefractionIndex() const
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
