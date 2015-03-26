#ifndef CS488_MESH_HPP
#define CS488_MESH_HPP

#include <list>
#include <vector>
#include <iosfwd>
#include "primitive.hpp"
#include "algebra.hpp"
#include "pixel.hpp"

// A polygonal mesh.
class Mesh : public Primitive {
public:
  Mesh(const std::vector<Point3D>& verts,
       const std::vector< std::vector<int> >& faces);

  typedef std::vector<int> Face;

  virtual int rayTracing(Point3D ray_org, Vector3D ray_dir, pixel& p);
  virtual void transform(const Matrix4x4 t);
  virtual Primitive* clone(); 
  
private:
  std::vector<Point3D> m_trans_verts;
  std::vector<Point3D> m_verts;
  std::vector<Face> m_faces;

  friend std::ostream& operator<<(std::ostream& out, const Mesh& mesh);
};

#endif
