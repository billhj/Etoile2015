/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Vec3.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#ifdef USING_BOOST
#include <boost/serialization/serialization.hpp>
#endif

using namespace std;

namespace Etoile
{
	template <class T>
	class Vec3 
	{

	public:
		// CONSTRUCTORS & DESTRUCTOR
		Vec3() 
		{ 
			m_data[0] = m_data[1] = m_data[2] = 0; 
		}

		Vec3(T f) 
		{ 
			m_data[0] = m_data[1] = m_data[2] = f; 
		}

		Vec3(const Vec3 &V) 
		{
			m_data[0] = V.m_data[0];
			m_data[1] = V.m_data[1];
			m_data[2] = V.m_data[2]; 
		}

		Vec3(T d0, T d1, T d2) 
		{
			m_data[0] = d0;
			m_data[1] = d1;
			m_data[2] = d2; 
		}

		Vec3(T*  d) 
		{
			m_data[0] = d[0];
			m_data[1] = d[1];
			m_data[2] = d[2];
		}

		~Vec3() {}

		// ACCESSORS
		void get(T &d0, T &d1, T &d2) const 
		{
			d0 = m_data[0];
			d1 = m_data[1];
			d2 = m_data[2]; 
		}

		const T& operator[](int i) const 
		{
			assert (i >= 0 && i < 3); 
			return m_data[i]; 
		}

		T& operator[](int i) 
		{
			assert (i >= 0 && i < 3); 
			return m_data[i]; 
		}

		bool operator <(const Vec3& v) const
		{
			if(x() == v.x())
			{
				if(y() == v.y())
				{
					return z() < v.z();
				}
				else
				{
					return y() < v.y();
				}
			}
			else
			{
				return x() < v.x();
			}
		}

		bool operator >(const Vec3& v) const
		{
			if(x() == v.x())
			{
				if(y() == v.y())
				{
					return z() > v.z();
				}
				else
				{
					return y() > v.y();
				}
			}
			else
			{
				return x() > v.x();
			}
		}

		T* getdata()
		{
			return &m_data[0];
		}

		T x() const { return m_data[0]; }
		T y() const { return m_data[1]; }
		T z() const { return m_data[2]; }
		T r() const { return m_data[0]; }
		T g() const { return m_data[1]; }
		T b() const { return m_data[2]; }

		T length() const 
		{
			T l = (T)sqrt( m_data[0] * m_data[0] +
				m_data[1] * m_data[1] +
				m_data[2] * m_data[2] );
			return l; 
		}

		// MODIFIERS
		void set(T d0, T d1, T d2) 
		{
			m_data[0] = d0;
			m_data[1] = d1;
			m_data[2] = d2; 
		}

		void scaleSelf(T d0, T d1, T d2) 
		{
			m_data[0] *= d0;
			m_data[1] *= d1;
			m_data[2] *= d2;
		}

		Vec3 scale(T  d0, T  d1, T  d2)
		{
			Vec3 v(m_data[0] * d0,
			m_data[1] * d1,
			m_data[2] * d2);
			return v;
		}

		Vec3 scale(Vec3 d) 
		{
			Vec3 v(m_data[0] * d.m_data[0],
			m_data[1] * d.m_data[1],
			m_data[2] * d.m_data[2]);
			return v;
		}

		void divide(T d0, T d1, T d2) 
		{
			m_data[0] /= d0;
			m_data[1] /= d1;
			m_data[2] /= d2;
		}

		void negate() 
		{
			m_data[0] = -m_data[0];
			m_data[1] = -m_data[1];
			m_data[2] = -m_data[2];
		}

		void normalize() 
		{
			T l = length();
			if (l > 0) 
			{
				m_data[0] /= l;
				m_data[1] /= l;
				m_data[2] /= l; 
			}
		}

		Vec3 normalized() const
		{
			T l = length();
			if (l > 0) 
			{
				Vec3 res(m_data[0] / l, m_data[1] / l,m_data[2] / l); 
				return res;
			}
			return Vec3();
		}

		Vec3 precise(int n)
		{
			Vec3 out;
			int cut = pow(double(10),n);
			int a = m_data[0] * cut + 0.5;
			int b = m_data[1] * cut + 0.5;
			int c = m_data[2] * cut + 0.5;
			out.set(double(a)/double(cut), double(b)/double(cut), double(c)/double(cut) );
			return out;
		}

