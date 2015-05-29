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

	typedef Vec3f VertexPosition;
	typedef Vec3f VertexNormal;
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
		};
	
		struct Material
		{
			ColorRGBA m_diffuse;
			ColorRGBA m_specular;
			ColorRGBA m_ambient;
			float m_shininess;
			float m_transparency;
			std::string m_diffuseTextureFilePath;
			std::string m_specularTextureFilePath;
			std::string m_ambientTextureFilePath;
			std::string m_displacementTextureFilePath;
			std::string m_bumpTextureFilePath;
		};

		struct MatGroup
		{
			std::vector<Face> m_faces;
			int m_materialIndex;
		};


		std::vector<VertexPosition> m_positions;
		std::vector<VertexNormal> m_normals;
		std::vector<TextureCoordinate> m_texcoords;
		std::vector<MatGroup> m_groups;
		std::vector<Material> m_materials;
	};
}