/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file OBJSimpleMeshLoader.cpp
* @brief 
* @date 1/2/2011
*/

#include "OBJSimpleMeshLoader.h"
//#include "renderer/OpenGL/GLTexture2D.h"
/**
* @brief For tracking memory leaks under windows using the crtdbg
*/
#if ( defined( _DEBUG ) || defined( DEBUG ) ) && defined( _MSC_VER )
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif

namespace Etoile
{

	SimpleMesh* OBJSimpleMeshLoader::loadFromFile(const std::string& fileName)
	{
		std::fstream in(fileName.c_str(), std::ios_base::in );

		if (!in.is_open() || !in.good())
		{
			std::cerr << "[OBJReader] : cannot not open file "
				<< fileName
				<< std::endl;
			return false;
		}

		{
#if defined(WIN32)
			std::string::size_type dot = fileName.find_last_of("\\/");
#else
			std::string::size_type dot = fileName.rfind("/");
#endif
			_path = (dot == std::string::npos)
				? "./"
				: std::string(fileName.substr(0,dot+1));
		}

		SimpleMesh* result = read(in);

		in.close();
		return result;

	}

	SimpleMesh* OBJSimpleMeshLoader::read(std::istream& in)
	{
		std::cout << "[OBJReader] : read file"<<std::endl;
		SimpleMesh* mesh = new SimpleMesh();
		std::string line;
		std::string keyWrd;
		double x, y, z, u, v;


		std::string matname;
		int materialIndex = -1;

		int lineIndex = 0;

		while( in && !in.eof() )
		{
			lineIndex++;
			if( lineIndex % 10000 == 0)
				std::cout << /*lineIndex*/ "."<<std::flush;

			std::getline(in,line);
			if ( in.bad() ){
				std::cout << "  Warning! Could not read file properly!"<<std::endl;
				return false;
			}

			// Trim Both leading and trailing spaces
			trimString(line);

			// comment
			if ( line.size() == 0 || line[0] == '#' || isspace(line[0]) || line.empty() ) {
				continue;
			}

			std::stringstream stream(line);

			stream >> keyWrd;


			// material file
			if (keyWrd == "mtllib")
			{
				handle_mtllib(stream, mesh);
			}
			// usemtl
			else if (keyWrd == "usemtl")
			{
				stream >> matname;
				std::map<std::string, int>::iterator itor = m_materialNameMap.find(matname);
				if(itor != m_materialNameMap.end())
				{
					materialIndex = itor->second;
				}else{
					materialIndex = 0;
				}
			}
			//PB is the files for materials, using usemtl or not, the obj file is not uniform
			else if ( keyWrd == "g")
			{
				/*std::string objectName;
				stream >> objectName;
				materialIndex = -1;
				if(!objectName.empionty())
				{
				std::cout << "found object: " << objectName << std::endl;
				}*/
			}

			// vertex
			else if (keyWrd == "v")
			{

				stream >> x; stream >> y; stream >> z;

				if ( !stream.fail() )
					mesh->m_positions.push_back(Vec3f(x,y,z));
				else{
					std::cout << " vertex error"<< std::endl;
					return false;
				}
				//            if( (_vertices.size() + 1 )% 10000 == 0)
				//                std::cout << "Vertex count : " <<_vertices.size()<<std::endl;

			}

			// texture coord
			else if (keyWrd == "vt")
			{
				stream >> u; stream >> v;

				if ( !stream.fail() ){

					mesh->m_texcoords.push_back(Vec3f(u, v, 0));

				}else{

					std::cout << "Only single 2D texture coordinate per vertex"
						<< "allowed!" << std::endl;
					return false;
				}
				//            if( (_texcoords.size() + 1 )% 10000 == 0)
				//                std::cout << "Tex count : " <<_texcoords.size()<<std::endl;
			}


			// normal
			else if (keyWrd == "vn")
			{
				stream >> x; stream >> y; stream >> z;

				if ( !stream.fail() ){
					mesh->m_normals.push_back(Vec3f(x,y,z));
				}else{

					std::cout << "normal vector error"<< std::endl;
					return false;
				}
				//            if( (_normals.size() + 1 )% 10000 == 0)
				//                std::cout << "Normal count : " <<_normals.size()<<std::endl;
			}


			// face
			else if (keyWrd == "f")
			{
				
				mesh->m_facecount++;
				// read full line after detecting a face
				std::string faceLine;
				std::getline(stream,faceLine);
				SimpleMesh::Face face = handle_face(faceLine);
				face.m_materialIndex = materialIndex;
				mesh->m_matgroups[materialIndex].m_faceIndex.push_back(mesh->m_faces.size());
				mesh->m_faces.push_back(face);
				//            if( (_faces.size() + 1 ) % 10000 == 0)
				//                std::cout << "Face count : " <<_faces.size()<<std::endl;

			}

		}
		std::cout << std::endl<<
			"finish reading : "<<
			"Vertex count : " << mesh->m_positions.size()<<" "<<
			"Tex count : " << mesh->m_texcoords.size()<<" "<<
			"Normal count : " << mesh->m_normals.size()<<" "<<
			"Face count : " <<mesh->m_facecount<<std::endl;

		regroupVertexAttributes(mesh);
		std::cout<<"regroup attributes done!" << std::endl;
		triangulate(mesh);
		std::cout<<"Triangulation done!" <<std::endl;

		createConnectivities(mesh);
		std::cout<<"Connectivities done" <<std::endl;
		calculateFaceNormals(mesh);
		std::cout<<"face normals created done" <<std::endl;

		if(mesh->m_normals.size() == 0)
		{
			computeVertexNormals(mesh);
			std::cout<<"vertex normals recreated done" <<std::endl;
		}

		meshToUnitCube(mesh);
		return mesh;
	}

