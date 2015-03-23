#include "primitive.hpp"
#include "polyroots.hpp"
#include <cmath>

Primitive::~Primitive()
{
}

Primitive* Primitive::clone()
{
	Primitive* new_primitive = new Primitive;
	new_primitive->setMaterial(m_material);
	return new_primitive;
}

Sphere::Sphere() 
{
	std::vector<Point3D> vertices;
	vertices.resize(12);

	float t = (1.0 + std::sqrt(5.0)) / 2.0;

	vertices[0] = Point3D(-1.0, t, 0.0);
	vertices[1] = Point3D(1.0, t, 0.0);
	vertices[2] = Point3D(-1.0, -t, 0.0);
	vertices[3] = Point3D(1.0, -t, 0.0);

	vertices[4] = Point3D(0.0, -1.0, t);
	vertices[5] = Point3D(0.0, 1.0, t);
	vertices[6] = Point3D(0.0, -1.0, -t);
	vertices[7] = Point3D(0.0, 1.0, -t);

	vertices[8] = Point3D(t, 0.0, -1.0);
	vertices[9] = Point3D(t, 0.0, 1.0);
	vertices[10] = Point3D(-t, 0.0, -1.0);
	vertices[11] = Point3D(-t, 0.0, 1.0);	

 	addTriangle(&m_verts, vertices, 0, 11, 5);
 	addTriangle(&m_verts, vertices, 0, 5, 1);
 	addTriangle(&m_verts, vertices, 0, 1, 7);
 	addTriangle(&m_verts, vertices, 0, 7, 10);
 	addTriangle(&m_verts, vertices, 0, 10, 11);

 	addTriangle(&m_verts, vertices, 1, 5, 9);
 	addTriangle(&m_verts, vertices, 5, 11, 4);
 	addTriangle(&m_verts, vertices, 11, 10, 2);
 	addTriangle(&m_verts, vertices, 10, 7, 6);
 	addTriangle(&m_verts, vertices, 7, 1, 8);

 	addTriangle(&m_verts, vertices, 3, 9, 4);
 	addTriangle(&m_verts, vertices, 3, 4, 2);
 	addTriangle(&m_verts, vertices, 3, 2, 6);
 	addTriangle(&m_verts, vertices, 3, 6, 8);
 	addTriangle(&m_verts, vertices, 3, 8, 9);

 	addTriangle(&m_verts, vertices, 4, 9, 5);
 	addTriangle(&m_verts, vertices, 2, 4, 11);
 	addTriangle(&m_verts, vertices, 6, 2, 10);
 	addTriangle(&m_verts, vertices, 8, 6, 7);
 	addTriangle(&m_verts, vertices, 9, 8, 1);
	
	// refine triangles
	for (int i = 0; i < 2; i++) {
		std::vector<Point3D> new_sphere_vertices;
		for (unsigned int j = 0; j < m_verts.size(); j+=3) {
			Point3D a(m_verts[j]);
			Point3D b(m_verts[j+1]);	
			Point3D c(m_verts[j+2]);
				
			Point3D m_ab = getMiddlePoint(a, b);
			Point3D m_bc = getMiddlePoint(c, b);
			Point3D m_ac = getMiddlePoint(a, c);

			addTriangle(&new_sphere_vertices, a, m_ab, m_ac);
			addTriangle(&new_sphere_vertices, b, m_bc, m_ab);
			addTriangle(&new_sphere_vertices, c, m_ac, m_bc);
			addTriangle(&new_sphere_vertices, m_bc, m_ac, m_ab);
		}
		m_verts.clear();
		m_verts = new_sphere_vertices;
	}
}

void Sphere::transform(const Matrix4x4 t)
{
	m_trans_verts.clear();
	for (std::vector<Point3D>::const_iterator I = m_verts.begin(); I != m_verts.end(); ++I) {
		m_trans_verts.push_back(t * (*I));
	}
}

