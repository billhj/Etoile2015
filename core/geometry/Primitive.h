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
	class Primitive : public RenderMesh
	{
	public:
		Primitive(const std::string& name) : RenderMesh(name){}
		Material* getMaterial(){return getRenderSubMesh(0)->getMaterial();}
		void setMaterial(Material* material){getRenderSubMesh(0)->setMaterial(material);}
	};


	class Quad : public Primitive
	{
	public:
		Quad(const std::string& name): Primitive(name) { init();}
		virtual void init();
	protected:

	};

	class Triangle : public Primitive
	{
	public:
		Triangle(const std::string& name): Primitive(name) { init();}
		virtual void init();
	protected:
	};


	class Plane :  public Primitive
	{
	public:
		Plane(const std::string& name): Primitive(name), m_line(4){init();}
		void setLine(int line){m_line = line;}
		virtual void init();
		void reinit();
	protected:
		int m_line;
	};
}

