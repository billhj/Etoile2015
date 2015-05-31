/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file SimpleMesh.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <string>
#include <vector>
#include "Image/Image.h"
#include "math/MathHead.h"

namespace Etoile
{

	enum TextureMaterial
	{
		DIFFUSE_MAP,
		SPECULAR_MAP,
		AMBIENT_MAP,
		BUMP_MAP,
		DISPLACEMENT_MAP,
	};

	typedef Vec3f VertexPosition;
	typedef Vec3f VertexNormal;
	typedef Vec3f FaceNormal;
	typedef Vec3f TextureCoordinate;
	typedef Vec4f ColorRGBA;

	struct SimpleMesh
	{
		struct Vertex
		{
			int m_posIndex;
			int m_normalIndex;
			int m_texcoordIndex;
		};

		struct Face
		{
			std::vector<Vertex> m_verticesInfo;
			FaceNormal m_facenormal;
			int m_groupIndex;
			int m_materialIndex;
		};

		struct Material
		{
			std::string m_name;
			ColorRGBA m_diffuse;
			ColorRGBA m_specular;
			ColorRGBA m_ambient;
			float m_shininess;
			float m_transparency;
			std::vector<std::string> m_maps;
			std::vector<Image> m_images;
			std::vector<int> m_indicesInRessouce;
			std::vector<std::string> m_binding;

		};

		struct Group
		{
			std::string m_groupName;
			int m_groupIndex;
			int m_materialIndex;
			std::vector<int> m_faceIndices;
			std::vector<int> m_vertexIndicesOfFaces;
		};

		int m_facecount;
		std::vector<VertexPosition> m_positions;
		std::vector<VertexNormal> m_normals;
		std::vector<TextureCoordinate> m_texcoords;
		std::vector<Face> m_faces;
		std::vector<Group> m_groups;
		std::vector<Material> m_materials;
		std::vector<std::vector<int>> m_neighbourfacesIndices;

		static Vec3f calculateNormal(Vec3f v0, Vec3f v1, Vec3f v2);
		static void regroupVertexAttributes(SimpleMesh* mesh);
		static void triangulate(SimpleMesh* mesh);
		static void createConnectivities(SimpleMesh* mesh);
		static void calculateFaceNormals(SimpleMesh* mesh);
		static void computeVertexNormals(SimpleMesh* mesh);
		static void meshToUnitCube(SimpleMesh* mesh);
		static void buildVertexIndices(SimpleMesh* mesh);
	};

	
}