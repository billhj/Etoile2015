// BVHConversion.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "animation/BVH.h"
#include "util/File.h"
#include "util/FileSearch.h"
#include <string>
#include <iostream>
#include "OctreeDataCreation.h"

#ifndef UNICODE  
  typedef std::string String; 
#else
  typedef std::wstring String; 
#endif

  void dealBVH(const std::string& s)
  {
	Etoile::BVH bvh;
	std::string path = Etoile::File::getFilePath(s);
	std::string filename = Etoile::File::getFileNameWithoutExtension(s);
	bvh.loadFromBVHFile(s);
	bvh.changeOrderToZYX();
	bvh.saveToBVHFile(filename +"_zyx.bvh");
  }

  void createBVHData(const std::string& s)
  {
	Etoile::BVH bvh;
	bvh.loadFromBVHFile(s);
	std::string path = Etoile::File::getFilePath(s);
	std::string filename = Etoile::File::getFileNameWithoutExtension(s);
	createBVHDataFile(bvh, filename+".data");
	createRelativeBodyBVHDataFile(bvh, filename+".data2");
  }

int _tmain(int argc, _TCHAR* argv[])
{
	for (int i = 1; i<argc; ++i)
	{
		std::wstring para = (argv[i]);
		std::string s( para.begin(), para.end() );
		std::cout<<s<<std::endl;
		std::string fileext = Etoile::File::getFileExtension(s);
		if(fileext=="")
		{
			std::vector<Etoile::FileInfo> fileinfos = Etoile::FileSearch::search(s, "bvh");
			for(unsigned int i = 0; i < fileinfos.size(); ++i)
			{
				//dealBVH(fileinfos[i]._path);
				createBVHData(fileinfos[i]._path);
			}
		}else
		{
			dealBVH(s);
		}
	}


	return 0;
}

