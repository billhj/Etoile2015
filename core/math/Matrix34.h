/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Matrix34.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "Vectors.h"

/************************************************************************
*  Homogeneous 3D matrix class
*
*  Rxx[0] Rxx[1] Rxx[2] P[0]	R is the rotation matrix
*  Ryx[0] Ryy[1] Ryy[2] P[1]	D is the position/translation vector
*  Rzx[0] Rzy[1] Rzz[2] P[2]
*  Dx[0]  Dy[1]  Dz[2]  P[3]   P (0, 0, 0, 1)T
*
************************************************************************/

namespace Etoile
{
	template <class T>
	class Matrix34 
	{

	public:
		Vec3<T> m_a; T m_pad0;
		Vec3<T> m_b; T m_pad1;
		Vec3<T> m_c; T m_pad2;
		Vec3<T> m_d; T m_pad3;


		/*static const Matrix34 Matrix34::IDENTITY( 1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0,
		0.0, 0.0, 0.0);*/
	public:
		inline Matrix34()
		{
			identity(); 
		}

		inline Matrix34( T ax,T ay,T az,T bx,T by,T bz,
			T cx,T cy,T cz,T dx,T dy,T dz )
		{
			m_a.x = ax; m_b.x = bx; m_c.x = cx; m_d.x = dx;
			m_a.y = ay; m_b.y = by; m_c.y = cy; m_d.y = dy;
			m_a.z = az; m_b.z = bz; m_c.z = cz; m_d.z = dz;
			m_pad0 = m_pad1 = m_pad2 = 0.0; 
			m_pad3 = 1.0;
		}

		inline Matrix34( const T* const m)
		{
			memcpy(&a, m, 16 * sizeof(T));
		}

		inline void identity()
		{
			m_a[0] = 1.0;  m_a[1] = m_a[2] = m_pad0 = 0; 
			m_b[1] = 1.0;  m_b[0] = m_b[2] = m_pad1 = 0;
			m_c[2] = 1.0;  m_c[0] = m_c[1] = m_pad2 = 0;
			m_pad3 = 1.0; m_d[0] = m_d[1] = m_d[2] = 0;
		}

		inline operator T*()
		{
			return (T*)this;
		}

		inline operator const T*() const
		{
			return (T*)this;
		}

		//Arithmetic operations
		inline const Matrix34 operator * ( const Matrix34& rhs ) const
		{
			Matrix34 lhs;

			lhs.m_a.x = m_a.x * rhs.m_a.x + m_a.y * rhs.m_b.x + m_a.z * rhs.m_c.x;
			lhs.m_a.y = m_a.x * rhs.m_a.y + m_a.y * rhs.m_b.y + m_a.z * rhs.m_c.y;
			lhs.m_a.z = m_a.x * rhs.m_a.z + m_a.y * rhs.m_b.z + m_a.z * rhs.m_c.z;

			lhs.m_b.x = m_b.x * rhs.m_a.x + m_b.y * rhs.m_b.x + m_b.z * rhs.m_c.x;
			lhs.m_b.y = m_b.x * rhs.m_a.y + m_b.y * rhs.m_b.y + m_b.z * rhs.m_c.y;
			lhs.m_b.z = m_b.x * rhs.m_a.z + m_b.y * rhs.m_b.z + m_b.z * rhs.m_c.z;

			lhs.m_c.x = m_c.x * rhs.m_a.x + m_c.y * rhs.m_b.x + m_c.z * rhs.m_c.x;
			lhs.m_c.y = m_c.x * rhs.m_a.y + m_c.y * rhs.m_b.y + m_c.z * rhs.m_c.y;
			lhs.m_c.z = m_c.x * rhs.m_a.z + m_c.y * rhs.m_b.z + m_c.z * rhs.m_c.z;

			lhs.m_d.x = m_d.x * rhs.m_a.x + m_d.y * rhs.m_b.x + m_d.z * rhs.m_c.x + rhs.m_d.x;
			lhs.m_d.y = m_d.x * rhs.m_a.y + m_d.y * rhs.m_b.y + m_d.z * rhs.m_c.y + rhs.m_d.y;
			lhs.m_d.z = m_d.x * rhs.m_a.z + m_d.y * rhs.m_b.z + m_d.z * rhs.m_c.z + rhs.m_d.z;

			return lhs;
		}

