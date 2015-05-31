/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file OBJSimpleMeshLoader.cpp
* @brief 
* @date 1/2/2011
*/

#include "OBJSimpleMeshLoader.h"
#include <iostream>
#include <exception>
#include "Image/ImageManager.h"
#include "geometry/TextureManager.h"
#include "geometry/Texture.h"

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
		int groupIndex = -1;

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
				if(mesh->m_groups.size() > 0 && mesh->m_groups[groupIndex].m_materialIndex == -1)
				{
					mesh->m_groups[groupIndex].m_materialIndex = materialIndex;
				}else
				{
					groupIndex = mesh->m_groups.size();
					mesh->m_groups.push_back(SimpleMesh::Group());
					mesh->m_groups[groupIndex].m_groupIndex = groupIndex;
					mesh->m_groups[groupIndex].m_groupName = matname;
					mesh->m_groups[groupIndex].m_materialIndex = materialIndex;
				}
			}
			//PB is the files for materials, using usemtl or not, the obj file is not uniform
			else if ( keyWrd == "g")
			{
				std::string objectName;
				stream >> objectName;
				materialIndex = -1;
				if(!stream.fail())
				{
					groupIndex = mesh->m_groups.size();
					mesh->m_groups.push_back(SimpleMesh::Group());
					mesh->m_groups[groupIndex].m_groupIndex = groupIndex;
					mesh->m_groups[groupIndex].m_groupName = objectName;
					mesh->m_groups[groupIndex].m_materialIndex = -1;
				}
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
				
				//mesh->m_facecount++;
				// read full line after detecting a face
				std::string faceLine;
				std::getline(stream,faceLine);
				SimpleMesh::Face face = handle_face(faceLine);
				face.m_materialIndex = materialIndex;
				face.m_groupIndex = groupIndex;
				mesh->m_groups[groupIndex].m_faceIndices.push_back(mesh->m_faces.size());
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
			"Face count : " <<mesh->m_faces.size()<<std::endl;

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
		buildVertexIndices(mesh);
		SimpleMesh::fillMaterial(mesh);
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
		int currentMatIndex = 0;
		mesh->m_materials[0].m_name = "empty";

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
				currentMatIndex = mesh->m_materials.size();
				m_materialNameMap[key] = currentMatIndex;
				mesh->m_materials.push_back(SimpleMesh::Material());
				mesh->m_materials[currentMatIndex].m_name = key;
			}

			else if (keyWrd == "Kd") // diffuse color
			{
				stream >> f1; stream >> f2; stream >> f3;

				if( !stream.fail() )
				{
					mesh->m_materials[currentMatIndex].m_diffuse = ColorRGBA(f1, f2, f3, 1);
				}
			}

			else if (keyWrd == "Ka") // ambient color
			{
				stream >> f1; stream >> f2; stream >> f3;

				if( !stream.fail() )
				{
					mesh->m_materials[currentMatIndex].m_ambient = ColorRGBA(f1, f2, f3, 1);
				}
			}

			else if (keyWrd == "Ks") // specular color
			{
				stream >> f1; stream >> f2; stream >> f3;

				if( !stream.fail() )
				{
					mesh->m_materials[currentMatIndex].m_specular = ColorRGBA(f1, f2, f3, 1);
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
					mesh->m_materials[currentMatIndex].m_shininess = f1;
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
					SimpleMesh::Material& mat = mesh->m_materials[currentMatIndex];
					mat.m_maps[DIFFUSE_MAP] = s;
					mat.m_binding[DIFFUSE_MAP] = "DiffuseMap";
					try
					{
						mat.m_images[DIFFUSE_MAP] = Image();
						ImageManager::getInstance()->getCurrentImageLoader()->loadImageFromFile(s, mat.m_images[DIFFUSE_MAP]);
						Texture* t = TextureManager::getInstance()->getCurrentTextureCreator()->createFromFile(s);
						mat.m_indicesInRessouce[DIFFUSE_MAP] = t->getTexturePoolIndex();
					}catch(exception& e)
					{
						std::cerr<<"TextureCreator not defined"<<std::endl;
					}
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
					SimpleMesh::Material& mat = mesh->m_materials[currentMatIndex];
					mat.m_maps[SPECULAR_MAP] = s;
					mat.m_binding[SPECULAR_MAP] = "SpecularMap";
					try{
						mat.m_images[SPECULAR_MAP] = Image();
						ImageManager::getInstance()->getCurrentImageLoader()->loadImageFromFile(s, mat.m_images[SPECULAR_MAP]);
						Texture* t = TextureManager::getInstance()->getCurrentTextureCreator()->createFromFile(s);
						mat.m_indicesInRessouce[SPECULAR_MAP] = t->getTexturePoolIndex();
					}catch(exception& e)
					{
						std::cerr<<"TextureCreator not defined"<<std::endl;
					}
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
					SimpleMesh::Material& mat = mesh->m_materials[currentMatIndex];
					mat.m_maps[BUMP_MAP] = s;
					mat.m_binding[BUMP_MAP] = "BumpMap";
					try{
						mat.m_images[BUMP_MAP] = Image();
						ImageManager::getInstance()->getCurrentImageLoader()->loadImageFromFile(s, mat.m_images[BUMP_MAP]);
						Texture* t = TextureManager::getInstance()->getCurrentTextureCreator()->createFromFile(s);
						mat.m_indicesInRessouce[BUMP_MAP] = t->getTexturePoolIndex();
					}catch(exception& e)
					{
						std::cerr<<"TextureCreator not defined"<<std::endl;
					}
				}
			}
			else if (keyWrd == "Tr") // transparency value
			{
				stream >> f1;
				if( !stream.fail() )
				{
					mesh->m_materials[currentMatIndex].m_transparency = f1;
				}
			}
			else if (keyWrd == "d") // transparency value
			{
				stream >> f1;
				if( !stream.fail() )
				{
					mesh->m_materials[currentMatIndex].m_transparency = f1;
				}
			}
		}
		return mesh->m_materials.size();
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
		SimpleMesh::regroupVertexAttributes(mesh);
	}

	void OBJSimpleMeshLoader::triangulate(SimpleMesh* mesh)
	{
		SimpleMesh::triangulate(mesh);
	}

	void OBJSimpleMeshLoader::createConnectivities(SimpleMesh* mesh)
	{
		SimpleMesh::createConnectivities(mesh);
	}


	void OBJSimpleMeshLoader::calculateFaceNormals(SimpleMesh* mesh)
	{
		SimpleMesh::calculateFaceNormals(mesh);
	}

	void OBJSimpleMeshLoader::computeVertexNormals(SimpleMesh* mesh)
	{
		SimpleMesh::computeVertexNormals(mesh);
	}

	void OBJSimpleMeshLoader::meshToUnitCube(SimpleMesh* mesh)
	{
		SimpleMesh::meshToUnitCube(mesh);
	}

	void OBJSimpleMeshLoader::buildVertexIndices(SimpleMesh* mesh)
	{
		SimpleMesh::buildVertexIndices(mesh);
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

