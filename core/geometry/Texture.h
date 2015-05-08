/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Texture.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <vector>
#include <map>
#include "TextureManager.h"

namespace Etoile
{
	class Texture
	{
	public:
		Texture(const std::string& name): m_name(name), m_width(0), m_height(0), m_layer(0)
		{
			TextureManager::getInstance()->add(name, this);
		}

		Texture(Texture& t)
		{
			m_name = t.m_name;
			m_width = t.m_width; m_height = t.m_height;
			m_layer = t.m_layer;
			m_mipmaped = t.m_mipmaped;
		}

		void setFilePath(const std::string& filepath)
		{
			m_filepath = filepath;
		}
		const std::string& filepath()
		{
			return m_filepath;
		}

		virtual ~Texture(){}
		virtual void use() const = 0;
		virtual void unUse() const = 0;
		virtual void release() = 0;
		int getWidth() const
		{
			return m_width;
		}
		int getHeight() const
		{
			return m_height;
		}

		int getLayerNumbers() const
		{
			return m_layer;
		}

		bool usingMipmap() const{ return m_mipmaped;}
		virtual void generateMipmap()  const = 0;
		virtual void read(float* data) = 0;
		virtual void write(int x, int y, int width, int height, float* data) = 0;
		virtual void write(int size, float* data) = 0;
		const std::string getName(){return m_name;}
		void setName(const std::string& name){ m_name = name;}
		virtual void draw(int w, int h) = 0;
		virtual void draw() = 0;
	protected:
		int m_width, m_height, m_layer;
		bool m_mipmaped;
		std::string m_name;
		std::string m_filepath;
	};


}
