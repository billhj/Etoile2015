/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Primitive.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <vector>
#include "RenderMesh.h"

namespace Etoile
{
	class Primitive
	{
	public:
		std::vector<Vec3f> m_vdata, m_ndata;
		std::vector<Vec2f> m_tdata;
		std::vector<Vec3f> m_tcdata;
		std::vector<int> m_vertices_index_face;
	};


	class Quad : public Primitive
	{
	public:
		Quad(){}
		virtual void init();
	protected:

	};

	class Triangle : public Primitive
	{
	public:
		Triangle() {}
		virtual void init();
	protected:
	};


	class Plane :  public Primitive
	{
	public:
		Plane(): m_line(4){}
		void setLine(int line){m_line = line;}
		virtual void init();
		void reinit();
	protected:
		int m_line;
	};
}

