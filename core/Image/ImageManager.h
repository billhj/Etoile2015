/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file ImageManager.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "ImageLoader.h"
#include "Image.h"

namespace Etoile
{
	class ImageManager
	{
	private:
		ImageManager():_pCurrentLoader(NULL){}
		ImageLoader* _pCurrentLoader;
	public:
		static ImageManager* getInstance()
		{
			static ImageManager manager;
			return &manager;
		}
		void setImageLoader(ImageLoader* loader)
		{
			_pCurrentLoader = loader;
		}
		ImageLoader* getCurrentImageLoader()
		{
			return _pCurrentLoader;
		}
	};
}