		// OVERLOADED OPERATORS
		Vec3 operator-() const 
		{
			return Vec3(-m_data[0], -m_data[1], -m_data[2]); 
		}

		Vec3 operator*(T f) const
		{
			return Vec3(m_data[0] * f, m_data[1] * f, m_data[2] * f); 
		}

		Vec3 operator/(T f) const
		{
			return Vec3(m_data[0]/f, m_data[1]/f, m_data[2]/f);
		}

		Vec3& operator*=(T f) 
		{
			m_data[0] *= f;
			m_data[1] *= f;
			m_data[2] *= f;
			return (*this); 
		}

		Vec3& operator/=(T f) 
		{
			m_data[0] /= f;
			m_data[1] /= f;
			m_data[2] /= f;
			return *this; 
		}

		Vec3& operator=(const Vec3 &V) 
		{
			m_data[0] = V.m_data[0];
			m_data[1] = V.m_data[1];
			m_data[2] = V.m_data[2];
			return *this; 
		}

		Vec3 operator+(const Vec3 &f) 
		{
			return Vec3(m_data[0] + f.x(), m_data[1] + f.y(), m_data[2] + f.z());
		} 

		Vec3 operator-(const Vec3 &f) 
		{
			return Vec3(m_data[0] - f.x(), m_data[1] - f.y(), m_data[2] - f.z());
		} 

		T operator*(const Vec3 &V)const 
		{
			return m_data[0] * V.m_data[0] +
				m_data[1] * V.m_data[1] +
				m_data[2] * V.m_data[2] ; 
		}

		friend Vec3 operator+(const Vec3 &f1, const Vec3 &f2) 
		{
			return Vec3(f1.x() + f2.x(), f1.y() + f2.y(), f1.z() + f2.z());
		} 

		friend Vec3 operator-(const Vec3 &f1, const Vec3 &f2) 
		{
			return Vec3(f1.x() - f2.x(), f1.y() - f2.y(), f1.z() - f2.z());
		} 

		friend T operator*(const Vec3 &f1, const Vec3 &f2) 
		{
			return f1.x() * f2.x() +
				f1.y() * f2.y() +
				f1.z() * f2.z() ; 
		}

		int operator==(const Vec3 &V) 
		{
			return ((m_data[0] == V.m_data[0]) &&
				(m_data[1] == V.m_data[1]) &&
				(m_data[2] == V.m_data[2])); 
		}

		int operator!=(const Vec3 &V) 
		{
			return ((m_data[0] != V.m_data[0]) ||
				(m_data[1] != V.m_data[1]) ||
				(m_data[2] != V.m_data[2])); 
		}

		Vec3& operator+=(const Vec3 &V) 
		{
			m_data[0] += V.m_data[0];
			m_data[1] += V.m_data[1];
			m_data[2] += V.m_data[2];
			return *this; 
		}

		Vec3& operator-=(const Vec3 &V) 
		{
			m_data[0] -= V.m_data[0];
			m_data[1] -= V.m_data[1];
			m_data[2] -= V.m_data[2];
			return *this; 
		}

		// OPERATIONS
		T dot2(const Vec3 &V) const 
		{
			return m_data[0] * V.m_data[0] +
				m_data[1] * V.m_data[1]; 
		}

		T dot3(const Vec3 &V) const 
		{
			return m_data[0] * V.m_data[0] +
				m_data[1] * V.m_data[1] +
				m_data[2] * V.m_data[2] ; 
		}

		// STATIC OPERATIONS
		static void add(Vec3 &a, const Vec3 &b, const Vec3 &c )
		{
			a.m_data[0] = b.m_data[0] + c.m_data[0];
			a.m_data[1] = b.m_data[1] + c.m_data[1];
			a.m_data[2] = b.m_data[2] + c.m_data[2]; 
		}

		static void sub(Vec3 &a, const Vec3 &b, const Vec3 &c )
		{
			a.m_data[0] = b.m_data[0] - c.m_data[0];
			a.m_data[1] = b.m_data[1] - c.m_data[1];
			a.m_data[2] = b.m_data[2] - c.m_data[2];
		}