		inline const Matrix34 operator - ( const Matrix34& rhs ) const
		{
			Matrix34 lhs;

			lhs.a = a - rhs.a; lhs.m_pad0 -= rhs.m_pad0;
			lhs.b = b - rhs.b; lhs.m_pad1 -= rhs.m_pad1;
			lhs.c = c - rhs.c; lhs.m_pad2 -= rhs.m_pad2;
			lhs.d = d - rhs.d; lhs.m_pad3 -= rhs.m_pad3;

			return lhs;
		}

		inline const Matrix34 operator + ( const Matrix34& rhs ) const
		{
			Matrix34 lhs;

			lhs.a = a + rhs.a; lhs.m_pad0 += rhs.m_pad0;
			lhs.b = b + rhs.b; lhs.m_pad1 += rhs.m_pad1;
			lhs.c = c + rhs.c; lhs.m_pad2 += rhs.m_pad2;
			lhs.d = d + rhs.d; lhs.m_pad3 += rhs.m_pad3;

			return lhs;
		}

		inline const Matrix34& operator *= ( const Matrix34& rhs) 
		{
			T ax = m_a.x * rhs.m_a.x + m_a.y * rhs.m_b.x + m_a.z * rhs.m_c.x;
			T ay = m_a.x * rhs.m_a.y + m_a.y * rhs.m_b.y + m_a.z * rhs.m_c.y;
			T az = m_a.x * rhs.m_a.z + m_a.y * rhs.m_b.z + m_a.z * rhs.m_c.z;

			T bx = m_b.x * rhs.m_a.x + m_b.y * rhs.m_b.x + m_b.z * rhs.m_c.x;
			T by = m_b.x * rhs.m_a.y + m_b.y * rhs.m_b.y + m_b.z * rhs.m_c.y;
			T bz = m_b.x * rhs.m_a.z + m_b.y * rhs.m_b.z + m_b.z * rhs.m_c.z;

			T cx = m_c.x * rhs.m_a.x + m_c.y * rhs.m_b.x + m_c.z * rhs.m_c.x;
			T cy = m_c.x * rhs.m_a.y + m_c.y * rhs.m_b.y + m_c.z * rhs.m_c.y;
			T cz = m_c.x * rhs.m_a.z + m_c.y * rhs.m_b.z + m_c.z * rhs.m_c.z;

			T dx = m_d.x * rhs.m_a.x + m_d.y * rhs.m_b.x + m_d.z * rhs.m_c.x + rhs.m_d.x;
			T dy = m_d.x * rhs.m_a.y + m_d.y * rhs.m_b.y + m_d.z * rhs.m_c.y + rhs.m_d.y;
			T dz = m_d.x * rhs.m_a.z + m_d.y * rhs.m_b.z + m_d.z * rhs.m_c.z + rhs.m_d.z;

			m_a.x = ax; m_a.y = ay; m_a.z = az;
			m_b.x = bx; m_b.y = by; m_b.z = bz;
			m_c.x = cx; m_c.y = cy; m_c.z = cz;
			m_d.x = dx; m_d.y = dy; m_d.z = dz;

			return *this;
		}

		inline Matrix34& operator += ( const Matrix34& rhs) 
		{
			a += rhs.a;  m_pad0 += rhs.m_pad0;
			b += rhs.b;  m_pad1 += rhs.m_pad1;
			c += rhs.c;  m_pad2 += rhs.m_pad2;
			d += rhs.d;  m_pad3 += rhs.m_pad3;

			return *this;
		}

		inline Matrix34& operator -= ( const Matrix34& rhs) 
		{
			a -= rhs.a;  m_pad0 -= rhs.m_pad0;
			b -= rhs.b;  m_pad1 -= rhs.m_pad1;
			c -= rhs.c;  m_pad2 -= rhs.m_pad2;
			d -= rhs.d;  m_pad3 -= rhs.m_pad3;

			return *this;
		}

		inline bool operator == ( const Matrix34& rhs )
		{
			return a == rhs.a && b == rhs.b && c == rhs.c && d == rhs.d && m_pad0 == rhs.m_pad0 && m_pad1 == rhs.m_pad1 && m_pad2 == rhs.m_pad2 && m_pad3 == rhs.m_pad3;				
		}

