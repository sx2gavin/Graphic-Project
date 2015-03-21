#include <iostream>
#include "scene.hpp"
#include <cstring>

SceneNode::SceneNode(const std::string& name)
: m_name(name)
{
}

SceneNode::~SceneNode()
{
}

void SceneNode::set_parent_transform(const Matrix4x4& m)
{
	m_parent_trans = m;
}

void SceneNode::rotate(char axis, double angle)
{
	std::cerr << "Stub: Rotate " << m_name << " around " << axis << " by " << angle << std::endl;
	
	double data[16];
	data[3] = 0;
	data[7] = 0;
	data[11] = 0;
	data[12] = 0;
	data[13] = 0;
	data[14] = 0;
	data[15] = 1;

	if (axis == 'x' || axis == 'X') { // rotate on x-axis
		data[0] = 1; // m11
		data[1] = 0; // m12
		data[2] = 0; // m13
		data[4] = 0; // m21
		data[5] = cos(angle * M_PI / 180.0); // m22
		data[6] = -sin(angle * M_PI / 180.0); // m23
		data[8] = 0; // m31
		data[9] = sin(angle * M_PI / 180.0); // m32
		data[10] = cos(angle * M_PI / 180.0); // m33
		Matrix4x4 temp_matrix(data);
		m_trans = m_trans * temp_matrix;
	} else if (axis == 'y' || axis == 'Y') {// rotate on y-axis
		data[0] = cos(angle * M_PI / 180.0); // m11
		data[1] = 0; // m12 
		data[2] = sin(angle * M_PI / 180.0); // m13
		data[4] = 0; // m21 
		data[5] = 1; // m22 
		data[6] = 0; // m23
		data[8] = -sin(angle * M_PI / 180.0); // m31
		data[9] = 0; // m32
		data[10] = cos(angle * M_PI / 180.0); // m33
		Matrix4x4 temp_matrix(data);
		m_trans = m_trans * temp_matrix;
	} else if (axis == 'z' || axis == 'Z') {// rotate on z-axis
		data[0] = cos(angle * M_PI / 180.0); // m11 
        data[1] = -sin(angle * M_PI / 180.0); // m12 
        data[2] = 0; // m13 
        data[4] = sin(angle * M_PI / 180.0); // m21
        data[5] = cos(angle * M_PI / 180.0); // m22
        data[6] = 0; // m23 
        data[8] = 0; // m31
        data[9] = 0; // m32
        data[10] = 1; // m33
		Matrix4x4 temp_matrix(data);
		m_trans = m_trans * temp_matrix;
	}
}

void SceneNode::scale(const Vector3D& amount)
{
	std::cerr << "Stub: Scale " << m_name << " by " << amount << std::endl;
	Matrix4x4 temp;
	temp[0][0] = amount[0];
	temp[1][1] = amount[1];
	temp[2][2] = amount[2];
	
	m_scale = m_scale * temp;
}

void SceneNode::translate(const Vector3D& amount)
{
	std::cerr << "Stub: Translate " << m_name << " by " << amount << std::endl;
	// Fill me in
	Matrix4x4 temp;
	temp[0][3] = amount[0];
	temp[1][3] = amount[1];
	temp[2][3] = amount[2];

	m_trans = m_trans * temp;
}

bool SceneNode::is_joint() const
{
	return false;
}

void SceneNode::collectPrimitives(std::list<Primitive*> &objects)
{
	for (std::list<SceneNode*>::const_iterator i = m_children.begin(); i != m_children.end(); ++i) {
		(*i)->set_parent_transform(m_parent_trans * m_trans);
		(*i)->collectPrimitives(objects);	
	}	
}

JointNode::JointNode(const std::string& name)
: SceneNode(name)
{
}

JointNode::~JointNode()
{
}

bool JointNode::is_joint() const
{
	return true;
}

void JointNode::set_joint_x(double min, double init, double max)
{
	m_joint_x.min = min;
	m_joint_x.init = init;
	m_joint_x.max = max;
}

void JointNode::set_joint_y(double min, double init, double max)
{
	m_joint_y.min = min;
	m_joint_y.init = init;
	m_joint_y.max = max;
}

	GeometryNode::GeometryNode(const std::string& name, Primitive* primitive)
: SceneNode(name),
	m_primitive(primitive)
{
}

GeometryNode::~GeometryNode()
{
}

void GeometryNode::collectPrimitives(std::list<Primitive*> &objects)
{
	Primitive *new_object = new Primitive;
	new_object = m_primitive->clone();
	new_object->transform(m_parent_trans * m_trans * m_scale);
	new_object->setMaterial(m_material);

	objects.push_back(new_object);
	SceneNode::collectPrimitives(objects);
}
