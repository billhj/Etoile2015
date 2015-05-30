/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file SimpleMesh.cpp
* @brief 
* @date 1/2/2011
*/

#include "SimpleMesh.h"

namespace Etoile
{

	void SimpleMesh::regroupVertexAttributes(SimpleMesh* mesh)
	{
		if(mesh->m_texcoords.size() == 0)
		{
			mesh->m_texcoords.resize(mesh->m_positions.size());
		}
		else if(mesh->m_texcoords.size() != mesh->m_positions.size())
		{
			std::vector<TextureCoordinate> newtexcoords(mesh->m_positions.size());
			for(unsigned int i = 0; i < mesh->m_faces.size(); ++i)
			{
				SimpleMesh::Face& face = mesh->m_faces[i];
				for(int j = 0; j < face.m_verticesInfo.size(); ++j)
				{
					SimpleMesh::Vertex& vertexInfo = face.m_verticesInfo[j];
					if(vertexInfo.m_texcoordIndex != vertexInfo.m_posIndex)
					{
						newtexcoords[vertexInfo.m_posIndex] = mesh->m_texcoords[vertexInfo.m_texcoordIndex];
						vertexInfo.m_texcoordIndex = vertexInfo.m_posIndex;
					}
				}
			}
			mesh->m_texcoords = newtexcoords;
		}
		if(mesh->m_normals.size() == 0)
		{
			return;
		}
		else if(mesh->m_normals.size() != mesh->m_positions.size())
		{
			std::vector<VertexNormal> newnormals(mesh->m_positions.size());
			for(unsigned int i = 0; i < mesh->m_faces.size(); ++i)
			{
				SimpleMesh::Face& face = mesh->m_faces[i];
				for(int j = 0; j < face.m_verticesInfo.size(); ++j)
				{
					SimpleMesh::Vertex& vertexInfo = face.m_verticesInfo[j];
					if(vertexInfo.m_normalIndex != vertexInfo.m_posIndex)
					{
						newnormals[vertexInfo.m_posIndex] = mesh->m_normals[vertexInfo.m_normalIndex];
						vertexInfo.m_normalIndex = vertexInfo.m_posIndex;
					}
				}
			}
			mesh->m_normals = newnormals;
		}
	}

	void SimpleMesh::triangulate(SimpleMesh* mesh)
	{
		std::vector<SimpleMesh::Face> newfaces;
		for(unsigned int i = 0; i < mesh->m_faces.size(); ++i)
		{
			SimpleMesh::Face& face = mesh->m_faces[i];
			const int triangleIndices[][3] = {{0, 1, 2}, {0, 2, 3}};
			const int triangleCount = (face.m_verticesInfo.size() > 3) ? 2 : 1;
			if(triangleCount > 1)
			{
				for(int triangle = 0; triangle < triangleCount; ++triangle )
				{

					SimpleMesh::Face newface;
					newface.m_materialIndex = face.m_materialIndex;
					newface.m_groupIndex = face.m_groupIndex;
					for(unsigned int j = 0; j < 3; ++j)
					{
						int vIndex = triangleIndices[triangle][j];
						newface.m_verticesInfo[j].m_posIndex = face.m_verticesInfo[vIndex].m_posIndex;
						newface.m_verticesInfo[j].m_normalIndex = face.m_verticesInfo[vIndex].m_normalIndex;
						newface.m_verticesInfo[j].m_texcoordIndex = face.m_verticesInfo[vIndex].m_texcoordIndex;	
					}
					mesh->m_groups[face.m_groupIndex].m_faceIndices.push_back(newfaces.size());
					newfaces.push_back(newface);
				}

			}
			else
			{
				newfaces.push_back(face);
			}
		}
		mesh->m_faces = newfaces;
	}

	void SimpleMesh::createConnectivities(SimpleMesh* mesh)
	{
		mesh->m_neighbourfacesIndices.resize(mesh->m_positions.size());
		for(unsigned int i = 0; i < mesh->m_faces.size(); ++i)
		{
			SimpleMesh::Face& face = mesh->m_faces[i];
			for(int j = 0; j < face.m_verticesInfo.size(); ++j)
			{
				SimpleMesh::Vertex& vertexInfo = face.m_verticesInfo[j];
				std::vector<int>& indices = mesh->m_neighbourfacesIndices.at(vertexInfo.m_posIndex);
				indices.push_back(i);
			}
		}
	}