		inline bool operator != ( const Matrix34& rhs )
		{
			return a != rhs.a && b != rhs.b && c != rhs.c && d != rhs.d && m_pad0 != rhs.m_pad0 && m_pad1 != rhs.m_pad1 && m_pad2 != rhs.m_pad2 && m_pad3 != rhs.m_pad3;
		}

		/************************************************************************
		Rotation Transformation
		**********************************************************
		************/

		//Rotations (t is given in radians)
		inline static  Matrix34 getRotationX( T t )  
		{
			Matrix34 rx;

			T st = sin(t);
			T ct = cos(t);

			rx.m_a.x = 1.0; rx.m_a.y = 0.0; rx.m_a.z = 0.0;
			rx.m_b.x = 0.0; rx.m_b.y = ct;  rx.m_b.z = st;
			rx.m_c.x = 0.0; rx.m_c.y = -st;  rx.m_c.z = ct;
			rx.m_d.makeZero();

			return rx;
		}

		inline void makeRotationX( T t)
		{
			T st = sin(t);
			T ct = cos(t);

			m_a.x = 1.0;  m_a.y = 0.0;  m_a.z = 0.0;
			m_b.x = 0.0;  m_b.y = ct;   m_b.z = st;
			m_c.x = 0.0;  m_c.y = -st;   m_c.z = ct;
			m_d.makeZero();
		}

		inline static Matrix34 getRotationY(T t)  
		{
			Matrix34 ry;

			T st = sin(t);
			T ct = cos(t);

			ry.m_a.x = ct;  ry.m_a.y = 0.0; ry.m_a.z = -st;
			ry.m_b.x = 0.0; ry.m_b.y = 1.0; ry.m_b.z = 0.0;
			ry.m_c.x = st;  ry.m_c.y = 0.0; ry.m_c.z = ct;
			ry.m_d.makeZero();

			return ry;
		}

		inline void makeRotationY( T t)
		{
			T st = sin(t);
			T ct = cos(t);

			m_a.x = ct;	 m_a.y = 0.0;  m_a.z = -st;
			m_b.x = 0.0;  m_b.y = 1.0;  m_b.z = 0.0;
			m_c.x = st;   m_c.y = 0.0;  m_c.z = ct;
			m_d.makeZero();
		}

		inline static Matrix34 getRotationZ( T t )  
		{
			Matrix34 rz;

			T st = sin(t);
			T ct = cos(t);

			rz.m_a.x = ct;   rz.m_a.y = st;  rz.m_a.z = 0.0;
			rz.m_b.x = -st;  rz.m_b.y = ct;  rz.m_b.z = 0.0;
			rz.m_c.x = 0.0;  rz.m_c.y = 0.0;  rz.m_c.z = 1.0;
			rz.m_d.makeZero();

			return rz;
		}

		inline void makeRotationZ( T t )
		{
			T st = sin(t);
			T ct = cos(t);

			m_a.x = ct;   m_a.y = st;  m_a.z = 0.0;
			m_b.x = -st;  m_b.y = ct;  m_b.z = 0.0;
			m_c.x = 0.0;  m_c.y = 0.0; m_c.z = 1.0;
			m_d.makeZero();
		}

		inline static Matrix34 getRotation( const Vec3<T>& r, T t )
		{
			Matrix34 mr;

			Vec3<T> v = r.copyNormalize();

			T ct   = cos(t);
			T st   = sin(t);
			T omct = 1.0 - ct;

			mr.m_a.x = omct * v.x * v.x + ct;
			mr.m_a.y = omct * v.x * v.y + st * v.z;
			mr.m_a.z = omct * v.x * v.z - st * v.y;
			mr.m_b.x = omct * v.x * v.y - st * v.z;
			mr.m_b.y = omct * v.y * v.y + ct;
			mr.m_b.z = omct * v.y * v.z + st * v.x;
			mr.m_c.x = omct * v.x * v.z + st * v.y;
			mr.m_c.y = omct * v.y * v.z - st * v.x;
			mr.m_c.z = omct * v.z * v.z + ct;
			mr.m_d.makeZero();

			return mr;
		}

