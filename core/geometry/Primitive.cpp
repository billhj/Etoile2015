/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Primitive.h
* @brief 
* @date 1/2/2011
*/

#include "Primitive.h"
#include <string.h>

/**
* @brief For tracking memory leaks under windows using the crtdbg
*/
#if ( defined( _DEBUG ) || defined( DEBUG ) ) && defined( _MSC_VER )
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif

namespace Etoile
{

	void Quad::init()
	{
		m_vdata.push_back(Vec3f(  1.0, -1.0, -1));
		m_vdata.push_back(Vec3f( -1.0, -1.0, -1));
		m_vdata.push_back(Vec3f(  1.0,  1.0, -1));

		m_vdata.push_back(Vec3f( -1.0,  1.0, -1));
		m_vdata.push_back(Vec3f(  1.0,  1.0, -1));
		m_vdata.push_back(Vec3f( -1.0, -1.0, -1));

		m_tdata.push_back(Vec2f(1.0, 0.0));
		m_tdata.push_back(Vec2f(0.0, 0.0));
		m_tdata.push_back(Vec2f(1.0, 1.0));

		m_tdata.push_back(Vec2f(0.0, 1.0));
		m_tdata.push_back(Vec2f(1.0, 1.0));
		m_tdata.push_back(Vec2f(0.0, 0.0));

		m_tcdata.push_back(Vec3f(1.0f, 0.0f, 0));
		m_tcdata.push_back(Vec3f(0.0f, 0.0f, 0));
		m_tcdata.push_back(Vec3f(1.0f, 1.0f, 0));

		m_tcdata.push_back(Vec3f(0.0f, 1.0f, 0));
		m_tcdata.push_back(Vec3f(1.0f, 1.0f, 0));
		m_tcdata.push_back(Vec3f(0.0f, 0.0f, 0));

		m_ndata.push_back(Vec3f( 0.0, 0, 1.0));
		m_ndata.push_back(Vec3f( 0.0, 0, 1.0));
		m_ndata.push_back(Vec3f( 0.0, 0, 1.0));
		m_ndata.push_back(Vec3f( 0.0, 0, 1.0));
		m_ndata.push_back(Vec3f( 0.0, 0, 1.0));
		m_ndata.push_back(Vec3f( 0.0, 0, 1.0));

		m_vertices_index_face.push_back(0);
		m_vertices_index_face.push_back(1);
		m_vertices_index_face.push_back(2);
		m_vertices_index_face.push_back(3);
		m_vertices_index_face.push_back(4);
		m_vertices_index_face.push_back(5);
	}




	void Triangle::init()
	{

		m_vdata.push_back(Vec3f(-1.0, -1.0, 0.0));
		m_vdata.push_back(Vec3f( 1.0, -1.0, 0.0));
		m_vdata.push_back(Vec3f( 0.0f,  1.0, 0.0));

		m_tdata.push_back(Vec2f(1.0f, 0.0f));
		m_tdata.push_back(Vec2f(0.0f, 0.0f));
		m_tdata.push_back(Vec2f(1.0f, 1.0f));

		m_tcdata.push_back(Vec3f(1.0f, 0.0f, 0));
		m_tcdata.push_back(Vec3f(0.0f, 0.0f, 0));
		m_tcdata.push_back(Vec3f(1.0f, 1.0f, 0));

		m_ndata.push_back(Vec3f( 0.0f, 1.0f, 0));
		m_ndata.push_back(Vec3f( 0.0f, 1.0f, 0));
		m_ndata.push_back(Vec3f( 0.0f, 1.0f, 0));

		m_vertices_index_face.push_back(0);
		m_vertices_index_face.push_back(1);
		m_vertices_index_face.push_back(2);
	}

