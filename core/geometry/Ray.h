/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Ray.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "math/Vectors.h"

namespace Etoile
{
	/*! Ray structure. Contains all information about a ray including
	*  precomputed reciprocal direction. */
	template <class T>
	class Ray
	{
		/*! Default construction does nothing. */
		inline Ray() {}

		/*! Constructs a ray from origin, direction, and ray segment. Near
		*  has to be smaller than far. */
		inline Ray(const Vec3<T>& org, const Vec3<T>& dir, T near = 0.01f, T far = 10000.0f, T time = 0.0f)
			: m_org(org), m_dir(dir), m_rdir(dir), m_near(near), m_far(far), m_time(time) {}

		void setOrigine(const Vec3<T>& origine){ m_org = origine;}
		const Vec3<T> getOrigine() const{ return m_org;}
		void setDirection(const Vec3<T>& direction){ m_dir = direction;}
		const Vec3<T> getDirection() const{ return m_dir;}
		void setReciprocalRayDirection(const Vec3<T>& direction){ m_rdir = direction;}
		const Vec3<T> getReciprocalRayDirection() const{ return m_rdir;}
		void setNearPlane(T near){ m_near = near;}
		const T getNearPlane() const{ return m_near;}
		void setFarPlane(T far){ m_far = far;}
		const T getFarPlane() const{ return m_far;}
		void setMotionBlurTime(T time){ m_time = time;}
		const T getMotionBlurTime() const{ return m_time;}

		/*! Outputs ray to stream. */
		static inline std::ostream& operator<<(std::ostream& cout, const Ray& ray) {
			return cout << "{ org = " << ray.getOrigine() << ", dir = " << ray.getDirection() << ", rdir = " << ray.getReciprocalRayDirection() 
				<< ", near = " << ray.getNearPlane() << ", far = " << ray.getFarPlane() << ", time = " << ray.time << " }";
		}
	private:
		Vec3<T> m_org;     //!< Ray origin
		Vec3<T> m_dir;     //!< Ray direction
		Vec3<T> m_rdir;    //!< Reciprocal ray direction
		T m_near;    //!< Start of ray segment
		T m_far;     //!< End of ray segment
		T m_time;    //!< Time of this ray for motion blur.
	};

	typedef Ray<float> Rayf;
	typedef Ray<double> Rayd;

}