		inline void makeRotation( const Vec3<T>& r, T t )
		{
			Vec3<T> v = r.CopyNormalize();

			T ct   = cos(t);
			T st   = sin(t);
			T omct = 1.0 - ct;

			m_a.x = omct * v.x * v.x + ct;
			m_b.y = omct * v.y * v.y + ct;
			m_c.z = omct * v.z * v.z + ct;
			m_a.y = omct * v.x * v.y + st * v.z;
			m_b.x = omct * v.x * v.y - st * v.z;
			m_a.z = omct * v.x * v.z - st * v.y;
			m_c.x = omct * v.x * v.z + st * v.y;
			m_b.z = omct * v.y * v.z + st * v.x;
			m_c.y = omct * v.y * v.z - st * v.x;
			m_d.makeZero();
		}

		inline static Matrix34 getRotation( T x, T y, T z, T t )
		{
			Vec3<T> v(x, y, z);

			return GetRotation(v, t);
		}

		inline void makeRotation( T x, T y, T z, T t )
		{
			Vec3<T> v(x, y, z);

			makeRotation(v, t);
		}

		inline Vec3<T> getRotationAxeX()
		{
			return a;
		}

		inline Vec3<T> getRotationAxeY()
		{
			return b;
		}

		inline Vec3<T> getRotationAxeZ()
		{
			return c;
		}

		inline void setRotation(const Vec3<T>& xAxis, const Vec3<T>& yAxis, const Vec3<T>& zAxis)
		{
			a = xAxis; m_a.normalize();
			b = yAxis; m_b.normalize();
			c = zAxis; m_c.normalize();
		}

		//will set only the rotation part
		inline void setRotation(const Matrix34& m)
		{
			a = m.a;
			b = m.b;
			c = m.c;
		}

		/************************************************************************
		*  Translation Transformation
		************************************************************************/

		/* Sets the Translation transformations
		*/
		inline void setTranslation(const Vec3<T>& t)
		{
			d = t;
		}

		inline void setTranslation( T x, T y, T z )
		{
			m_d.x = x; m_d.y = y; m_d.z = z;
		}

		inline Vec3<T> getTranslation()
		{
			return d;
		}

		inline static Matrix34 getTranslation( T x, T y, T z )
		{
			Matrix34 t;

			t.m_a.makeZero();
			t.m_b.makeZero();
			t.m_c.makeZero();
			t.m_d.x = x; t.m_d.y = y; t.m_d.z = z;

			return t;
		}

		inline void makeTranslation( T x, T y, T z )
		{
			m_a.makeZero();
			m_b.makeZero();
			m_c.makeZero();
			m_d.x = x;  m_d.y = y;  m_d.z = z;
		}

		inline static Matrix34 getTranslation( const Vec3<T>& t )
		{
			Matrix34 vt;

			vt.m_a.makeZero();
			vt.m_b.makeZero();
			vt.m_c.makeZero();
			vt.d = t;

			return vt;
		}

		inline void makeTranslation( const Vec3<T>& t  )
		{
			m_a.makeZero();
			m_b.makeZero();
			m_c.makeZero();
			d = t;
		}

		inline Vec3<T> getTranslation() const
		{
			return d;
		}

		/************************************************************************
		*  Scale Transformation
		************************************************************************/

		inline static Matrix34 getScale( T x, T y, T z )
		{
			Matrix34 s;

			s.m_a.x = x;   s.m_a.y = 0.0; s.m_a.z = 0.0;
			s.m_b.x = 0.0; s.m_b.y = y;   s.m_b.z = 0.0;
			s.m_c.x = 0.0; s.m_c.y = 0.0; s.m_c.z = z;
			s.m_d.makeZero();

			return s;
		}

		inline void makeScale( T x, T y, T z )
		{
			m_a.x = x;    m_a.y = 0.0;  m_a.z = 0.0;
			m_b.x = 0.0;  m_b.y = y;    m_b.z = 0.0;
			m_c.x = 0.0;  m_c.y = 0.0;  m_c.z = z;
			m_d.makeZero();
		}