	void OBJSimpleMeshLoader::handle_mtllib(std::stringstream& stream, SimpleMesh* mesh)
	{
		std::string matFile;

		// Get the rest of the line, removing leading or trailing spaces
		// This will define the filename of the texture
		std::getline(stream,matFile);
		trimString(matFile);

		matFile = _path + matFile;

		std::cout << "Load material file " << matFile << std::endl;

		std::fstream matStream( matFile.c_str(), std::ios_base::in );

		int nb_material = 0;
		if ( matStream ){
			nb_material = readMaterial( matStream, mesh);
			if ( nb_material == 0 )
				std::cout << "  Warning! Could not read file properly!"<<std::endl;
			matStream.close();

		}else
			std::cout << "  Warning! Material file '" << matFile << "' not found!\n";

		std::cout << "  " << nb_material << " materials loaded."<<std::endl;
	}

	int OBJSimpleMeshLoader::readMaterial(std::fstream& in, SimpleMesh* mesh)
	{

		std::string line;
		std::string keyWrd;
		//std::string textureName;

		std::string key;
		double f1,f2,f3;
		
		m_materialNameMap["empty"] = 0;
		mesh->m_materials.push_back(SimpleMesh::Material());
		mesh->m_matgroups.push_back(SimpleMesh::MatGroup());
		mesh->m_matgroups.back().m_materialIndex = 0;
		SimpleMesh::Material* currentMat = &(mesh->m_materials.back());
		currentMat->m_name = "empty";
		int nb_material = 1;

		while( in && !in.eof() )
		{
			std::getline(in,line);
			if ( in.bad() ){
				std::cout << "  Warning! Could not read file properly!"<<std::endl;
				return 0;
			}

			if ( line.empty() )
				continue;

			std::stringstream stream(line);

			stream >> keyWrd;


			// oldcode:
			//if( isspace(line[0]) || line[0] == '#' )
			// isspace might be a tab at the beginning of the line...
			// ignore it
			if( /*( isspace(line[0]) && line[ 0 ] != 0x09 ) ||*/ line[0] == '#' || line.empty())
			{
				if (!key.empty())
				{
					/*_materials[key] = mat;
					mat.cleanup();*/
				}
			}
			else if (keyWrd == "newmtl") // begin new material definition
			{
				stream >> key;
				m_materialNameMap[key] = nb_material;
				mesh->m_materials.push_back(SimpleMesh::Material());
				SimpleMesh::Material* currentMat = &(mesh->m_materials.back());
				currentMat->m_name = key;

				mesh->m_matgroups.push_back(SimpleMesh::MatGroup());
				mesh->m_matgroups.back().m_materialIndex = nb_material;
				nb_material++;
			}

			else if (keyWrd == "Kd") // diffuse color
			{
				stream >> f1; stream >> f2; stream >> f3;

				if( !stream.fail() )
				{
					currentMat->m_diffuse = ColorRGBA(f1, f2, f3, 0);
				}
			}

			else if (keyWrd == "Ka") // ambient color
			{
				stream >> f1; stream >> f2; stream >> f3;

				if( !stream.fail() )
				{
					currentMat->m_ambient = ColorRGBA(f1, f2, f3, 0);
				}
			}

			else if (keyWrd == "Ks") // specular color
			{
				stream >> f1; stream >> f2; stream >> f3;

				if( !stream.fail() )
				{
					currentMat->m_specular = ColorRGBA(f1, f2, f3, 0);
				}
			}

			else if (keyWrd == "illum") // diffuse/specular shading model
			{
				; // just skip this
			}

			else if (keyWrd == "Ns") // Shininess [0..200]
			{
				stream >> f1;
				if( !stream.fail() )
				{
					currentMat->m_shininess = f1;
				} // just skip this
			}

			else if (keyWrd == "map_") // map images
			{
				// map_Ks, specular map
				// map_Ka, ambient map
				// map_Bump, bump map
				// map_d,  opacity map
				; // just skip this
			}

			else if (keyWrd == "map_Kd" ) {
				// Get the rest of the line, removing leading or trailing spaces
				// This will define the filename of the texture
				std::string textureName;
				std::getline(stream,textureName);
				trimString(textureName);

				size_t pos = textureName.find("\\");
				if(pos != textureName.npos)
				{
					textureName[pos] = '/';
				}

				if(!textureName.empty())
				{
					std::string s = _path + textureName;
					currentMat->m_diffuseTextureFilePath = s;
				}
			}
			else if (keyWrd == "map_Ks") // map images
			{
				std::string textureName;
				std::getline(stream,textureName);
				trimString(textureName);
				if ( ! textureName.empty() )
				{
					std::string s = _path + textureName;
					currentMat->m_specularTextureFilePath = s;
				}
			}
			else if (keyWrd == "map_bump") // map images
			{
				std::string textureName;
				std::getline(stream,textureName);
				trimString(textureName);
				if ( ! textureName.empty() )
				{
					std::string s = _path + textureName;
					currentMat->m_bumpTextureFilePath = s;
				}
			}
			else if (keyWrd == "Tr") // transparency value
			{
				stream >> f1;
				if( !stream.fail() )
				{
					currentMat->m_transparency = f1;
				}
			}
			else if (keyWrd == "d") // transparency value
			{
				stream >> f1;
				if( !stream.fail() )
				{
					currentMat->m_transparency = f1;
				}
			}
		}
		return nb_material;
	}




