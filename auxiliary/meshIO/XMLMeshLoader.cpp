/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file XMLMeshLoader.cpp
* @brief 
* @date 1/2/2011
*/

#include "XMLMeshLoader.h"

/**
* @brief For tracking memory leaks under windows using the crtdbg
*/
#if ( defined( _DEBUG ) || defined( DEBUG ) ) && defined( _MSC_VER )
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif

namespace Etoile{
	XMLMeshLoader::XMLMeshLoader(): MeshLoader(),_pDoc(0)
	{

	}

	XMLMeshLoader::~XMLMeshLoader()
	{
		delete _pDoc;
	}


	bool XMLMeshLoader::loadFromFile(const std::string& fileName,RenderMesh* mesh)
	{
		_fileName = fileName;
		_pMesh = mesh;
		_pDoc = new TiXmlDocument(_fileName.c_str());


		if(!_pDoc->LoadFile()){
			std::cout << "erreur while loading" << std::endl;
			std::cout << "error #" << _pDoc->ErrorId() << " : " << _pDoc->ErrorDesc() << std::endl;
			return false;
		}

		bool b = loadMeshes();
		//_pMesh->createVBO(usage, eInternalType);
		//meshToUnitCube();
		//creatGpuData();
		_pMesh->computeAABB();
		return b;
	}

	void XMLMeshLoader::readVBORenderSubMesh(TiXmlElement *elemVBORenderSubMesh)
	{

		std::string material = elemVBORenderSubMesh->Attribute("material");
		//std::string usesharedvertices = elemVBORenderSubMesh->Attribute("usesharedvertices");
		//std::string use32bitindexes = elemVBORenderSubMesh->Attribute("use32bitindexes");
		//std::string operationtype = elemVBORenderSubMesh->Attribute("operationtype");
		//Material* m = new Material(material);
		VBORenderSubMesh* submesh = new VBORenderSubMesh(material);
		_pMesh->addRenderSubMesh(submesh);
		//_pMesh->_materialGroup.push_back(m);
#if defined(_DEBUG) || defined(DEBUG)
		std::cout<<"read faces start"<<std::endl;
#endif

		TiXmlElement *elemFaces = elemVBORenderSubMesh->FirstChildElement("faces");
		while (elemFaces)
		{			
			readFaces(elemFaces, submesh);
			elemFaces = elemFaces->NextSiblingElement("faces"); // iteration
		}
#if defined(_DEBUG) || defined(DEBUG)
		std::cout<<"read faces end"<<std::endl;
#endif

		TiXmlElement *elemGeometry = elemVBORenderSubMesh->FirstChildElement("geometry");
		while (elemGeometry)
		{			
			readGeometry(elemGeometry, submesh);
			elemGeometry = elemGeometry->NextSiblingElement("geometry"); // iteration
		}	
#if defined(_DEBUG) || defined(DEBUG)
		std::cout<<"read Geometry end"<<std::endl;
#endif

		TiXmlElement *elemBoneAssignment = elemVBORenderSubMesh->FirstChildElement("boneassignments");
		while (elemBoneAssignment)
		{			
			readBoneAssignment(elemBoneAssignment, submesh);
			elemBoneAssignment = elemBoneAssignment->NextSiblingElement("boneassignments"); // iteration
		}	

		//meshToUnitCube(submesh);

	}

	void XMLMeshLoader::readFaces(TiXmlElement *elemFaces, VBORenderSubMesh* submesh)
	{
		int count = 0;
		int returnvalue = elemFaces->QueryIntAttribute("count", &count);
		if( !returnvalue == TIXML_SUCCESS)
			return;
		submesh->m_numberOfFaces = count;
		int size = count * 3;
		submesh->m_vertices_index_face.resize(size);

		TiXmlElement *elemFace = elemFaces->FirstChildElement("face");
		int n = 0;
		while (elemFace)
		{
			int v1 = 0, v2 = 0, v3 = 0;
			elemFace->QueryIntAttribute("v1", &v1);
			elemFace->QueryIntAttribute("v2", &v2);
			elemFace->QueryIntAttribute("v3", &v3);
			submesh->m_vertices_index_face[n * 3] = v1;
			submesh->m_vertices_index_face[n * 3 + 1] = v2;
			submesh->m_vertices_index_face[n * 3 + 2] = v3;
			n++;
			elemFace = elemFace->NextSiblingElement("face"); // iteration
		}

	}


