/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file OpenMeshLoader.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include "geometry/RenderMesh.h"
#include "MeshLoader.h"
#include "renderer/OpenGL/VBORenderSubMesh.h"

struct MyTraits : public OpenMesh::DefaultTraits
{
	//typedef OpenMesh::VectorT<float, 4> Color;

};


typedef OpenMesh::TriMesh_ArrayKernelT<MyTraits> OMesh;



namespace Etoile
{
	class OpenMeshLoader : public MeshLoader
	{
	public:
		OpenMeshLoader();
		~OpenMeshLoader();
		virtual bool loadFromFile(const std::string& fileName,RenderMesh* mesh) override;
	private:
		void createData(OMesh* omesh);
		void meshToUnitCube(OMesh& mesh);

		void loadTextures(const std::string& directory);
		OMesh* _omesh;
		double _smoothThreshold;
	};


}
