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
			int m_materialIndex;
			FaceNormal m_facenormal;
			int m_groupIndex;
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
		};

		struct Group
		{
			std::vector<int> m_faceIndex;
			int m_materialIndex;
			int m_groupIndex;
		};

		int m_facecount;
		std::vector<VertexPosition> m_positions;
		std::vector<VertexNormal> m_normals;
		std::vector<TextureCoordinate> m_texcoords;
		std::vector<Face> m_faces;
		std::vector<Group> m_groups;
		std::vector<Material> m_materials;
		std::vector<std::vector<int>> m_neighbourfacesIndices;
	};
}