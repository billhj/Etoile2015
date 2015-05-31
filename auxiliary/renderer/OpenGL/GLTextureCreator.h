/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file GLTextureCreator.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "renderer/OpenGL/GLTexture2D.h"
#include "geometry/TextureManager.h"
#include "Image/ImageManager.h"

namespace Etoile
{
	class GLTextureCreator : public TextureCreator
	{
	protected:
		Texture* empty;
	public:
		GLTextureCreator()
		{
			float emptyMap[16] = {1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1};
			float checkboard[64] = {0,0,0,0, 1,1,1,1, 0,0,0,0, 1,1,1,1, 
			1,1,1,1,0,0,0,0, 1,1,1,1,0,0,0,0,  
			0,0,0,0, 1,1,1,1, 0,0,0,0, 1,1,1,1, 
			1,1,1,1,0,0,0,0, 1,1,1,1,0,0,0,0};
			GLTexture2D* t = new GLTexture2D();
			t->setFilePath("emptyMap");
			t->create(2, 2, 1 , GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT , &emptyMap[0], false);
			GLTexture2D* t2 = new GLTexture2D();
			t2->setFilePath("checkBoardMap");
			t2->create(4, 4, 1 , GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT , &checkboard[0], false);
			empty = t2;
		}

		~GLTextureCreator(){}
		virtual Texture* createFromFile(const std::string& fileName) override
		{
			Image image;
			ImageLoader* loader = ImageManager::getInstance()->getCurrentImageLoader();
			bool b_image = loader->loadImageFromFile(fileName, image);
			if(b_image != true)
			{
				std::cout<<"can not load texture : "<<fileName<<std::endl;
				//assert(!qimage.isNull());
				return empty;
			}
			else
			{
				GLTexture2D* t = new GLTexture2D();
				t->setFilePath(fileName);
				t->create(image.getWidth(), image.getHeight(),1 , GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT ,(float*)&image.getData()[0], false);
				return t;
			}
		}
	
	};

}
