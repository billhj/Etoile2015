/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file OpenMeshLoader.cpp
* @brief 
* @date 1/2/2011
*/
#include "OpenMeshLoader.h"
#include "util/File.h"
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

/*
*
*
*/

namespace Etoile
{

	OpenMeshLoader::OpenMeshLoader():MeshLoader(),_smoothThreshold(0.6f)
	{

	}

	OpenMeshLoader::~OpenMeshLoader()
	{

	}


	bool OpenMeshLoader::loadFromFile(const std::string& fileName,RenderMesh* mesh)
	{
		// request vertex normals, so the mesh reader can use normal information
		// if available
		_fileName = fileName;
		_pMesh = mesh;
		_omesh = new OMesh();
		_omesh->request_vertex_normals();
		_omesh->request_vertex_texcoords2D();
		_omesh->request_face_texture_index();
		_omesh->request_face_colors();
		//this->request_vertex_colors();
		// assure we have vertex normals
		if (!_omesh->has_vertex_normals())
		{
			std::cerr << "ERROR: Standard vertex property 'Normals' not available!\n";
			return true;
		}

		if (!OpenMesh::IO::read_mesh(*_omesh, fileName)) {
			std::cout << "Couldn't load mesh: " << fileName << std::endl;
			return false;
		}

		if (!_omesh->has_vertex_normals())
		{
			std::cerr << "ERROR: Standard vertex property 'Normals' not available!\n";
		}
		else
		{
			std::cout << "Having vnormals" << std::endl;
		}


		if (!_omesh->has_face_colors())
		{
			std::cerr << "ERROR: Standard vertex property 'FACE_Colors' not available!\n";
		}
		else
		{
			std::cout << "Having FACE COLORS" << std::endl;
		}

		OpenMesh::IO::Options opt;
		// If the file did not provide vertex normals, then calculate them
		if (!opt.check(OpenMesh::IO::Options::VertexNormal))
		{
			std::cout << "Calculating vnormals" << std::endl;
			// we need face normals to update the vertex normals
			_omesh->request_face_normals();

			// let the mesh update the normals
			_omesh->update_normals();

			// dispose the face normals, as we don't need them anymore
			//  this->release_face_normals();
		}
		// mesh.request_halfedge_texcoords2D ();
		//mesh.request_face_texture_index ();

		this->loadTextures(fileName);
		createData(_omesh);
		
		return true;
	}



	// create Vertex buffer object(vertices, normal, color, texCord)
	void OpenMeshLoader::createData(OMesh* omesh)
	{
		
		meshToUnitCube(*omesh);

		std::vector<VBORenderSubMesh*> submeshes;

		for( unsigned int i = 0; i < _materials.size() + 1; ++i){
			if(i==0)
			{
				VBORenderSubMesh* submesh = new VBORenderSubMesh("empty");
				_pMesh->addRenderSubMesh(submesh);
				submeshes.push_back(submesh);
			}
			else
			{
				VBORenderSubMesh* submesh = new VBORenderSubMesh();
				_pMesh->addRenderSubMesh(submesh);
				submesh->setMaterial(_materials[i - 1]);
				submeshes.push_back(submesh);
			}
		}
		std::cout<<"submesh created" <<std::endl;



		size_t sNumFaces = omesh->n_faces();

		OMesh::FaceIter f = omesh->faces_begin() , f_end = omesh->faces_end();
		for ( ; f != f_end; ++f )
		{
			OMesh::FaceHandle hFace = f.handle();
			int iTexIndex = omesh->texture_index( hFace );
			VBORenderSubMesh* submesh = submeshes[iTexIndex];

			for( OMesh::FaceVertexIter fv_it=_omesh->fv_iter(f.handle()); fv_it; ++fv_it)
			{

				OMesh::VertexHandle vh = fv_it.handle() ;

				OMesh::Normal smoothNormal(0,0,0);// _omesh->normal(f.handle());
				const OMesh::Normal faceNormal = omesh->normal(f.handle());

				for(OMesh::VertexFaceIter vf_it = omesh->vf_iter(vh); vf_it; ++vf_it)
				{
					const OMesh::Normal &nn = omesh->normal(vf_it.handle());
					if( OpenMesh::dot(faceNormal,nn)  > _smoothThreshold)
					{
						smoothNormal += nn;
					}
				}

				const OMesh::Point & pp = omesh->point(vh);
				const OMesh::Normal & nn = smoothNormal/smoothNormal.norm();
				const OMesh::TexCoord2D & tt = omesh->texcoord2D(vh);

				submesh->m_ndata.push_back(Vec3f(nn.data()[0], nn.data()[1], nn.data()[2]));

				submesh->m_tdata.push_back(Vec3f(tt.data()[0], tt.data()[1], 0));

				submesh->m_vdata.push_back(Vec3f(pp.data()[0], pp.data()[1], pp.data()[2]));

				submesh->m_vertices_index_face.push_back(submesh->m_vdata.size() - 1);

			}

		}

		std::cout<<"debug:"<<"nb faces:"<<sNumFaces<<std::endl;

		_pMesh->cleanEmptyRenderSubMesh();
		//_pMesh->buildRuntimeData();
		_pMesh->computeAABB();
	}

