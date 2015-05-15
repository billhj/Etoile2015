/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file XMLMeshLoader.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <string>
#include "tinyxml/tinyxml.h"
#include "geometry/RenderMesh.h"
#include "renderer/OpenGL/VBORenderSubMesh.h"
#include "MeshLoader.h"
namespace Etoile
{
	class XMLMeshLoader : public MeshLoader
	{
	public:
		XMLMeshLoader();
		~XMLMeshLoader();
		virtual bool loadFromFile(const std::string& fileName,RenderMesh* mesh) override;
	private:
		bool loadMeshes();
		bool loadPoses();
		void meshToUnitCube(VBORenderSubMesh* submesh);
		void checkSpecifiedPose(const std::string& nameOfPose);
		void readVBORenderSubMesh(TiXmlElement *elemVBORenderSubMesh);
		void readFaces(TiXmlElement *elemFaces, VBORenderSubMesh* submesh);
		void readGeometry(TiXmlElement *elemGeometry, VBORenderSubMesh* submesh);
		void readBoneAssignment(TiXmlElement *elemBoneAssignment, VBORenderSubMesh* submesh);
		void readPoses(TiXmlElement *elemPoses);
		void readAnimations(TiXmlElement *elemAnimations);
		void readTracks(TiXmlElement *elemTracks);

		TiXmlDocument* _pDoc;
	};

}
