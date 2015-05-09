/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file RenderSubMesh.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <string>
#include <vector>
#include "math/MathHead.h"
#include "Material.h"
#include "AxisAlignedBoundingBox.h"

#ifdef USING_BOOST
#include <boost/serialization/map.hpp>
#endif

namespace Etoile
{

	class RenderSubMesh
	{
	public:
		RenderSubMesh(const std::string& name = ""): m_name(name), m_nbVerticesPerFace(3)
		{
			p_material = NULL;
		}

		RenderSubMesh(RenderSubMesh& sub): m_nbVerticesPerFace(3)
		{
			m_name = sub.m_name;
			p_material = sub.p_material;
			m_nbVerticesPerFace = sub.m_nbVerticesPerFace;
			m_numberOfFaces = sub.m_numberOfFaces;
			m_vertices_index_face = sub.m_vertices_index_face;
			m_normal_index_face = sub.m_normal_index_face;
			m_texcoord_index_face = sub.m_texcoord_index_face;
			m_vdata = sub.m_vdata;
			m_ndata = sub.m_ndata;
			m_tdata = sub.m_tdata;
		}

		virtual void release()
		{
			m_vertices_index_face.clear();
			m_normal_index_face.clear();
			m_texcoord_index_face.clear();
			m_vdata.clear();
			m_ndata.clear();
			m_tdata.clear();
		}

		const std::string getName(){return m_name;}
		void setName(const std::string& name){ m_name = name;}

		void setMaterial(Material* material)
		{
			p_material = material;
		}

		Material* getMaterial()
		{
			return p_material;
		}

		void addVertexIndex(int vertices_index)
		{
			m_vertices_index_face.push_back(vertices_index);
		}

		void addNormalIndex(int normal_index)
		{
			m_normal_index_face.push_back(normal_index);
		}

		void addTextureCoordinateIndex(int texcoord_index)
		{
			m_texcoord_index_face.push_back(texcoord_index);
		}

		std::vector<int>& getVertexIndexForFaces(){return m_vertices_index_face;}
		std::vector<int>& getNormalIndexForFaces(){return m_normal_index_face;}
		std::vector<int>&  getTextureIndexForFaces(){return m_texcoord_index_face;}
		std::vector<Vec3f>&  getVertices(){return m_vdata;} 
		std::vector<Vec3f>&  getNormals(){return m_ndata;}
		std::vector<Vec2f>& getTextureCoords(){return m_tdata;}
	
		void computeAABB()
		{
			m_aabb.build(m_vdata);
		}

		AxisAlignedBoundingBoxf* getAABB(){return &m_aabb; }

#ifdef USING_BOOST
		friend class boost::serialization::access;

		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			ar & m_name;
			ar & _material;
			ar & m_nbVerticesPerFace;
			ar & m_numberOfFaces;
			ar & m_vdata;
			ar & m_ndata;
			ar & m_tdata;
			ar & m_vertices_index_face;
		}
#endif

	public:
		int m_nbVerticesPerFace;
		int m_numberOfFaces;
		std::vector<int> m_vertices_index_face;
		std::vector<int> m_normal_index_face;
		std::vector<int> m_texcoord_index_face;
		std::vector<Vec3f> m_vdata, m_ndata;
		std::vector<Vec2f> m_tdata;
		std::string m_name;
		AxisAlignedBoundingBoxf m_aabb;
		Material* p_material;
	};
}