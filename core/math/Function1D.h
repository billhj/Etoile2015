/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Function1D.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <math.h>

namespace Etoile
{
	class Function1D
	{
	protected:
		double m_center;
	public:
		Function1D(): m_center(0){}
		virtual double get(double x) = 0;
		virtual double getGradient(double x) = 0;
		void setCenter(double center){m_center = center;}
		double getCenter(){return m_center;}
	};

	class Linear : public Function1D
	{
	public:
		virtual double get(double x) override
		{
			return x;
		}

		virtual double getGradient(double x) override
		{
			return 1;
		}
	};

	class Sigmoid : public Function1D
	{
	public:
		virtual double get(double x) override
		{
			return 1/(1+exp(-(x - m_center)));
		}

		virtual double getGradient(double x) override
		{
			return  x * ( 1 - x );
		}
	};

	class GaussianFunction1D : public Function1D
	{
	protected:
		double m_sigma;
	public:
		GaussianFunction1D():Function1D(), m_sigma(0){}
		virtual double get(double x) override
		{
			return exp(-pow(x - m_center, 2) * 0.5 / pow(m_sigma, 2));
		}
		void setSigma(double sigma){m_sigma = sigma;}
		double getSigma(){return m_sigma;}

		//wrong here needs to compute
		virtual double getGradient(double x) override
		{
			return  x * ( 1 - x );
		}
	};

	class EulerIntegration : public Function1D
	{
		double m_lastY;
		double m_h;
		double m_t;
	public:
		virtual double get(double x) override
		{
			double value = m_lastY + m_h * f(m_t, m_lastY);
			m_lastY = value;
			return value;
		}

		double get(double t, double x)
		{
			double value = m_lastY + m_h * f(m_t, m_lastY);
			m_lastY = value;
			m_t = t;
			return value;
		}

		virtual double f(double t, double y) = 0;
		virtual double getGradient(double x) override
		{
			return 1;
		}
	};

	class VerletIntegration : public Function1D
	{
		double m_lastX;
		double m_lastY;
		double m__lastY;
		double m_a;
	public:
		virtual double get(double x) override
		{
			double value = 2 * m_lastY - m__lastY + m_a * x * x;
			m__lastY = m_lastY;
			m_lastY = value;
			return value;
		}

		virtual double getGradient(double x) override
		{
			return 1;
		}
	};

}