	void Box::init()
	{

		m_vdata.push_back(Vec3f(-1.0, -1.0, -1.0));
		m_vdata.push_back(Vec3f(-1.0, -1.0, 1.0));
		m_vdata.push_back(Vec3f(-1.0f, 1.0, -1.0));
		m_vdata.push_back(Vec3f(-1.0, 1.0, 1.0));
		m_vdata.push_back(Vec3f(1.0, -1.0, -1.0));
		m_vdata.push_back(Vec3f(1.0f, -1.0, 1.0));
		m_vdata.push_back(Vec3f(1.0, 1.0, -1.0));
		m_vdata.push_back(Vec3f(1.0, 1.0, 1.0));

		m_tdata.push_back(Vec2f(1.0f, 0.0f));
		m_tdata.push_back(Vec2f(0.0f, 0.0f));
		m_tdata.push_back(Vec2f(1.0f, 1.0f));

		m_tcdata.push_back(Vec3f(1.0f, 0.0f, 0));
		m_tcdata.push_back(Vec3f(0.0f, 0.0f, 0));
		m_tcdata.push_back(Vec3f(1.0f, 1.0f, 0));

		m_ndata.push_back(Vec3f( 0.0f, 1.0f, 0));
		m_ndata.push_back(Vec3f( 0.0f, 1.0f, 0));
		m_ndata.push_back(Vec3f( 0.0f, 1.0f, 0));

		m_vertices_index_face.push_back(0);
		m_vertices_index_face.push_back(1);
		m_vertices_index_face.push_back(2);
	}

	void Plane::init()
	{
		int index = 0;
		for(int i = -m_line; i < m_line; ++i)
		{
			for(int j = -m_line; j < m_line; ++j)
			{
				m_vdata.push_back(Vec3f(  1.0 + i, 0, j - 1.0));
				m_vdata.push_back(Vec3f( -1.0 + i, 0, j - 1.0));
				m_vdata.push_back(Vec3f(  1.0 + i, 0, j + 1.0));

				m_vdata.push_back(Vec3f( -1.0 + i, 0, j + 1.0));
				m_vdata.push_back(Vec3f(  1.0 + i, 0, j + 1.0));
				m_vdata.push_back(Vec3f( -1.0 + i, 0, j - 1.0));

				float x0 = (i + m_line) / (2.0 * m_line);
				float x1 = (i + m_line + 1.0) / (2.0 * m_line);
				float z0 = (j + m_line) / (2.0 * m_line);
				float z1 = (j + m_line + 1.0) / (2.0 * m_line);

				m_tdata.push_back(Vec2f(1, 0));
				m_tdata.push_back(Vec2f(0, 0));
				m_tdata.push_back(Vec2f(1, 1));

				m_tdata.push_back(Vec2f(0, 1));
				m_tdata.push_back(Vec2f(1, 1));
				m_tdata.push_back(Vec2f(0, 0));

				m_tcdata.push_back(Vec3f(1.0f, 0.0f, 0));
				m_tcdata.push_back(Vec3f(0.0f, 0.0f, 0));
				m_tcdata.push_back(Vec3f(1.0f, 1.0f, 0));

				m_tcdata.push_back(Vec3f(0.0f, 1.0f, 0));
				m_tcdata.push_back(Vec3f(1.0f, 1.0f, 0));
				m_tcdata.push_back(Vec3f(0.0f, 0.0f, 0));

				/*m_tdata.push_back(Vec2f(x1, z0));
				m_tdata.push_back(Vec2f(x0, z0));
				m_tdata.push_back(Vec2f(x1, z1));

				m_tdata.push_back(Vec2f(x0, z1));
				m_tdata.push_back(Vec2f(x1, z1));
				m_tdata.push_back(Vec2f(x0, z0));*/

				m_ndata.push_back(Vec3f( 0.0, 1.0, 0));
				m_ndata.push_back(Vec3f( 0.0, 1.0, 0));
				m_ndata.push_back(Vec3f( 0.0, 1.0, 0));
				m_ndata.push_back(Vec3f( 0.0, 1.0, 0));
				m_ndata.push_back(Vec3f( 0.0, 1.0, 0));
				m_ndata.push_back(Vec3f( 0.0, 1.0, 0));

				m_vertices_index_face.push_back(index++);
				m_vertices_index_face.push_back(index++);
				m_vertices_index_face.push_back(index++);
				m_vertices_index_face.push_back(index++);
				m_vertices_index_face.push_back(index++);
				m_vertices_index_face.push_back(index++);
			}
		}
	}

}