	std::string OBJSimpleMeshLoader::extractSubString(const std::string& input, int first, int last)
	{
		std::string ret;
		for(int i = first; i <= last; ++i)
		{
			ret += input[i];
		}
		return ret;
	}

	SimpleMesh::Face OBJSimpleMeshLoader::handle_face(const std::string& lineString)
	{
		std::stringstream lineData(lineString);
		SimpleMesh::Face face;
		std::vector<std::string> components;
		while(!lineData.eof())
		{
			std::string tmp;
			lineData >> tmp;
			components.push_back(tmp);
		}
		int vertexCount = components.size();

		for(unsigned int i = 0; i < components.size(); ++i)
		{
			face.m_verticesInfo.push_back(SimpleMesh::Vertex());
			const std::string& component = components[i];
			int pos0 = component.find("/");
			if( pos0 == -1)
			{
				int value;
				std::stringstream tmp( component );
				tmp >> value;
				face.m_verticesInfo[i].m_posIndex = abs(value) - 1;
			}
			else
			{
				int pos1  = component.find("/", pos0 + 1);
				if( pos1 == -1)
				{
					std::string vertexString = extractSubString(component, 0, pos0 - 1);
					std::string textureCoordString = extractSubString(component, pos0 + 1, component.size() - 1);

					int valueVertex, valueTextureCoord;
					std::stringstream ssvertex(vertexString);
					ssvertex >> valueVertex;

					std::stringstream ssTextureCoord(textureCoordString);
					ssTextureCoord >> valueTextureCoord;

					face.m_verticesInfo[i].m_posIndex = abs(valueVertex) - 1;
					face.m_verticesInfo[i].m_texcoordIndex = abs(valueTextureCoord) - 1;
					//face._hasTextureCoordIndex = true;
				}
				else if( (pos1 - pos0) == 1) // Vertex/normal case
				{
					std::string vertexString = extractSubString(component, 0, pos0 - 1);
					std::string normalString = extractSubString(component, pos0 + 2, component.size() - 1);

					int valueVertex, valueNormal;
					std::stringstream ssvertex(vertexString);
					ssvertex >> valueVertex;
					std::stringstream ssnormal(normalString);
					ssnormal >> valueNormal;

					face.m_verticesInfo[i].m_posIndex = abs(valueVertex) - 1;
					face.m_verticesInfo[i].m_normalIndex = abs(valueNormal) - 1;
				}
				else
				{
					std::string vertexString = extractSubString(component, 0, pos0 - 1);
					std::string textureCoordString = extractSubString(component, pos0 + 1, pos1 - 1);
					std::string normalString = extractSubString(component, pos1 + 1, component.size() - 1);

					int valueVertex, valueTextureCoord, valueNormal;
					std::stringstream ssvertex(vertexString);
					ssvertex >> valueVertex;
					std::stringstream ssnormal(normalString);
					ssnormal >> valueNormal;
					std::stringstream ssTextureCoord(textureCoordString);
					ssTextureCoord >> valueTextureCoord;

					face.m_verticesInfo[i].m_posIndex = abs(valueVertex) - 1;
					face.m_verticesInfo[i].m_texcoordIndex = abs(valueTextureCoord) - 1;
					face.m_verticesInfo[i].m_normalIndex = abs(valueNormal) - 1;
				}
			}

		}

		return face;
	}