void Sphere::addTriangle (std::vector<Point3D>* sphere_vertices, std::vector<Point3D> vertices, int index_1, int index_2, int index_3)
{
	double length;
	Point3D point;
	for (int i = 0; i < 3; i++) {
		if (i == 0) {
			point = vertices[index_1];
		} else if (i == 1) {
			point = vertices[index_2];
		} else if (i == 2) {
			point = vertices[index_3];
		}


		length = std::sqrt(point[0] * point[0] + point[1] * point[1] + point[2] * point[2]);	

		point[0] = point[0] / length;
		point[1] = point[1] / length;
		point[2] = point[2] / length;

		sphere_vertices->push_back(point);
	}

}

Primitive* Sphere::clone()
{
	Sphere* new_sphere = new Sphere;
	return new_sphere;
}

void Sphere::addTriangle(std::vector<Point3D>* sphere_vertices, Point3D point_1, Point3D point_2, Point3D point_3)
{
	double length;
	Point3D point;
	for (int i = 0; i < 3; i++) {
		if (i == 0) {
			point = point_1;
		} else if (i == 1) {
			point = point_2;
		} else if (i == 2) {
			point = point_3;
		}

		length = std::sqrt(point[0] * point[0] + point[1] * point[1] + point[2] * point[2]);	

		point[0] = point[0] / length;
		point[1] = point[1] / length;
		point[2] = point[2] / length;

		sphere_vertices->push_back(point);
	}
}


Point3D Sphere::getMiddlePoint(Point3D a, Point3D b)
{
	Point3D ret;
	ret[0] = (a[0] + b[0]) / 2;
	ret[1] = (a[1] + b[1]) / 2;
	ret[2] = (a[2] + b[2]) / 2;
	return ret;
}

Sphere::~Sphere()
{
}

int Sphere::rayTracing (Point3D eye, Point3D p_world,  pixel& p)
{
	int retVal = 0; 
	Point3D p0;
	Point3D p1;
	Point3D p2;
	Vector3D n;
	float den;
	float x1;
	float x2;
	float x3;
	float r1;
	float y1;
	float y2;
	float y3;
	float r2;
	float z1;
	float z2;
	float z3;
	float r3;
	float d ;
	float d1;
	float d2;
	float d3;
	float beta;
	float gamma;
	float t;

		
	for (std::vector<Point3D>::const_iterator I = m_trans_verts.begin(); I != m_trans_verts.end(); I+=3) {

		p0 = *I;
		p1 = *(I + 1);
		p2 = *(I + 2);

		n = (p1 - p0).cross(p2 - p0);
		den = n.dot(p_world - eye);

		// if the ray doesn't hit the plane represented by the triangle.
		if (den == 0) continue;

		x1 = p1[0] - p0[0];
		x2 = p2[0] - p0[0];
		x3 = - p_world[0] + eye[0];
		r1 = eye[0] - p0[0];

		y1 = p1[1] - p0[1];
		y2 = p2[1] - p0[1];
		y3 = - p_world[1] + eye[1];
		r2 = eye[1] - p0[1];

		z1 = p1[2] - p0[2];
		z2 = p2[2] - p0[2];
		z3 = - p_world[2] + eye[2];
		r3 = eye[2] - p0[2];

		d = det(x1, x2, x3, y1, y2, y3, z1, z2, z3);
		d1 = det(r1, x2, x3, r2, y2, y3, r3, z2, z3);
		d2 = det(x1, r1, x3, y1, r2, y3, z1, r3, z3);
		d3= det(x1, x2, r1, y1, y2, r2, z1, z2, r3);

		beta = d1 / d;
		gamma = d2 / d;
		t = d3 / d;

		if ( beta >= 0 && gamma >= 0 && (beta + gamma) <= 1 && t > 0.0) {
			// the ray hits the triangle. 
			retVal = 1;
			if ( p.z_buffer == 0 || p.z_buffer > t) {
				p.z_buffer = t;
				p.material = m_material;
				p.normal = n;

			}
		}
	}
	return retVal;
}

