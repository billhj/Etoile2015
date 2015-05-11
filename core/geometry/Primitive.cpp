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
		RenderSubMesh* submesh = new RenderSubMesh("Quad");
		submesh->setMaterial(new Material("empty"));
		submesh->getMaterial()->setDiffuse(Vec4f(0,0,1,1));
		this->addRenderSubMesh(submesh);
		
		submesh->getVertices().push_back(Vec3f(  1.0, -1.0, -1));
		submesh->getVertices().push_back(Vec3f( -1.0, -1.0, -1));
		submesh->getVertices().push_back(Vec3f(  1.0,  1.0, -1));

		submesh->getVertices().push_back(Vec3f( -1.0,  1.0, -1));
		submesh->getVertices().push_back(Vec3f(  1.0,  1.0, -1));
		submesh->getVertices().push_back(Vec3f( -1.0, -1.0, -1));

		submesh->getTextureCoords().push_back(Vec2f(1.0, 0.0));
		submesh->getTextureCoords().push_back(Vec2f(0.0, 0.0));
		submesh->getTextureCoords().push_back(Vec2f(1.0, 1.0));

		submesh->getTextureCoords().push_back(Vec2f(0.0, 1.0));
		submesh->getTextureCoords().push_back(Vec2f(1.0, 1.0));
		submesh->getTextureCoords().push_back(Vec2f(0.0, 0.0));

		submesh->getTextureCoordsColor().push_back(Vec3f(1.0f, 0.0f, 0));
		submesh->getTextureCoordsColor().push_back(Vec3f(0.0f, 0.0f, 0));
		submesh->getTextureCoordsColor().push_back(Vec3f(1.0f, 1.0f, 0));

		submesh->getTextureCoordsColor().push_back(Vec3f(0.0f, 1.0f, 0));
		submesh->getTextureCoordsColor().push_back(Vec3f(1.0f, 1.0f, 0));
		submesh->getTextureCoordsColor().push_back(Vec3f(0.0f, 0.0f, 0));

		submesh->getNormals().push_back(Vec3f( 0.0, 0, 1.0));
		submesh->getNormals().push_back(Vec3f( 0.0, 0, 1.0));
		submesh->getNormals().push_back(Vec3f( 0.0, 0, 1.0));
		submesh->getNormals().push_back(Vec3f( 0.0, 0, 1.0));
		submesh->getNormals().push_back(Vec3f( 0.0, 0, 1.0));
		submesh->getNormals().push_back(Vec3f( 0.0, 0, 1.0));

		submesh->getVertexIndexForFaces().push_back(0);
		submesh->getVertexIndexForFaces().push_back(1);
		submesh->getVertexIndexForFaces().push_back(2);
		submesh->getVertexIndexForFaces().push_back(3);
		submesh->getVertexIndexForFaces().push_back(4);
		submesh->getVertexIndexForFaces().push_back(5);
	}




	void Triangle::init()
	{
		RenderSubMesh* submesh = new RenderSubMesh("Triangle");
		submesh->setMaterial(new Material("empty"));
		submesh->getMaterial()->setDiffuse(Vec4f(0,0,1,1));
		this->addRenderSubMesh(submesh);
		
		submesh->getVertices().push_back(Vec3f(-1.0, -1.0, 0.0));
		submesh->getVertices().push_back(Vec3f( 1.0, -1.0, 0.0));
		submesh->getVertices().push_back(Vec3f( 0.0f,  1.0, 0.0));

		submesh->getTextureCoords().push_back(Vec2f(1.0f, 0.0f));
		submesh->getTextureCoords().push_back(Vec2f(0.0f, 0.0f));
		submesh->getTextureCoords().push_back(Vec2f(1.0f, 1.0f));

		submesh->getTextureCoordsColor().push_back(Vec3f(1.0f, 0.0f, 0));
		submesh->getTextureCoordsColor().push_back(Vec3f(0.0f, 0.0f, 0));
		submesh->getTextureCoordsColor().push_back(Vec3f(1.0f, 1.0f, 0));

		submesh->getNormals().push_back(Vec3f( 0.0f, 1.0f, 0));
		submesh->getNormals().push_back(Vec3f( 0.0f, 1.0f, 0));
		submesh->getNormals().push_back(Vec3f( 0.0f, 1.0f, 0));

		submesh->getVertexIndexForFaces().push_back(0);
		submesh->getVertexIndexForFaces().push_back(1);
		submesh->getVertexIndexForFaces().push_back(2);
	}

	void Plane::init()
	{
		RenderSubMesh* submesh = new RenderSubMesh("Plane");
		submesh->setMaterial(new Material("empty"));
		submesh->getMaterial()->setDiffuse(Vec4f(0.9,0.9,0.9,1));
		this->addRenderSubMesh(submesh);
		int index = 0;
		for(int i = -m_line; i < m_line; ++i)
		{
			for(int j = -m_line; j < m_line; ++j)
			{
				submesh->getVertices().push_back(Vec3f(  1.0 + i, 0, j - 1.0));
				submesh->getVertices().push_back(Vec3f( -1.0 + i, 0, j - 1.0));
				submesh->getVertices().push_back(Vec3f(  1.0 + i, 0, j + 1.0));

				submesh->getVertices().push_back(Vec3f( -1.0 + i, 0, j + 1.0));
				submesh->getVertices().push_back(Vec3f(  1.0 + i, 0, j + 1.0));
				submesh->getVertices().push_back(Vec3f( -1.0 + i, 0, j - 1.0));

				float x0 = (i + m_line) / (2.0 * m_line);
				float x1 = (i + m_line + 1.0) / (2.0 * m_line);
				float z0 = (j + m_line) / (2.0 * m_line);
				float z1 = (j + m_line + 1.0) / (2.0 * m_line);

				submesh->getTextureCoords().push_back(Vec2f(1, 0));
				submesh->getTextureCoords().push_back(Vec2f(0, 0));
				submesh->getTextureCoords().push_back(Vec2f(1, 1));

				submesh->getTextureCoords().push_back(Vec2f(0, 1));
				submesh->getTextureCoords().push_back(Vec2f(1, 1));
				submesh->getTextureCoords().push_back(Vec2f(0, 0));

				submesh->getTextureCoordsColor().push_back(Vec3f(1.0f, 0.0f, 0));
				submesh->getTextureCoordsColor().push_back(Vec3f(0.0f, 0.0f, 0));
				submesh->getTextureCoordsColor().push_back(Vec3f(1.0f, 1.0f, 0));

				submesh->getTextureCoordsColor().push_back(Vec3f(0.0f, 1.0f, 0));
				submesh->getTextureCoordsColor().push_back(Vec3f(1.0f, 1.0f, 0));
				submesh->getTextureCoordsColor().push_back(Vec3f(0.0f, 0.0f, 0));

				/*submesh->getTextureCoords().push_back(Vec2f(x1, z0));
				submesh->getTextureCoords().push_back(Vec2f(x0, z0));
				submesh->getTextureCoords().push_back(Vec2f(x1, z1));

				submesh->getTextureCoords().push_back(Vec2f(x0, z1));
				submesh->getTextureCoords().push_back(Vec2f(x1, z1));
				submesh->getTextureCoords().push_back(Vec2f(x0, z0));*/

				submesh->getNormals().push_back(Vec3f( 0.0, 1.0, 0));
				submesh->getNormals().push_back(Vec3f( 0.0, 1.0, 0));
				submesh->getNormals().push_back(Vec3f( 0.0, 1.0, 0));
				submesh->getNormals().push_back(Vec3f( 0.0, 1.0, 0));
				submesh->getNormals().push_back(Vec3f( 0.0, 1.0, 0));
				submesh->getNormals().push_back(Vec3f( 0.0, 1.0, 0));

				submesh->getVertexIndexForFaces().push_back(index++);
				submesh->getVertexIndexForFaces().push_back(index++);
				submesh->getVertexIndexForFaces().push_back(index++);
				submesh->getVertexIndexForFaces().push_back(index++);
				submesh->getVertexIndexForFaces().push_back(index++);
				submesh->getVertexIndexForFaces().push_back(index++);
			}
		}
		this->computeAABB();
	}

	void Plane::reinit()
	{
		RenderSubMesh* submesh = this->getRenderSubMesh(0);
		submesh->getTextureCoords().clear();
		submesh->getNormals().clear();
		submesh->getVertexIndexForFaces().clear();
		int index = 0;
		for(int i = -m_line; i < m_line; ++i)
		{
			for(int j = -m_line; j < m_line; ++j)
			{
				submesh->getVertices().push_back(Vec3f(  1.0 + i, 0, j - 1.0));
				submesh->getVertices().push_back(Vec3f( -1.0 + i, 0, j - 1.0));
				submesh->getVertices().push_back(Vec3f(  1.0 + i, 0, j + 1.0));

				submesh->getVertices().push_back(Vec3f( -1.0 + i, 0, j + 1.0));
				submesh->getVertices().push_back(Vec3f(  1.0 + i, 0, j + 1.0));
				submesh->getVertices().push_back(Vec3f( -1.0 + i, 0, j - 1.0));

				float x0 = (i + m_line) / (2.0 * m_line);
				float x1 = (i + m_line + 1.0) / (2.0 * m_line);
				float z0 = (j + m_line) / (2.0 * m_line);
				float z1 = (j + m_line + 1.0) / (2.0 * m_line);

				submesh->getTextureCoords().push_back(Vec2f(1, 0));
				submesh->getTextureCoords().push_back(Vec2f(0, 0));
				submesh->getTextureCoords().push_back(Vec2f(1, 1));

				submesh->getTextureCoords().push_back(Vec2f(0, 1));
				submesh->getTextureCoords().push_back(Vec2f(1, 1));
				submesh->getTextureCoords().push_back(Vec2f(0, 0));


				submesh->getTextureCoordsColor().push_back(Vec3f(1.0f, 0.0f, 0));
				submesh->getTextureCoordsColor().push_back(Vec3f(0.0f, 0.0f, 0));
				submesh->getTextureCoordsColor().push_back(Vec3f(1.0f, 1.0f, 0));

				submesh->getTextureCoordsColor().push_back(Vec3f(0.0f, 1.0f, 0));
				submesh->getTextureCoordsColor().push_back(Vec3f(1.0f, 1.0f, 0));
				submesh->getTextureCoordsColor().push_back(Vec3f(0.0f, 0.0f, 0));

				/*submesh->getTextureCoords().push_back(Vec2f(x1, z0));
				submesh->getTextureCoords().push_back(Vec2f(x0, z0));
				submesh->getTextureCoords().push_back(Vec2f(x1, z1));

				submesh->getTextureCoords().push_back(Vec2f(x0, z1));
				submesh->getTextureCoords().push_back(Vec2f(x1, z1));
				submesh->getTextureCoords().push_back(Vec2f(x0, z0));*/

				submesh->getNormals().push_back(Vec3f( 0.0, 1.0, 0));
				submesh->getNormals().push_back(Vec3f( 0.0, 1.0, 0));
				submesh->getNormals().push_back(Vec3f( 0.0, 1.0, 0));
				submesh->getNormals().push_back(Vec3f( 0.0, 1.0, 0));
				submesh->getNormals().push_back(Vec3f( 0.0, 1.0, 0));
				submesh->getNormals().push_back(Vec3f( 0.0, 1.0, 0));

				submesh->getVertexIndexForFaces().push_back(index++);
				submesh->getVertexIndexForFaces().push_back(index++);
				submesh->getVertexIndexForFaces().push_back(index++);
				submesh->getVertexIndexForFaces().push_back(index++);
				submesh->getVertexIndexForFaces().push_back(index++);
				submesh->getVertexIndexForFaces().push_back(index++);
			}
		}
		this->computeAABB();
	}
}
