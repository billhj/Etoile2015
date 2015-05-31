/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file GeneralImageLoader.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "Image/Image.h"
#include "Image/ImageLoader.h"
#include "Image/PFMImageLoader.h"
#include "QTImageLoader.h"
#include "Image/TGAImageLoader.h"
#include "util/File.h"

namespace Etoile
{
    class GeneralImageLoader : public ImageLoader
	{
	public:
		bool loadImageFromFile(const std::string& filepath, Image& image)
		{
			std::string strname = File::getFileExtension(filepath, true);
			bool b_image = false;
			if(strname.compare("pfm")==0)
			{
				PFMImageLoader loader;
				b_image = loader.loadImageFromFile(filepath, image);

			}else if(strname.compare("tga")==0)
			{
				TGAImageLoader loader;
				b_image = loader.loadImageFromFile(filepath, image);
			}
			else
			{
				QTImageLoader loader;
				b_image = loader.loadImageFromFile(filepath, image);
			}
			return b_image;
		}

	};

}
