#include "Plane.h"
#include <stdio.h>

namespace Etoile
{
	Plane::Plane(Vec3f &v1,  Vec3f &v2,  Vec3f &v3) 
	{
		set3Points(v1,v2,v3);
	}


	Plane::Plane() {}

	Plane::~Plane() {}


	void Plane::set3Points(Vec3f &v1, Vec3f &v2, Vec3f &v3) 
	{
		Vec3f aux1, aux2;
		aux1 = v1 - v2;
		aux2 = v3 - v2;

		m_normal = aux2.cross3(aux1);
		m_normal.normalize();
		m_point = v2;
		m_d = -(m_normal.dot3(m_point));
	}

	void Plane::setNormalAndPoint(Vec3f &normal, Vec3f &point) 
	{
		m_normal = (normal);
		m_normal.normalize();
		m_d = -(m_normal.dot3(point));
	}

	void Plane::setCoefficients(float a, float b, float c, float d) 
	{

		// set the normal vector
		m_normal.set(a,b,c);
		//compute the lenght of the vector
		float l = m_normal.length();
		// normalize the vector
		m_normal.set(a/l,b/l,c/l);
		// and divide d by th length as well
		m_d = d/l;
	}

	float Plane::distance(Vec3f &p) {

		return (m_d + m_normal.dot3(p));
	}


}