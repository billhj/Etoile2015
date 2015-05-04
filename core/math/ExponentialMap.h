/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file ExponentialMap.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "Vec3.h"
#include "Quaternion.h"

namespace Etoile
{
	template<class T>
	class ExponentialMap
	{
	public:
		ExponentialMap()
		{

		}

		ExponentialMap(Vec3<T> axis, T angle)
		{
			m_axis = axis.normalized();
			m_angle = angle;
		}

		ExponentialMap(Quaternion value)
		{
			m_axis = value.axis().normalized();
			m_angle = value.angle();
		}

		Quaternion getQuaternion()
		{
			Quaternion q;
			q.setAxisAngle(m_axis, m_angle);
			return q;
		}

		Vec3<T> operator *(Vec3<T> v)
		{
			return rotate(v);
		}

		ExponentialMap inverse() const
		{
			return ExponentialMap(m_axis, -m_angle);
		}

		Vec3<T> rotate(const Vec3<T> & v) const
		{
			Vec3<T> r = v * cos(m_angle)
				+ m_axis.cross3(v) * sin(m_angle)
				+ m_axis * m_axis.dot3(v) * (1 - cos(m_angle));
			return r;
		}

		Vec3<T> inverseRotate(const Vec3<T> & v) const
		{
			return inverse().rotate(v);
		}

		void invert()
		{
			m_angle = -m_angle;
		}
	private:
		Vec3<T> m_axis;
		T m_angle;

	};
}