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

	protected:
		Material()
		{
		}
};

class PhongMaterial : public Material {
	public:
		PhongMaterial(const Colour& kd, const Colour& ks, double shininess);
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

	private:
		Colour m_kd;
		Colour m_ks;

		double m_shininess;
};


#endif
