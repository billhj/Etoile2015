/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Mesh.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <string>
#include <vector>
#include "math/MathHead.h"
#include "SubMesh.h"
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
	class Mesh
	{
	public:
		Mesh(const std::string& name = ""): m_name(name), m_numberOfFaces(0)
		{
		}
		void setDeviceID(long id)
		{
			m_deviceID = id;
		}

		long getDeviceID(){return m_deviceID;}

		Mesh(Mesh& m)
		{
			m_name = m.m_name;
			m_subMeshList = m.m_subMeshList;
			m_numberOfFaces = m.m_numberOfFaces;

		}
		//virtual ~Mesh(){}
		const std::string getName(){return m_name;}
		void setName(const std::string& name){ m_name = name;}

		virtual void release()
		{
			for(unsigned int i = 0; i < m_subMeshList.size(); ++i)
			{
				delete m_subMeshList[i];
			}
			m_subMeshList.clear();
		}

		void cleanEmptySubMesh()
		{
			std::vector<SubMesh*>::iterator itor;
			std::vector<SubMesh*> newlist;
			for(itor = m_subMeshList.begin(); itor != m_subMeshList.end(); ++itor)
			{
				newlist.push_back(*itor);
			}
			m_subMeshList.clear();
			for(itor = newlist.begin(); itor != newlist.end(); ++itor)
			{
				SubMesh* submesh = (*itor);
				if(submesh->getOriginalVertexIndexForFaces().empty())
				{
					delete submesh;
				}else
				{
					m_subMeshList.push_back(submesh);
				}
			}
		}

		int getNumberOfFaces() const {return m_numberOfFaces;}
		unsigned int addSubMesh(SubMesh* sub)
		{
			m_subMeshList.push_back(sub);
			return m_subMeshList.size() - 1;
		}
		SubMesh* creatNewSubMesh(const std::string& name = "")
		{
			SubMesh* submesh = new SubMesh(name);
			m_subMeshList.push_back(submesh);
			return submesh;
		}

		SubMesh* getSubMesh(int idx)
		{
			return m_subMeshList[idx];
		}

		std::vector<SubMesh*>& getSubMeshList(){return m_subMeshList;}

		void computeAABB()
		{
			std::vector<AxisAlignedBoundingBoxf*> aabbs;
			for(unsigned int i = 0; i < m_subMeshList.size(); ++i)
			{
				m_subMeshList[i]->computeAABB();
				aabbs.push_back(m_subMeshList[i]->getAABB());
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
			ar & m_subMeshList;
		}
#endif

		int m_numberOfFaces;
		std::vector<SubMesh*> m_subMeshList;
		std::string m_name;
		long m_deviceID;
		AxisAlignedBoundingBoxf m_aabb;
	};

}