	void XMLMeshLoader::readGeometry(TiXmlElement *elemGeometry, VBORenderSubMesh* submesh)
	{

		int vertexcount = 0;
		int returnvalue = elemGeometry->QueryIntAttribute("vertexcount", &vertexcount);
		if(returnvalue != TIXML_SUCCESS)
			return;
		submesh->m_vdata.resize(vertexcount); 
		submesh->m_ndata.resize(vertexcount);
		submesh->m_tdata.resize(vertexcount); 
		TiXmlElement *elemvertexbuffer = elemGeometry->FirstChildElement("vertexbuffer");
		while (elemvertexbuffer)
		{
			const char * positionsC = elemvertexbuffer->Attribute("positions");
			const char * normalsC = elemvertexbuffer->Attribute("normals");
			std::string positions, normals;
			if(positionsC != NULL)
				positions = positionsC;
			if(normalsC != NULL)
				normals = normalsC;

			int texture_coord_dimensions_0 = 0;
			int returnvalue = elemvertexbuffer->QueryIntAttribute("texture_coord_dimensions_0", &texture_coord_dimensions_0);
			int texture_coords = 0;
			int returnvalue1 = elemvertexbuffer->QueryIntAttribute("texture_coords", &texture_coords);

			if(positions=="true")
			{
#if defined(_DEBUG) || defined(DEBUG)
				std::cout<<"read positions start"<<std::endl;
#endif

				TiXmlElement *elemvertex = elemvertexbuffer->FirstChildElement("vertex");
				int n = 0;
				while (elemvertex)
				{
					TiXmlElement *elemposition = elemvertex->FirstChildElement("position");

					while (elemposition)
					{
						double x = 0, y = 0, z = 0;
						elemposition->QueryDoubleAttribute("x", &x);
						elemposition->QueryDoubleAttribute("y", &y);
						elemposition->QueryDoubleAttribute("z", &z);

						Vec3f xyz(x*_scale.x(),y*_scale.y(),z*_scale.z());
						submesh->m_vdata[n] = _rotate * xyz + _translate;
						n++;
						elemposition = elemposition->NextSiblingElement("position"); // iteration
					}

					elemvertex = elemvertex->NextSiblingElement("vertex"); // iteration
				}


#if defined(_DEBUG) || defined(DEBUG)
				std::cout<<"read positions end"<<std::endl;
#endif


			}
			if(normals=="true")
			{

#if defined(_DEBUG) || defined(DEBUG)
				std::cout<<"read normals start"<<std::endl;
#endif


				TiXmlElement *elemvertex = elemvertexbuffer->FirstChildElement("vertex");
				int n = 0;
				while (elemvertex)
				{
					TiXmlElement *elemnormal = elemvertex->FirstChildElement("normal");

					while (elemnormal)
					{
						double x = 0, y = 0, z = 0;
						elemnormal->QueryDoubleAttribute("x", &x);
						elemnormal->QueryDoubleAttribute("y", &y);
						elemnormal->QueryDoubleAttribute("z", &z);
						Vec3f xyz(x,y,z);
						submesh->m_ndata[n] = xyz;
						n++;
						elemnormal = elemnormal->NextSiblingElement("normal"); // iteration
					}
					elemvertex = elemvertex->NextSiblingElement("vertex"); // iteration
				}



#if defined(_DEBUG) || defined(DEBUG)
				std::cout<<"read normals end"<<std::endl;
#endif


			}
			if(returnvalue==TIXML_SUCCESS && returnvalue1==TIXML_SUCCESS)
			{

#if defined(_DEBUG) || defined(DEBUG)
				std::cout<<"read textureCoord start"<<std::endl;
#endif

				TiXmlElement *elemvertex = elemvertexbuffer->FirstChildElement("vertex");
				int n = 0;
				while (elemvertex)
				{
					TiXmlElement *elemtexcoord = elemvertex->FirstChildElement("texcoord");

					while (elemtexcoord)
					{
						double u = 0, v = 0;

						elemtexcoord->QueryDoubleAttribute("u", &u);
						elemtexcoord->QueryDoubleAttribute("v", &v);
						Vec3f uv(u,v,0);
						submesh->m_tdata[n] = uv;
						n++;

						elemtexcoord = elemtexcoord->NextSiblingElement("texcoord"); // iteration
					}
					elemvertex = elemvertex->NextSiblingElement("vertex"); // iteration
				}


#if defined(_DEBUG) || defined(DEBUG)
				std::cout<<"read textureCoord end"<<std::endl;
#endif


			}


			elemvertexbuffer = elemvertexbuffer->NextSiblingElement("vertexbuffer"); // iteration
		}

	}