Cube::Cube()
{	
	for (double x = 0.0; x <= 1.0; x += 1.0) {
		for (double y = 0.0; y <= 1.0; y += 1.0) {
			for (double z = 0.0; z <= 1.0; z += 1.0) {
				m_verts.push_back(Point3D(x, y, z));
			}
		}
	}

	m_faces.resize(6);
	for (int i = 0; i < 6; i++) {
		m_faces[i].resize(4);
	}
	
	m_faces[0][0] = 0;
	m_faces[0][1] = 1;
	m_faces[0][2] = 3;
	m_faces[0][3] = 2;

	m_faces[1][0] = 0;
	m_faces[1][1] = 2;
	m_faces[1][2] = 6;
	m_faces[1][3] = 4;
		
	m_faces[2][0] = 0;
	m_faces[2][1] = 1;
	m_faces[2][2] = 5;
	m_faces[2][3] = 4;

	m_faces[3][0] = 7;
	m_faces[3][1] = 3;
	m_faces[3][2] = 2;
	m_faces[3][3] = 6;

	m_faces[4][0] = 7;
	m_faces[4][1] = 3;
	m_faces[4][2] = 1;
	m_faces[4][3] = 5;

	m_faces[5][0] = 7;
	m_faces[5][1] = 6;
	m_faces[5][2] = 4;
	m_faces[5][3] = 5;
}

Cube::~Cube()
{
}

int Cube::rayTracing(Point3D eye, Point3D p_world,  pixel& p)
{
	int retVal = 0; 
	Point3D p0;
	Point3D p1;
	Point3D p2;
	Vector3D n;
	float den;
	float x1;
	float x2;
	float x3;
	float r1;
	float y1;
	float y2;
	float y3;
	float r2;
	float z1;
	float z2;
	float z3;
	float r3;
	float d ;
	float d1;
	float d2;
	float d3;
	float beta;
	float gamma;
	float t;

		
	for (std::vector< std::vector<int> >::const_iterator I = m_faces.begin(); I != m_faces.end(); ++I) {
		for (std::vector<int>::const_iterator J = I->begin(); J != I->end() - 2; ++J) {
			p0 = m_trans_verts[(*I)[0]];
			p1 = m_trans_verts[*(J + 1)];
			p2 = m_trans_verts[*(J + 2)];

			n = (p1 - p0).cross(p2 - p0);
			den = n.dot(p_world - eye);

			// if the ray doesn't hit the plane represented by the triangle.
			if (den == 0) break;

			x1 = p1[0] - p0[0];
			x2 = p2[0] - p0[0];
			x3 = - p_world[0] + eye[0];
			r1 = eye[0] - p0[0];

			y1 = p1[1] - p0[1];
			y2 = p2[1] - p0[1];
			y3 = - p_world[1] + eye[1];
			r2 = eye[1] - p0[1];

			z1 = p1[2] - p0[2];
			z2 = p2[2] - p0[2];
			z3 = - p_world[2] + eye[2];
			r3 = eye[2] - p0[2];

			d = det(x1, x2, x3, y1, y2, y3, z1, z2, z3);
			d1 = det(r1, x2, x3, r2, y2, y3, r3, z2, z3);
			d2 = det(x1, r1, x3, y1, r2, y3, z1, r3, z3);
			d3= det(x1, x2, r1, y1, y2, r2, z1, z2, r3);

			beta = d1 / d;
			gamma = d2 / d;
			t = d3 / d;

			if ( beta >= 0 && gamma >= 0 && (beta + gamma) <= 1 && t > 0.0) {
				// the ray hits the triangle. 
				retVal = 1;
				if ( p.z_buffer == 0 || p.z_buffer > t) {
					p.z_buffer = t;
					p.material = m_material;
					p.normal = n;
					
				}
				break;
			}
		}
	}
	return retVal;
}

void Cube::transform(const Matrix4x4 t)
{
	m_trans_verts.clear();
	for (std::vector<Point3D>::const_iterator I = m_verts.begin(); I != m_verts.end(); ++I) {
		m_trans_verts.push_back(t * (*I));
	}
}

Primitive* Cube::clone()
{
	Cube* new_cube = new Cube;
	return new_cube;
}

NonhierSphere::~NonhierSphere()
{
}

Primitive* NonhierSphere::clone()
{
	NonhierSphere* new_nhsphere = new NonhierSphere(m_pos, m_radius);
	return new_nhsphere;
}


