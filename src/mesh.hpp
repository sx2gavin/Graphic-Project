#ifndef CS488_MESH_HPP
#define CS488_MESH_HPP

#include <list>
#include <vector>
#include <iosfwd>
#include "primitive.hpp"
#include "algebra.hpp"
#include "pixel.hpp"
#include "image.hpp"

// A polygonal mesh.
class Mesh : public Primitive {
	public:
		Mesh(const std::vector<Point3D>& verts,
				const std::vector< std::vector<int> >& faces);

		typedef std::vector<int> Face;

		virtual int rayTracing(Point3D ray_org, Vector3D ray_dir, pixel& p);
		virtual void transform(const Matrix4x4 t);
		virtual Primitive* clone(); 
		virtual void addTexture(const std::string& filename, std::vector<Point3D> verts);

	private:
		std::vector<Point3D> m_trans_verts;
		std::vector<Point3D> m_verts;
		std::vector<Face> m_faces;
		// texture
		std::string m_texture_file;
		Image m_texture;
		std::vector<Point3D> m_map;

		friend std::ostream& operator<<(std::ostream& out, const Mesh& mesh);
};

#endif