		static void copyScale(Vec3 &a, const Vec3 &b, T c ) 
		{
			a.m_data[0] = b.m_data[0] * c;
			a.m_data[1] = b.m_data[1] * c;
			a.m_data[2] = b.m_data[2] * c;
		}

		static void addScale(Vec3 &a, const Vec3 &b, const Vec3 &c, T d )
		{
			a.m_data[0] = b.m_data[0] + c.m_data[0] * d;
			a.m_data[1] = b.m_data[1] + c.m_data[1] * d;
			a.m_data[2] = b.m_data[2] + c.m_data[2] * d; 
		}

		static void average(Vec3 &a, const Vec3 &b, const Vec3 &c ) 
		{
			a.m_data[0] = (b.m_data[0] + c.m_data[0]) * 0.5f;
			a.m_data[1] = (b.m_data[1] + c.m_data[1]) * 0.5f;
			a.m_data[2] = (b.m_data[2] + c.m_data[2]) * 0.5f; 
		}

		static void weightedSum(Vec3 &a, const Vec3 &b, T c, const Vec3 &d, T e )
		{
			a.m_data[0] = b.m_data[0] * c + d.m_data[0] * e;
			a.m_data[1] = b.m_data[1] * c + d.m_data[1] * e;
			a.m_data[2] = b.m_data[2] * c + d.m_data[2] * e; 
		}

		static void cross3(Vec3 &c, const Vec3 &v1, const Vec3 &v2) 
		{
			T x = v1.m_data[1]*v2.m_data[2] - v1.m_data[2]*v2.m_data[1];
			T y = v1.m_data[2]*v2.m_data[0] - v1.m_data[0]*v2.m_data[2];
			T z = v1.m_data[0]*v2.m_data[1] - v1.m_data[1]*v2.m_data[0];
			c.m_data[0] = x; c.m_data[1] = y; c.m_data[2] = z; 
		}

		static Vec3 cross3(const Vec3 &v1, const Vec3 &v2) 
		{
			Vec3 c; 
			T x = v1.m_data[1]*v2.m_data[2] - v1.m_data[2]*v2.m_data[1];
			T y = v1.m_data[2]*v2.m_data[0] - v1.m_data[0]*v2.m_data[2];
			T z = v1.m_data[0]*v2.m_data[1] - v1.m_data[1]*v2.m_data[0];
			c.m_data[0] = x; c.m_data[1] = y; c.m_data[2] = z; 
			return c;
		}

		Vec3 cross3(const Vec3 &v2) const
		{
			Vec3 c; 
			T x = m_data[1]*v2.m_data[2] - m_data[2]*v2.m_data[1];
			T y = m_data[2]*v2.m_data[0] - m_data[0]*v2.m_data[2];
			T z = m_data[0]*v2.m_data[1] - m_data[1]*v2.m_data[0];
			c.m_data[0] = x; c.m_data[1] = y; c.m_data[2] = z; 
			return c;
		}


		Vec3 orthogonalVec() const
		{
		  // Find smallest component. Keep equal case for null values.
		  if ((fabs(m_data[1]) >= 0.9*fabs(m_data[0])) && (fabs(m_data[2]) >= 0.9*fabs(m_data[0])))
			return Vec3(0.0, -m_data[2], m_data[1]);
		  else
			if ((fabs(m_data[0]) >= 0.9*fabs(m_data[1])) && (fabs(m_data[2]) >= 0.9*fabs(m_data[1])))
			  return Vec3(-m_data[2], 0.0, m_data[0]);
			else
			  return Vec3(-m_data[1], m_data[0], 0.0);
		}

		friend ostream& operator<<(ostream& o, const Vec3& v)
		{
			return o << v[0] << '\t' << v[1] << '\t'<< v[2] << '\t';
		}

		// INPUT / OUTPUT
		void write(FILE *F = stdout) 
		{
			fprintf (F, "%f %f %f\n",m_data[0],m_data[1],m_data[2]); 
		}

#ifdef USING_BOOST
		friend class boost::serialization::access;

		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			ar & m_data;
		}
#endif

	private:

		// REPRESENTATION
		T m_data[3];

	};

	typedef Vec3<int> Vec3i;
	typedef Vec3<float> Vec3f;
	typedef Vec3<double> Vec3d;
}