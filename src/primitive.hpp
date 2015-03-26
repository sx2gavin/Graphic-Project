#ifndef CS488_PRIMITIVE_HPP
#define CS488_PRIMITIVE_HPP

#include <list>
#include <vector>
#include "algebra.hpp"
#include "material.hpp"
#include "image.hpp"
#include "light.hpp"
#include "pixel.hpp"

class Primitive {
	public:
		virtual ~Primitive();
		void setMaterial(Material* m)
		{
			m_material = m;
		}
		virtual void transform(const Matrix4x4 t){}
		virtual Primitive* clone();

		virtual int rayTracing(Point3D ray_org, Vector3D ray_dir, pixel& p){return 0;}
		virtual int refractiveRay(Point3D in, Vector3D in_normal, Vector3D n, Point3D& out, Vector3D& out_normal){return 0;} 

	protected:
		Material*  m_material;	
};

class Sphere : public Primitive {
	public:
		Sphere();
		virtual ~Sphere();
		virtual int rayTracing(Point3D ray_org, Vector3D ray_dir,  pixel& p);
		virtual void transform(const Matrix4x4 t);
		virtual Primitive* clone();
		// virtual int refractiveRay(Point3D in, Vector3D in_normal, Vector3D n, Point3D& out, Vector3D out_normal);  

	private:
		Point3D getMiddlePoint(Point3D a, Point3D b);
		void addTriangle(std::vector<Point3D>* sphere_vertices, std::vector<Point3D> vertices, int index_1, int index_2, int index_3);
		void addTriangle(std::vector<Point3D>* sphere_vertices, Point3D point_1, Point3D point_2, Point3D point_3);

		std::vector<Point3D> m_trans_verts;
		std::vector<Point3D> m_verts;
		std::vector< std::vector<int> > m_faces;
};

class Cube : public Primitive {
	public:
		Cube();
		virtual ~Cube();
		virtual int rayTracing(Point3D ray_org, Vector3D ray_dir,  pixel& p);
		virtual void transform(const Matrix4x4 t);
		virtual Primitive* clone();
	private:
		std::vector<Point3D> m_trans_verts;
		std::vector<Point3D> m_verts;
		std::vector< std::vector<int> > m_faces;
};

class NonhierSphere : public Primitive {
	public:
		NonhierSphere(const Point3D& pos, double radius)
			: m_pos(pos), m_radius(radius)
		{
		}
		virtual ~NonhierSphere();
		virtual Primitive* clone();
		virtual int rayTracing(Point3D ray_org, Vector3D ray_dir,  pixel& p);
		virtual int refractiveRay(Point3D in, Vector3D in_normal, Vector3D n, Point3D& out, Vector3D& out_normal);

		Point3D getPosition()
		{
			return m_pos;
		}

		double getRadius()
		{
			return m_radius;
		}

	private:
		Point3D m_pos;
		double m_radius;
};

class NonhierBox : public Primitive {
	public:
		NonhierBox(const Point3D& pos, double size);

		virtual ~NonhierBox();
		virtual Primitive* clone();
		virtual int rayTracing(Point3D ray_org, Vector3D ray_dir,  pixel& p);

	private:
		Point3D m_pos;
		double m_size;

		std::vector<Point3D> m_verts;
		std::vector< std::vector<int> > m_faces;
};

class Cone : public Primitive {
	public:
		Cone(const Point3D& d, const Point3D& pos,  double height, double radius);		
		virtual ~Cone();
		virtual Primitive* clone();
		virtual int rayTracing(Point3D ray_org, Vector3D ray_dir, pixel& p);

	private:
		Vector3D m_d;
		Point3D m_pos;
		double m_height;
		double m_radius;
};

class Cylinder : public Primitive {
	public:
		Cylinder(const Point3D& d, const Point3D& pos,  double height, double radius);
		virtual ~Cylinder();
		virtual Primitive* clone();
		virtual int rayTracing(Point3D ray_org, Vector3D ray_dir, pixel& p);

	private:
		Vector3D m_d;
		Point3D m_pos;
		double m_height;
		double m_radius;
};
#endif