int NonhierSphere::rayTracing(Point3D eye, Point3D p_world, pixel& p) 
{
	int retVal = 0;
	double a = (p_world - eye).dot(p_world - eye);
	double b = 2 * (p_world - eye).dot(eye - m_pos);
	double c = (eye - m_pos).dot(eye - m_pos) - m_radius * m_radius;
	double roots[2]; 
	
	if (quadraticRoots(a, b, c, roots) == 1 && roots[0] > 0) {	
		p.z_buffer = roots[0];
		retVal = 1;
	} else if (quadraticRoots(a, b, c, roots) > 1 && std::min(roots[0], roots[1]) > 0) {
		p.z_buffer = std::min(roots[0], roots[1]);
		retVal = 1;
	}

	if (retVal) {
		p.material = m_material;
		p.normal = eye + p.z_buffer * (p_world - eye) - m_pos;
	}

	return retVal;
}

NonhierBox::NonhierBox(const Point3D& pos, double size)
: m_pos(pos), m_size(size)
{
	for (double x = m_pos[0]; x <= m_pos[0] + size; x += size) {
		for (double y = m_pos[1]; y <= m_pos[1] + size; y += size) {
			for (double z = m_pos[2]; z <= m_pos[2] + size; z += size) {
				m_verts.push_back(Point3D(x, y, z));
			}
		}
	}

	m_faces.resize(6);
	for (int i = 0; i < 6; i++) {
		m_faces[i].resize(4);
	}
	
	m_faces[0][0] = 0;
	m_faces[0][1] = 1;
	m_faces[0][2] = 3;
	m_faces[0][3] = 2;

	m_faces[1][0] = 0;
	m_faces[1][1] = 2;
	m_faces[1][2] = 6;
	m_faces[1][3] = 4;
		
	m_faces[2][0] = 0;
	m_faces[2][1] = 1;
	m_faces[2][2] = 5;
	m_faces[2][3] = 4;

	m_faces[3][0] = 7;
	m_faces[3][1] = 3;
	m_faces[3][2] = 2;
	m_faces[3][3] = 6;

	m_faces[4][0] = 7;
	m_faces[4][1] = 3;
	m_faces[4][2] = 1;
	m_faces[4][3] = 5;

	m_faces[5][0] = 7;
	m_faces[5][1] = 6;
	m_faces[5][2] = 4;
	m_faces[5][3] = 5;
}

NonhierBox::~NonhierBox()
{
}

Primitive* NonhierBox::clone()
{
	NonhierBox* new_nhbox = new NonhierBox(m_pos, m_size);
	return new_nhbox;
}

int NonhierBox::rayTracing(Point3D eye, Point3D p_world,  pixel& p)
{
	
	int retVal = 0; 
	Point3D p0;
	Point3D p1;
	Point3D p2;
	Vector3D n;
	float den;
	float x1;
	float x2;
	float x3;
	float r1;
	float y1;
	float y2;
	float y3;
	float r2;
	float z1;
	float z2;
	float z3;
	float r3;
	float d ;
	float d1;
	float d2;
	float d3;
	float beta;
	float gamma;
	float t;
		
	for (std::vector< std::vector<int> >::const_iterator I = m_faces.begin(); I != m_faces.end(); ++I) {
		for (std::vector<int>::const_iterator J = I->begin(); J != I->end() - 2; ++J) {
			p0 = m_verts[(*I)[0]];
			p1 = m_verts[*(J + 1)];
			p2 = m_verts[*(J + 2)];

			n = (p1 - p0).cross(p2 - p0);
			den = n.dot(p_world - eye);

			// if the ray doesn't hit the plane represented by the triangle.
			if (den == 0) break;

			x1 = p1[0] - p0[0];
			x2 = p2[0] - p0[0];
			x3 = - p_world[0] + eye[0];
			r1 = eye[0] - p0[0];

			y1 = p1[1] - p0[1];
			y2 = p2[1] - p0[1];
			y3 = - p_world[1] + eye[1];
			r2 = eye[1] - p0[1];

			z1 = p1[2] - p0[2];
			z2 = p2[2] - p0[2];
			z3 = - p_world[2] + eye[2];
			r3 = eye[2] - p0[2];

			d = det(x1, x2, x3, y1, y2, y3, z1, z2, z3);
			d1 = det(r1, x2, x3, r2, y2, y3, r3, z2, z3);
			d2 = det(x1, r1, x3, y1, r2, y3, z1, r3, z3);
			d3= det(x1, x2, r1, y1, y2, r2, z1, z2, r3);

			beta = d1 / d;
			gamma = d2 / d;
			t = d3 / d;

			if ( beta >= 0 && gamma >= 0 && (beta + gamma) <= 1 && t > 0.0) {
				// the ray hits the triangle. 
				retVal = 1;
				if ( p.z_buffer == 0 || p.z_buffer > t) {
					p.z_buffer = t;
					p.material = m_material;
					p.normal = n;
					
				}
				break;
			}
		}
	}
	return retVal;
}

