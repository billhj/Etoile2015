/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Matrix3.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <math.h>
#include "Vectors.h"

namespace Etoile
{
	template <class T>
	class Matrix3
	{
	public:	//! casting from a Vec3<T>  to a matrix is the tilde operator
		Matrix3(const Vec3<T>  &v)
		{
			m_data[0][0]= 0,   m_data[0][1]= -v[2], m_data[0][2]= v[1];
			m_data[1][0]= v[2], m_data[1][1]=  0,   m_data[1][2]=-v[0]; 
			m_data[2][0]= -v[1],m_data[2][1]=  v[0], m_data[2][2]= 0;
		}

		Matrix3(const T * v)
		{
			m_data[0][0]= v[0],   m_data[0][1]= v[1], m_data[0][2]= v[2];
			m_data[1][0]= v[3], m_data[1][1]=  v[4],   m_data[1][2]=v[5]; 
			m_data[2][0]= v[6],m_data[2][1]=  v[7], m_data[2][2]= v[8];
		}

		Matrix3(const Matrix3& m)
		{
			for (int y = 0; y < 3; y++)
			{
				for (int x = 0; x < 3; x++) 
				{
					m_data[y][x] = m.m_data[y][x];
				}
			}
		}

		Matrix3()
		{

		}

		Matrix3(const Vec3<T>  &v0, const Vec3<T>  &v1, const Vec3<T>  &v2)
		{	m_data[0]= v0, m_data[1]= v1, m_data[2]= v2;  }

		Matrix3(T  m00, T  m01, T  m02,
			T  m10, T  m11, T  m12,
			T  m20, T  m21, T  m22)
		{  
			m_data[0][0]= m00, m_data[0][1]= m01, m_data[0][2]= m02;
			m_data[1][0]= m10, m_data[1][1]= m11, m_data[1][2]= m12;
			m_data[2][0]= m20, m_data[2][1]= m21, m_data[2][2]= m22;
		}


		//! construct an x axis rotation matrix
		inline static void rotate_x(Matrix3 &m, T  angle)
		{
			m.m_data[0][0]= 1; m.m_data[0][1]= 0; m.m_data[0][2]= 0;
			m.m_data[1][0]= 0; m.m_data[1][1]= cosf(angle); m.m_data[1][2]= sinf(angle);
			m.m_data[2][0]= 0; m.m_data[2][1]= -sinf(angle); m.m_data[2][2]= cosf(angle);
		}

		//! construct an y axis rotation matrix
		inline static void rotate_y(Matrix3 &m, T  angle)
		{
			m.m_data[0][0]= cosf(angle); m.m_data[0][1]= 0; m.m_data[0][2]= -sinf(angle);
			m.m_data[1][0]= 0; m.m_data[1][1]= 0; m.m_data[1][2]= 0;
			m.m_data[2][0]= sinf(angle); m.m_data[2][1]= 0; m.m_data[2][2]= cosf(angle);
		}

		//! construct an z axis rotation matrix
		inline static void rotate_z(Matrix3 &m, T  angle)
		{
			m.m_data[0][0]= cosf(angle); m.m_data[0][1]= sinf(angle); m.m_data[0][2]= 0;
			m.m_data[1][0]= -sinf(angle); m.m_data[1][1]= cosf(angle);  m.m_data[1][2]= 0;
			m.m_data[2][0]= 0;           m.m_data[2][1]= 0;            m.m_data[2][2]= 1;
		}

		//! constructs a rotation matrix around an arbitrary vector
		inline static void rotate(Matrix3 &m, const Vec3<T>  &v, const T  angle)
		{
			T  c= 1-cosf(angle), s= sinf(angle), cxy= c*v[0]*v[1], cxz= c*v[0]*v[2], cyz= c*v[1]*v[2];

			m.m_data[0][0]= c*v[0]*v[0] + 1-c; m.m_data[0][1]= cxy     - v[2]*s;	m.m_data[0][2]= cxz + v[1]*s; 
			m.m_data[1][0]= cxy     + v[2]*s; m.m_data[1][1]= c*v[1]*v[1] + 1-c; m.m_data[1][2]= cyz - v[0]*s; 
			m.m_data[2][0]= cxz     - v[1]*s; m.m_data[2][1]= cyz     + v[0]*s; m.m_data[2][2]= c*v[2]*v[2] + 1-c; 
		}

		//! sets a rotation matrix up by euler angles
		//! uses pitch, roll, yaw (xyz) form
		inline static void rotate_euler(Matrix3 &m, T  x, T  y, T  z)
		{
			double cx = cos(x);
			double sx = sin(x);
			double cy = cos(y);
			double sy = sin(y);
			double cz = cos(z);
			double sz = sin(z);

			m.m_data[0][0] = (T )(cy*cz); 
			m.m_data[0][1] = (T )(cy*sz);
			m.m_data[0][2] = (T )(-sy);

			double sxsy = sx*sy;
			double cxsy = cx*sy;

			m.m_data[1][0] = (T )(sxsy*cz-cx*sz);
			m.m_data[1][1] = (T )(sxsy*sz+cx*cz);
			m.m_data[1][2] = (T )(sx*cy);

			m.m_data[2][0] = (T )(cxsy*cz+sx*sz);
			m.m_data[2][1] = (T )(cxsy*sz-sx*cz);
			m.m_data[2][2] = (T )(cx*cy);
		}

