/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file RenderMesh.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <string>
#include <vector>
#include "math/MathHead.h"
#include "RenderSubMesh.h"
#include "Component.h"

#ifdef USING_BOOST
#include <boost/serialization/serialization.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/map.hpp>
#endif

namespace Etoile
{
	struct RenderMesh
	{
	public:
		RenderMesh(const std::string& name = ""): m_name(name), m_numberOfFaces(0)
		{
		}

		~RenderMesh()
		{
			release();
		}

		void setDeviceID(long id)
		{
			m_deviceID = id;
		}

		long getDeviceID(){return m_deviceID;}

		RenderMesh(RenderMesh& m)
		{
			m_name = m.m_name;
			m_subRenderMeshList = m.m_subRenderMeshList;
			m_numberOfFaces = m.m_numberOfFaces;
			m_aabb = m.m_aabb;

		}
		//virtual ~RenderMesh(){}

		virtual void release()
		{
			for(unsigned int i = 0; i < m_subRenderMeshList.size(); ++i)
			{
				delete m_subRenderMeshList[i];
			}
			m_subRenderMeshList.clear();
		}

		void cleanEmptyRenderSubMesh()
		{
			std::vector<RenderSubMesh*>::iterator itor;
			std::vector<RenderSubMesh*> newlist;
			for(itor = m_subRenderMeshList.begin(); itor != m_subRenderMeshList.end(); ++itor)
			{
				newlist.push_back(*itor);
			}
			m_subRenderMeshList.clear();
			for(itor = newlist.begin(); itor != newlist.end(); ++itor)
			{
				RenderSubMesh* submesh = (*itor);
				if(submesh->m_vertices_index_face.empty())
				{
					delete submesh;
				}else
				{
					m_subRenderMeshList.push_back(submesh);
				}
			}
		}

		int getNumberOfFaces() const {return m_numberOfFaces;}
		unsigned int addRenderSubMesh(RenderSubMesh* sub)
		{
			m_subRenderMeshList.push_back(sub);
			return m_subRenderMeshList.size() - 1;
		}
		/*RenderSubMesh* creatNewRenderSubMesh(const std::string& name = "")
		{
			RenderSubMesh* submesh = new RenderSubMesh(name);
			m_subRenderMeshList.push_back(submesh);
			return submesh;
		}*/

		RenderSubMesh* getRenderSubMesh(int idx)
		{
			return m_subRenderMeshList[idx];
		}

		std::vector<RenderSubMesh*>& getRenderSubMeshList(){return m_subRenderMeshList;}

		void computeAABB()
		{
			std::vector<AxisAlignedBoundingBoxf*> aabbs;
			for(unsigned int i = 0; i < m_subRenderMeshList.size(); ++i)
			{
				m_subRenderMeshList[i]->computeAABB();
				aabbs.push_back(m_subRenderMeshList[i]->getAABB());
			}
			m_aabb.build(aabbs);
		}

		


#ifdef USING_BOOST
		friend class boost::serialization::access;

		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			ar & m_name;
			ar & m_numberOfFaces;
			ar & m_subRenderMeshList;
		}
#endif

		int m_numberOfFaces;
		std::vector<RenderSubMesh*> m_subRenderMeshList;
		std::string m_name;
		long m_deviceID;
		AxisAlignedBoundingBoxf m_aabb;
	};

}