	void OBJSimpleMeshLoader::regroupVertexAttributes(SimpleMesh* mesh)
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

	void OBJSimpleMeshLoader::triangulate(SimpleMesh* mesh)
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
					for(unsigned int j = 0; j < 3; ++j)
					{
						int vIndex = triangleIndices[triangle][j];
						newface.m_verticesInfo[j].m_posIndex = face.m_verticesInfo[vIndex].m_posIndex;
						newface.m_verticesInfo[j].m_normalIndex = face.m_verticesInfo[vIndex].m_normalIndex;
						newface.m_verticesInfo[j].m_texcoordIndex = face.m_verticesInfo[vIndex].m_texcoordIndex;
						
					}
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

	void OBJSimpleMeshLoader::createConnectivities(SimpleMesh* mesh)
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


	Vec3f calculateNormal(Vec3f v0, Vec3f v1, Vec3f v2)
	{
		Vec3f edge0 = v1 - v0;
		Vec3f edge1 = v2 - v0;
		Vec3f n;
		Vec3f::cross3(n, edge0, edge1);
		n.normalize();
		return n;
	}

	void OBJSimpleMeshLoader::calculateFaceNormals(SimpleMesh* mesh)
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

	void OBJSimpleMeshLoader::computeVertexNormals(SimpleMesh* mesh)
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



	void OBJSimpleMeshLoader::meshToUnitCube(SimpleMesh* mesh)
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

	void OBJSimpleMeshLoader::trimString( std::string& _string) {
		// Trim Both leading and trailing spaces

		size_t start = _string.find_first_not_of(" \t\r\n");
		size_t end   = _string.find_last_not_of(" \t\r\n");

		if(( std::string::npos == start ) || ( std::string::npos == end))
			_string = "";
		else
			_string = _string.substr( start, end-start+1 );
	}

}