		//! sets a rotation matrix up by euler angles
		//! uses pitch, roll, yaw (xyz) form
		inline static void rotate_euler(Matrix3 &m, const Vec3<T>  &angles)
		{	
			rotate_euler(m, angles[0], angles[1], angles[2]);	
		}

		//! multiplication by a matrix
		const Matrix3 &operator *=(const Matrix3 &m)
		{
			Matrix3 temp(Vec3<T> (m_data[0][0]*m.m_data[0][0] + m_data[1][0]*m.m_data[0][1] + m_data[2][0]*m.m_data[0][2],
				m_data[0][1]*m.m_data[0][0] + m_data[1][1]*m.m_data[0][1] + m_data[2][1]*m.m_data[0][2],
				m_data[0][2]*m.m_data[0][0] + m_data[1][2]*m.m_data[0][1] + m_data[2][2]*m.m_data[0][2]),
				Vec3<T> (m_data[0][0]*m.m_data[1][0] + m_data[1][0]*m.m_data[1][1] + m_data[2][0]*m.m_data[1][2],
				m_data[0][1]*m.m_data[1][0] + m_data[1][1]*m.m_data[1][1] + m_data[2][1]*m.m_data[1][2],
				m_data[0][2]*m.m_data[1][0] + m_data[1][2]*m.m_data[1][1] + m_data[2][2]*m.m_data[1][2]),
				Vec3<T> (m_data[0][0]*m.m_data[2][0] + m_data[1][0]*m.m_data[2][1] + m_data[2][0]*m.m_data[2][2],
				m_data[0][1]*m.m_data[2][0] + m_data[1][1]*m.m_data[2][1] + m_data[2][1]*m.m_data[2][2],
				m_data[0][2]*m.m_data[2][0] + m_data[1][2]*m.m_data[2][1] + m_data[2][2]*m.m_data[2][2]));		
			return *this= temp;
		}

		//! multiplication by a vector
		const Vec3<T>  operator *(const Vec3<T>  &v) const
		{	
			return Vec3<T> (v[0]*m_data[0][0] + v[1]*m_data[0][1] + v[2]*m_data[0][2], 
				v[0]*m_data[1][0] + v[1]*m_data[1][1] + v[2]*m_data[1][2], 
				v[0]*m_data[2][0] + v[1]*m_data[2][1] + v[2]*m_data[2][2]);	
		}			

		//! multiplication by a scalar
		const Matrix3 &operator *=(const T  s)
		{
			m_data[0]*=s; m_data[1]*=s; m_data[2]*=s;		
			return *this;
		}

		//! division by a scalar
		const Matrix3 &operator /=(const T  s)
		{
			m_data[0]/=s; m_data[1]/=s; m_data[2]/=s;		
			return *this;
		}

		//! addition	
		const Matrix3 operator +(const Matrix3 &m) const
		{   
			return Matrix3(m_data[0]+m.m_data[0], m_data[1]+m.m_data[1], m_data[2]+m.m_data[2]);	
		}	

		//! addition
		const Matrix3 &operator +=(const Matrix3 &m)
		{
			m_data[0]+= m.m_data[0];
			m_data[1]+= m.m_data[1];
			m_data[2]+= m.m_data[2];
			return *this;
		}

		const Vec3<T>& operator[](int i) const 
		{ 
			assert (i >= 0 && i < 3);
			return m_data[i]; 
		}

		Vec3<T> & operator[](int i) { 
			assert (i >= 0 && i < 3);
			return m_data[i]; 
		}


		//! subtraction	
		const Matrix3 operator -(const Matrix3 &m) const
		{   
			return Matrix3(m_data[0]-m.m_data[0], m_data[1]-m.m_data[1], m_data[2]-m.m_data[2]);	 
		}	

		//! subtraction
		const Matrix3 &operator -=(const Matrix3 &m)
		{
			m_data[0]-= m.m_data[0];
			m_data[1]-= m.m_data[1];
			m_data[2]-= m.m_data[2];
			return *this;
		}

		//! compute the trace
		T  trace() const
		{   return m_data[0][0] + m_data[1][1] + m_data[2][2];  }



		//! compute the determinant
		T  determinant() const
		{	
			return m_data[0][0]*(m_data[1][1]*m_data[2][2]-m_data[1][2]*m_data[2][1]) - 
				m_data[1][0]*(m_data[0][1]*m_data[2][2]-m_data[0][2]*m_data[2][1]) + 
				m_data[2][0]*(m_data[0][1]*m_data[1][2]-m_data[0][2]*m_data[1][1]);  
		}


		friend ostream& operator<<(ostream& o, const Matrix3& m)
		{
			return o << m[0] << '\t' << m[1] << '\t'<< m[2] << '\n'   << m[3] << '\t' << m[4] << '\t'<< m[5] << '\n'  << m[6] << '\t' << m[7] << '\t'<< m[8] << '\n';
		}

	private:
		Vec3<T> m_data[3];
	};

	typedef Matrix3<int> Matrix3i;
	typedef Matrix3<float> Matrix3f;
	typedef Matrix3<double> Matrix3d;
}