	void OpenMeshLoader::meshToUnitCube(OMesh& mesh)
	{
		OMesh::Point minP(1e6, 1e6, 1e6);
		OMesh::Point maxP(-1e6, -1e6, -1e6);

		for (OMesh::ConstVertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it)
		{
			OMesh::Point p = mesh.point(v_it.handle());
			for (int i = 0; i < 3; ++i)
			{
				if (p[i] > maxP[i]) maxP[i] = p[i];
				if (p[i] < minP[i]) minP[i] = p[i];
			}
		}

		// find the longest edge and scale it to 1
		double scale = 1.0f / max(max(maxP[0] - minP[0], maxP[1] - minP[1]), maxP[2] - minP[2]);

		OMesh::Point center((maxP[0] + minP[0]) / 2.0f,
			(maxP[1] + minP[1]) / 2.0f,
			(maxP[2] + minP[2]) / 2.0f);

		for (OMesh::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it)
		{
			mesh.point(v_it.handle()) -= center;
			mesh.point(v_it.handle()) *= scale;
		}
	}


	// textureloading

	void OpenMeshLoader::loadTextures(const std::string& fileName)
	{

		std::string directory = File::getFilePath(fileName);
		OpenMesh::MPropHandleT< std::map< int, std::string > > TextureMaps;
		int index_texture = 0;
		if ( _omesh->get_property_handle( TextureMaps, "TextureMapping" ) )
		{
			std::map< int, std::string >& refMap = _omesh->property( TextureMaps );

			std::map< int, std::string >::iterator itorMaps;
			for ( itorMaps = refMap.begin(); itorMaps != refMap.end(); ++itorMaps )
			{
				std::string strTexMapFilename = (*itorMaps).second;

#if defined(_DEBUG) || defined(DEBUG)
				std::cout<<"debug:"<<(*itorMaps).first << strTexMapFilename<<std::endl;
				std::cout<<directory+strTexMapFilename<<std::endl;
#endif

				/*std::stringstream index_texture_ss;
				index_texture_ss << index_texture;*/

				std::string path = (directory+strTexMapFilename);

				std::string currentMat = strTexMapFilename;
				_materials.push_back(Material());
				Material& mat = _materials.back();
				//_materialNameMap[currentMat] = _materials.size() - 1;
				/*GLTexture2D* t = new GLTexture2D();
				t->setFilePath(path);*/
				if(NULL != TextureManager::getInstance()->getCurrentTextureCreator())
					mat.setDiffuseTexture(TextureManager::getInstance()->getCurrentTextureCreator()->createFromFile(path));
				index_texture++;
			}

		}
		//_texturePathMap["emptyMap"] = "emptyMap";

	}

}
