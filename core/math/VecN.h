/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file VecN.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <cstring>

namespace Etoile
{
	template <class T>
	class VecN
	{
	public:
		VecN(): m_size(0),
			m_elements(NULL)
		{

		}

		VecN(unsigned long size)
			: m_elements(NULL)
		{
			setSize(size);
			makeZero();
		}

		VecN(unsigned long size, T* values)
			: m_elements(NULL)
		{
			setSize(size);
			makeZero();
			setElements(m_size, values);
		}

		VecN(const VecN& v)
		{
			m_elements = NULL;
			setSize(v.getSize());
			makeZero();
			setElements(m_size, v.m_elements);
		}

		~VecN()
		{
			if(m_elements != NULL)
			{
				delete m_elements;
			}

			m_elements = NULL;
		}

		VecN& operator=(const VecN& v)
		{
			if(m_elements != NULL)
			{
				delete []m_elements;
			}

			setSize(v.getSize());
			makeZero();
			setElements(m_size, v.m_elements);

			return *this;
		}

		T& operator[](unsigned long pos)
		{
			assert(pos < m_size);
			return m_elements[pos];
		}
		T operator[](unsigned long pos) const
		{
			assert(pos < m_size);
			return m_elements[pos];
		}

		//VecN operator+(const VecN&);
		//VecN operator+(T scalar);
		//VecN operator-(const VecN&);
		//VecN operator-(T scalar);

		//VecN& operator+=(const VecN&);
		//VecN& operator+=(T scalar);
		//VecN& operator-=(const VecN&);
		//VecN& operator-=(T scalar);

		//VecN& operator*=(T scalar);
		//VecN operator*(T scalar);

		unsigned long getSize() const
		{
			return m_size;
		}
		void setSize(unsigned long size)
		{
			this->m_size = size;

			if(m_elements)
			{
				delete []m_elements;
			}

			this->m_elements = new T[size];
		}
		void setElements(unsigned long size, T* values)
		{
			memcpy(m_elements, values, size * sizeof(T));
		}
		void makeZero()
		{
			assert(m_elements != NULL);
			for(unsigned long i = 0; i < m_size; i++)
			{
				m_elements[i] = 0.0;
			}
		}

		friend std::ostream& operator <<( std::ostream& o, const VecN& v )
		{
			for(unsigned long i = 0; i < v.getSize(); i++)
			{
				o <</* std::right << std::setw(12) <<*/ v.m_elements[i] << " ";
			}

			return o;
		}

	private:
		unsigned long m_size;
		T* m_elements;
	};

	typedef VecN<int> VecNi;
	typedef VecN<float> VecNf;
	typedef VecN<double> VecNd;
}