Cone::Cone(const Point3D& d, const Point3D& pos, double height, double radius):m_pos(pos), m_height(height), m_radius(radius) 
{
	m_d[0] = d[0];
	m_d[1] = d[1];
	m_d[2] = d[2];	
	m_d.normalize();
}

Cone::~Cone()
{
}

Primitive* Cone::clone()
{
	Point3D temp(m_d[0], m_d[1], m_d[2]);
	Cone *new_cone = new Cone(temp, m_pos, m_height, m_radius);
	return new_cone;
}

int Cone::rayTracing(Point3D eye, Point3D p_world, pixel& p)
{
	int retVal = 0;
	Point3D q;
	double angle = atan(m_radius / m_height);
	Vector3D v = p_world - eye;
	Vector3D delta_p = eye - m_pos;
	double v_dot_m_d = v.dot(m_d);
	double p_dot_m_d = delta_p.dot(m_d);
	
	double a = pow(cos(angle), 2) * (v - v_dot_m_d * m_d).length2() - pow(sin(angle) * v_dot_m_d, 2);
	double b = 2 * pow(cos(angle), 2) * (v - v_dot_m_d * m_d).dot(delta_p - p_dot_m_d * m_d) - 2 * pow(sin(angle), 2) * v_dot_m_d * p_dot_m_d;
	double c = pow(cos(angle), 2) * (delta_p - p_dot_m_d * m_d).length2() - pow(sin(angle) * p_dot_m_d, 2) ; 
	double roots[2]; 
	
	// intersection between the ray and the cone
	if (quadraticRoots(a, b, c, roots) == 1 && roots[0] > 0) {	
		q = eye + roots[0] * (p_world - eye);
		if ((q - m_pos).dot(m_d) >= 0 && (q - m_pos).dot(m_d) <= m_height) {
			p.z_buffer = roots[0];
			retVal = 1;
		}
	} else if (quadraticRoots(a, b, c, roots) > 1 && std::min(roots[0], roots[1]) > 0) {
		q = eye + std::min(roots[0], roots[1]) * (p_world - eye);
		if ((q - m_pos).dot(m_d) >= 0 && (q - m_pos).dot(m_d) <= m_height) {
			p.z_buffer = std::min(roots[0], roots[1]);
			retVal = 1;
		} else {
			q = eye + std::max(roots[0], roots[1]) * (p_world - eye);
			if ((q - m_pos).dot(m_d) >= 0 && (q - m_pos).dot(m_d) <= m_height) {
				p.z_buffer = std::max(roots[0], roots[1]);
				retVal = 1;
			}
		}
	}
	if (retVal) {
		p.material = m_material;
		p.normal = q - (m_pos + (q - m_pos).length() / cos(angle) * m_d);
	}

	// intersection between the ray and the cap
	Point3D center = m_pos + m_height * m_d;
	double den = m_d.dot(v);
	double num;
	if (den != 0) {
		num = -m_d.dot(eye - center);
		double t = num / den;
		Point3D intersection = eye + t * (p_world - eye);
		if ((intersection - center).length() <= m_radius && t < p.z_buffer) {
			p.z_buffer = t;
			p.normal = m_d;		
			retVal = 1;
		}
	}
	return retVal;
}
