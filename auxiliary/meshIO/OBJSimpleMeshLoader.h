/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file OBJSimpleMeshLoader.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#define NOMINMAX
#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <istream>
#include <deque>
#include <map>
#include "geometry/SimpleMesh.h"

namespace Etoile
{
	class OBJSimpleMeshLoader
	{
	public:
		OBJSimpleMeshLoader(): m_smoothThreshold(0.6f){}
		~OBJSimpleMeshLoader(){}
		SimpleMesh* loadFromFile(const std::string& fileName);

	protected:

		SimpleMesh* read(std::istream& in);
		void handle_mtllib(std::stringstream& stream, SimpleMesh* mesh);
		int readMaterial(std::fstream& in, SimpleMesh* mesh);
		std::string extractSubString(const std::string& input, int first, int last);

		SimpleMesh::Face handle_face(const std::string& lineString);
		void trimString( std::string& _string) ;


		void regroupVertexAttributes(SimpleMesh* mesh);
		void triangulate(SimpleMesh* mesh);
		void createConnectivities(SimpleMesh* mesh);
		void calculateFaceNormals(SimpleMesh* mesh);
		void computeVertexNormals(SimpleMesh* mesh);
		void meshToUnitCube(SimpleMesh* mesh);
		void buildVertexIndices(SimpleMesh* mesh);

		

		std::string _path;

		std::map<std::string, int> m_materialNameMap;
		/*std::vector<Face> _faces;
		std::vector<Vec3f> _faceNormals;
		std::vector<std::vector<int> > _neighbourfacesIndices;
		std::vector<Vec3f> _vertices;
		std::vector<Vec3f> _normals;
		std::vector<Vec3f> _texcoords;*/
		double m_smoothThreshold;

	};

}