		inline static Matrix34 getScale(T s)
		{					
			Matrix34 ms;

			ms.m_a.x = s;   ms.m_a.y = 0.0; ms.m_a.z = 0.0;
			ms.m_b.x = 0.0; ms.m_b.y = s;   ms.m_b.z = 0.0;
			ms.m_c.x = 0.0; ms.m_c.y = 0.0; ms.m_c.z = s;
			ms.m_d.makeZero();

			return ms;
		}

		inline void makeScale( T s )
		{
			m_a.x = s;   m_a.y = 0.0; m_a.z = 0.0;
			m_b.x = 0.0; m_b.y = s;   m_b.z = 0.0;
			m_c.x = 0.0; m_c.y = 0.0; m_c.z = s;
			m_d.makeZero();
		}

		inline void setScale( T x, T y, T z )
		{
			m_a.x = x; m_b.y = y; m_c.z = z;
		}

		inline void setScale( T s )
		{
			m_a.x = m_b.y = m_c.z = s;
		}

		/************************************************************************
		* Vector Transform
		************************************************************************/

		Vec3<T> Transform(Vec3<T> in)
		{
			T x = in.x * m_a.x + in.y * m_a.y + in.z * m_a.z;
			T y = in.x * m_b.x + in.y * m_b.y + in.z * m_b.z;
			T z = in.x * m_c.x + in.y * m_c.y + in.z * m_c.z;

			return Vec3<T>(x, y, z);
		}

		friend Vec3<T> operator* (const Vec3<T>& v, const Matrix34& m)
		{
			T x = v.x * m.m_a.x + v.y * m.m_a.y + v.z * m.m_a.z;
			T y = v.x * m.m_b.x + v.y * m.m_b.y + v.z * m.m_b.z;
			T z = v.x * m.m_c.x + v.y * m.m_c.y + v.z * m.m_c.z;

			return Vec3<T>(x, y, z);
		}

		//inline Matrix34 Transpose3x3()
		//{
		//	return	Matrix34 (m_a.x, m_b.x, m_c.x, m_a.y, m_b.y, m_c.y, m_a.z, m_b.z, m_c.z, m_d.x, m_d.y, m_d.z);		
		//}

		/************************************************************************
		*  Inverse
		************************************************************************/

		//work only on orthonormal matrices
		inline void fastInverse() 
		{
			MxAssert(isOrthonormal());

			Matrix34 inv;

			//the rotation matrix becomes the transpose of the current rotation matrix
			inv.m_a.x = m_a.x; inv.m_a.y = m_b.x; inv.m_a.z = m_c.x; 
			inv.m_b.x = m_a.y; inv.m_b.y = m_b.y; inv.m_b.z = m_c.y;
			inv.m_c.x = m_a.z; inv.m_c.y = m_b.z; inv.m_c.z = m_c.z;	

			inv.m_d.x = -m_d.dotProduct(a);
			inv.m_d.y = -m_d.dotProduct(b);
			inv.m_d.z = -m_d.dotProduct(c);

			(*this) = inv;
		}

		inline Matrix34 copyFastInverse() const
		{
			MxAssert(isOrthonormal());

			Matrix34 i;

			//the rotation matrix becomes the transpose of the current rotation matrix
			i.m_a.x = m_a.x; i.m_a.y = m_b.x; i.m_a.z = m_c.x; 
			i.m_b.x = m_a.y; i.m_b.y = m_b.y; i.m_b.z = m_c.y;
			i.m_c.x = m_a.z; i.m_c.y = m_b.z; i.m_c.z = m_c.z;

			i.m_d.x = -m_d.dotProduct(a);
			i.m_d.y = -m_d.dotProduct(b);
			i.m_d.z = -m_d.dotProduct(c);

			return i;
		}

