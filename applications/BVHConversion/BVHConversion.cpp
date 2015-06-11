// BVHConversion.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "animation/BVH.h"
#include "util/File.h"
#include <string>
#include <iostream>

#ifndef UNICODE  
  typedef std::string String; 
#else
  typedef std::wstring String; 
#endif
int _tmain(int argc, _TCHAR* argv[])
{
	Etoile::BVH bvh;
	for (int i = 1; i<argc; ++i)
	{
		std::wstring para = (argv[i]);
		std::string s( para.begin(), para.end() );
		std::cout<<s<<std::endl;
		std::string path = Etoile::File::getFilePath(s);
		std::string filename = Etoile::File::getFileNameWithoutExtension(s);
		bvh.loadFromBVHFile(s);
		bvh.changeOrderToZYX();
		bvh.saveToBVHFile(filename +"_zyx.bvh");
	}


	return 0;
}