	void XMLMeshLoader::readBoneAssignment(TiXmlElement *elemBoneAssignment, VBORenderSubMesh* submesh)
	{

		//TiXmlElement *vertexboneassignment = elemBoneAssignment->FirstChildElement("vertexboneassignment");

		//if(vertexboneassignment)
		//	submesh->getSkin()._vertexBoneAssignmentData.resize(submesh->m_vdata.size());

		//while (vertexboneassignment)
		//{			
		//	int vertexIndex = -1;
		//	vertexboneassignment->QueryIntAttribute("vertexindex", &vertexIndex);
		//	int boneIndex = -1;
		//	vertexboneassignment->QueryIntAttribute("boneindex", &boneIndex);
		//	double weight = 0;
		//	vertexboneassignment->QueryDoubleAttribute("weight", &weight);

		//	if(vertexIndex < 0) continue;
		//	submesh->getSkin()._vertexBoneAssignmentData[vertexIndex].setIdx(vertexIndex);
		//	submesh->getSkin()._vertexBoneAssignmentData[vertexIndex].addBoneWeight(boneIndex, weight);
		//	submesh->getSkin()._vertexBoneAssignmentData[vertexIndex].setPosition(submesh->m_vdata[vertexIndex]);

		//	vertexboneassignment = vertexboneassignment->NextSiblingElement("vertexboneassignment"); // iteration
		//}
	}


	void XMLMeshLoader::readPoses(TiXmlElement *elemPoses)
	{
		TiXmlElement *elemPose = elemPoses->FirstChildElement("pose");
		while (elemPose)
		{
			std::string target = elemPose->Attribute("target");
			std::string name = elemPose->Attribute("name");
			//int index = 0;
			//int returnvalue = elemPose->QueryIntAttribute("index", &index);
			//AnimatedMesh* animatedMesh = dynamic_cast<AnimatedMesh*>(_pMesh);
			//Pose* pose = new Pose(index, name,target);
			this->checkSpecifiedPose(name);
			//		animatedMesh->_poses.push_back(pose);
			TiXmlElement *elemPoseoffset = elemPose->FirstChildElement("poseoffset");
			while (elemPoseoffset)
			{
				int index;
				double x, y, z;
				elemPoseoffset->QueryIntAttribute("index", &index);
				elemPoseoffset->QueryDoubleAttribute("x", &x);
				elemPoseoffset->QueryDoubleAttribute("y", &y);
				elemPoseoffset->QueryDoubleAttribute("z", &z);
				//			pose->addOffset(index, Vec3f(x,y,z));
				elemPoseoffset = elemPoseoffset->NextSiblingElement("poseoffset"); // iteration
			}

			elemPose = elemPose->NextSiblingElement("pose"); // iteration
		}

	}

	void XMLMeshLoader::readAnimations(TiXmlElement *elemAnimations)
	{
		TiXmlElement *elemAnimation = elemAnimations->FirstChildElement("animation");
		while (elemAnimation)
		{

			TiXmlElement *elemTracks = elemAnimation->FirstChildElement("tracks");
			while (elemTracks)
			{
				readTracks(elemTracks);
				elemTracks = elemTracks->NextSiblingElement("tracks"); // iteration
			}

			elemAnimation = elemAnimation->NextSiblingElement("animation"); // iteration
		}
	}

	void XMLMeshLoader::readTracks(TiXmlElement *elemTracks)
	{
		TiXmlElement *elemTrack = elemTracks->FirstChildElement("track");
		while (elemTrack)
		{
			std::string target = elemTrack->Attribute("target");
			std::string type = elemTrack->Attribute("type");
			int index = 0 ;
			elemTrack->QueryIntAttribute("index", &index);


			TiXmlElement *elemKeyframes = elemTrack->FirstChildElement("keyframes");
			while (elemKeyframes)
			{
				elemKeyframes = elemKeyframes->NextSiblingElement("keyframes"); // iteration
			}

			elemTrack = elemTrack->NextSiblingElement("track"); // iteration
		}

	}