		void inverse()
		{
			if(isOrthonormal())
			{
				fastInverse();
				return;
			}

			T m10 = m_b.x, m11 = m_b.y, m12 = m_b.z;
			T m20 = m_c.x, m21 = m_c.y, m22 = m_c.z;

			T t00 = m22 * m11 - m21 * m12;
			T t10 = m20 * m12 - m22 * m10;
			T t20 = m21 * m10 - m20 * m11;

			T m00 = m_a.x, m01 = m_a.y, m02 = m_a.z;

			T invDet = 1 / (m00 * t00 + m01 * t10 + m02 * t20);

			t00 *= invDet; t10 *= invDet; t20 *= invDet;

			m00 *= invDet; m01 *= invDet; m02 *= invDet;

			T r00 = t00;
			T r01 = m02 * m21 - m01 * m22;
			T r02 = m01 * m12 - m02 * m11;

			T r10 = t10;
			T r11 = m00 * m22 - m02 * m20;
			T r12 = m02 * m10 - m00 * m12;

			T r20 = t20;
			T r21 = m01 * m20 - m00 * m21;
			T r22 = m00 * m11 - m01 * m10;

			T m03 = m_d.x, m13 = m_d.y, m23 = m_d.z;

			T r30 = - (r00 * m03 + r01 * m13 + r02 * m23);
			T r31 = - (r10 * m03 + r11 * m13 + r12 * m23);
			T r32 = - (r20 * m03 + r21 * m13 + r22 * m23);

			m_a.x = r00; m_a.y = r01; m_a.z = r02; 
			m_b.x = r10; m_b.y = r11; m_b.z = r12;
			m_c.x = r20; m_c.y = r21; m_c.z = r22;
			m_d.x = r30; m_d.y = r31; m_d.z = r32;
		}


		Matrix34 copyInverse() const
		{
			if(isOrthonormal())
			{
				return copyFastInverse();
			}

			T m10 = m_b.x, m11 = m_b.y, m12 = m_b.z;
			T m20 = m_c.x, m21 = m_c.y, m22 = m_c.z;

			T t00 = m22 * m11 - m21 * m12;
			T t10 = m20 * m12 - m22 * m10;
			T t20 = m21 * m10 - m20 * m11;

			T m00 = m_a.x, m01 = m_a.y, m02 = m_a.z;

			T invDet = 1 / (m00 * t00 + m01 * t10 + m02 * t20);

			t00 *= invDet; t10 *= invDet; t20 *= invDet;

			m00 *= invDet; m01 *= invDet; m02 *= invDet;

			T r00 = t00;
			T r01 = m02 * m21 - m01 * m22;
			T r02 = m01 * m12 - m02 * m11;

			T r10 = t10;
			T r11 = m00 * m22 - m02 * m20;
			T r12 = m02 * m10 - m00 * m12;

			T r20 = t20;
			T r21 = m01 * m20 - m00 * m21;
			T r22 = m00 * m11 - m01 * m10;

			T m03 = m_d.x, m13 = m_d.y, m23 = m_d.z;

			T r30 = - (r00 * m03 + r01 * m13 + r02 * m23);
			T r31 = - (r10 * m03 + r11 * m13 + r12 * m23);
			T r32 = - (r20 * m03 + r21 * m13 + r22 * m23);

			return Matrix34 ( r00, r01, r02, 
				r10, r11, r12, 
				r20, r21, r22, 
				r30, r31, r32 );
		}

		/************************************************************************
		* Orthonormality
		************************************************************************/

		inline bool isOrthonormal() const
		{
			if(m_a.magnitudeSquared() != 1 || m_b.magnitudeSquared()!= 1 || m_c.magnitudeSquared() != 1)
			{
				return false;
			}

			if(m_a.dotProduct(b) != 0 || m_b.dotProduct(c) != 0 || m_c.dotProduct(a) != 0)
			{
				return false;
			}

			return true;
		}


		inline friend std::ostream& operator <<
			( std::ostream& o, const Matrix34& v )
		{				
			o << "\n " << v.m_a.x << " " << v.m_a.y << " " << v.m_a.z << " " << v.m_pad0 << "\n";
			o << " " << v.m_b.x << " " << v.m_b.y << " " << v.m_b.z << " " << v.m_pad1 << "\n";
			o << " " << v.m_c.x << " " << v.m_c.y << " " << v.m_c.z << " " << v.m_pad2 << "\n";
			o << " " << v.m_d.x << " " << v.m_d.y << " " << v.m_d.z << " " << v.m_pad3 << "\n";
			return o;
		}
	};

	typedef Matrix34<int> Matrix34i;
	typedef Matrix34<float> Matrix34f;
	typedef Matrix34<double> Matrix34d;
}