	Vec3f SimpleMesh::calculateNormal(Vec3f v0, Vec3f v1, Vec3f v2)
	{
		Vec3f edge0 = v1 - v0;
		Vec3f edge1 = v2 - v0;
		Vec3f n;
		Vec3f::cross3(n, edge0, edge1);
		n.normalize();
		return n;
	}

	void SimpleMesh::calculateFaceNormals(SimpleMesh* mesh)
	{
		for(unsigned int i = 0; i < mesh->m_faces.size(); ++i)
		{
			SimpleMesh::Face& face = mesh->m_faces[i];

			int i0 = face.m_verticesInfo[0].m_normalIndex;
			int i1 = face.m_verticesInfo[1].m_normalIndex;
			int i2 = face.m_verticesInfo[2].m_normalIndex;
			Vec3f v0 = mesh->m_normals[i0];
			Vec3f v1 = mesh->m_normals[i1];
			Vec3f v2 = mesh->m_normals[i2];
			face.m_facenormal = calculateNormal(v0, v1, v2);
		}
	}

	void SimpleMesh::computeVertexNormals(SimpleMesh* mesh)
	{
		mesh->m_normals.resize(mesh->m_positions.size());
		for(unsigned int i = 0; i < mesh->m_positions.size(); ++i)
		{
			Vec3f currentVertexNormal(0.0f, 0.0f, 0.0f);
			const std::vector<int>& neighourfaces = mesh->m_neighbourfacesIndices[i];
			int count = 1;
			for(unsigned int faceIt = 0; faceIt < neighourfaces.size(); ++faceIt)
			{
				int faceIndex = neighourfaces[faceIt];
				SimpleMesh::Face& face = mesh->m_faces[faceIndex];
				Vec3f faceNormal = face.m_facenormal;
				currentVertexNormal += faceNormal;
				++count;
			}
			currentVertexNormal.normalize();
			mesh->m_normals[i] = currentVertexNormal;
		}

		/*for(unsigned int i = 0; i < mesh->m_faces.size(); ++i)
		{
			SimpleMesh::Face& face = mesh->m_faces[i];
			for(int j = 0; j < face.m_verticesInfo.size(); ++j)
			{
				SimpleMesh::Vertex& vertexInfo = face.m_verticesInfo[j];
				vertexInfo.m_normalIndex = vertexInfo.m_posIndex;
			}
		}*/
	}

	void SimpleMesh::meshToUnitCube(SimpleMesh* mesh)
	{
		Vec3f minPos(1e6, 1e6, 1e6);
		Vec3f maxPos(-1e6, -1e6, -1e6);
		
		for (unsigned int i = 0; i < mesh->m_positions.size(); ++i)
		{
			const Vec3f& p = mesh->m_positions[i];
			for (int i = 0; i < 3; ++i)
			{
				if (p[i] > maxPos[i]) maxPos[i] = p[i];
				if (p[i] < minPos[i]) minPos[i] = p[i];
			}
		}
		
		// find the longest edge and scale it to 1
		//double scale = 1.0f / std::max( std::max(maxPos[0] - minPos[0], maxPos[1] - minPos[1]),  maxPos[2] - minPos[2]);
		double scale = 1.0f / (maxPos - minPos).length();
		Vec3f center((maxPos[0] + minPos[0]) / 2.0f,
			(maxPos[1] + minPos[1]) / 2.0f,
			(maxPos[2] + minPos[2]) / 2.0f);

		
		for (unsigned int i = 0; i < mesh->m_positions.size(); ++i)
		{
			Vec3f& p = mesh->m_positions[i];
			p -= center;
			p *= scale;
		}
	}

	void SimpleMesh::buildVertexIndices(SimpleMesh* mesh)
	{
		for(unsigned int i = 0; i < mesh->m_groups.size(); ++i)
		{
			mesh->m_groups[i].m_vertexIndicesOfFaces.clear();
		}

		for(unsigned int i = 0; i < mesh->m_faces.size(); ++i)
		{
			SimpleMesh::Face& face = mesh->m_faces[i];
			for(unsigned int j = 0; j < face.m_verticesInfo.size(); ++j)
			{
				SimpleMesh::Vertex& vertex = face.m_verticesInfo[j];
				mesh->m_groups[face.m_groupIndex].m_vertexIndicesOfFaces.push_back(vertex.m_posIndex);
			}
		}
	
	}
}