	void XMLMeshLoader::meshToUnitCube(VBORenderSubMesh* submesh)
	{
		Vec3f minP(1e6, 1e6, 1e6);
		Vec3f maxP(-1e6, -1e6, -1e6);

		for (unsigned int j = 0; j < submesh->m_vdata.size(); j++)
		{
			Vec3f p = submesh->m_vdata[j];
			for (int i = 0; i < 3; ++i)
			{
				if (p[i] > maxP[i]) maxP[i] = p[i];
				if (p[i] < minP[i]) minP[i] = p[i];
			}
		}

		// find the longest edge and scale it to 1
		double scale = 1.0f / max(max(maxP[0] - minP[0], maxP[1] - minP[1]), maxP[2] - minP[2]);

		Vec3f center((maxP[0] + minP[0]) / 2.0f,
			(maxP[1] + minP[1]) / 2.0f,
			(maxP[2] + minP[2]) / 2.0f);

		for (unsigned int j = 0; j < submesh->m_vdata.size(); j++)
		{
			submesh->m_vdata[j] -= center;
			submesh->m_vdata[j] *= scale;
		}
	}


	bool XMLMeshLoader::loadMeshes()
	{

		TiXmlHandle hdl(_pDoc);
		TiXmlElement *elemRoot = hdl.FirstChildElement("mesh").Element();
		while (elemRoot)
		{

			TiXmlElement *elemSubmeshes = elemRoot->FirstChildElement("submeshes");
			while (elemSubmeshes)
			{
				TiXmlElement *elemVBORenderSubMesh = elemSubmeshes->FirstChildElement("submesh");

				while(elemVBORenderSubMesh){
					readVBORenderSubMesh(elemVBORenderSubMesh);
					elemVBORenderSubMesh = elemVBORenderSubMesh->NextSiblingElement("submesh");
				}

				elemSubmeshes = elemSubmeshes->NextSiblingElement("submeshes"); // iteration
			}


			elemRoot = elemRoot->NextSiblingElement("mesh"); // iteration
		}

		return true;
	}

	bool XMLMeshLoader::loadPoses()
	{

		TiXmlHandle hdl(_pDoc);
		TiXmlElement *elemRoot = hdl.FirstChildElement("mesh").Element();

#if defined(_DEBUG) || defined(DEBUG)
		std::cout<<"read poses start"<<std::endl;
#endif

		while (elemRoot)
		{
			TiXmlElement *elemPoses = elemRoot->FirstChildElement("poses");
			while (elemPoses)
			{
				readPoses(elemPoses);
				elemPoses = elemPoses->NextSiblingElement("poses"); // iteration
			}


			TiXmlElement *elemAnimations = elemRoot->FirstChildElement("animations");
			while (elemAnimations)
			{
				readAnimations(elemAnimations);
				elemAnimations = elemAnimations->NextSiblingElement("animations"); // iteration
			}


			elemRoot = elemRoot->NextSiblingElement("mesh"); // iteration
		}

#if defined(_DEBUG) || defined(DEBUG)
		std::cout<<"read poses end"<<std::endl;
#endif
		return true;

	}


	void XMLMeshLoader::checkSpecifiedPose(const std::string& nameOfPose)
	{
		if (nameOfPose=="FAP_6")
			;
		else if(nameOfPose=="FAP_7")
			;
		else if(nameOfPose=="FAP_12_0")
			;
		else if(nameOfPose=="FAP_12_1")
			;
		else if(nameOfPose=="FAP_13_0")
			;
		else if(nameOfPose=="FAP_13_1")
			;
		else if(nameOfPose=="FAP_17_")
			;
		else if(nameOfPose=="FAP_31_")
			;
		else if(nameOfPose=="FAP_32_")
			;
		else if(nameOfPose=="FAP_33_")
			;
		else if(nameOfPose=="FAP_34_")
			;
		else if(nameOfPose=="FAP_35_")
			;
		else if(nameOfPose=="FAP_36_")
			;
		else if(nameOfPose=="FAP_39_")
			;
		else if(nameOfPose=="FAP_40_")
			;
		else if(nameOfPose=="FAP_41_")
			;
		else if(nameOfPose=="FAP_42_")
			;
		else if(nameOfPose=="FAP_55_")
			;
		else if(nameOfPose=="FAP_56_")
			;
		else if(nameOfPose=="FAP_59_")
			;
		else if(nameOfPose=="FAP_60_")
			;
		//other FAPs
		else{
			//		AnimatedMesh* animatedMesh = dynamic_cast<AnimatedMesh*>(_pMesh);
			//		animatedMesh->_nameOfPosesNoSpecified.push_back(nameOfPose);
		}
	}
}