/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file GLTexture.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "glhead.h"
#include <string>
#include "geometry/Texture.h"

namespace Etoile
{
	class GLTexture : public Texture
	{
	public:
		GLTexture();
		GLTexture(GLuint id);
		GLTexture(GLTexture& t): Texture(t)
		{
			m_id = t.m_id;
			m_internalFormat = t.m_internalFormat;
		}

		void set(GLTexture& t)
		{
			Texture::set(t);
			m_id = t.m_id;
			m_internalFormat = t.m_internalFormat;
		}

		virtual ~GLTexture() ;
		virtual void release() override;
		void setId(GLuint id)
		{
			release();
			m_id = id;
		}
		GLuint getId() const { return m_id; }
		bool usingMipmap() const{ return m_mipmaped;}

		virtual bool create(int iWidth, int iHeight,int _iLayer, GLenum internalFormat = GL_RGBA16F_ARB, GLenum pixeldataformat = GL_RGBA, GLenum type = GL_FLOAT, float* data = 0, bool bCreateMipMap = false) = 0;
		virtual void setGLTextureParameter(GLenum pname, GLfloat param) = 0;
		virtual void use() const = 0;
		virtual void unUse() const = 0;
		virtual void generateMipmap()  const = 0;
		virtual void read(float* data) = 0;	
		virtual void write(int x, int y, int width, int height, float* data) = 0;
		virtual void write(int size, float* data) = 0;
		static int getPixelDataFormat(const GLenum internalFormat);
		static GLenum getInternalFormat(const std::string& internalFormat);
		virtual void draw(int w, int h) override;
		virtual void draw() override;
	protected:
		virtual void setGLTextureParameter(GLenum target, GLenum pname, GLfloat param);
		GLuint m_id;
		GLenum m_internalFormat;
	};


}
