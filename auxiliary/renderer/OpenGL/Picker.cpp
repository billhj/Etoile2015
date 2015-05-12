/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Picker.cpp
* @brief 
* @date 1/2/2011
*/

#include "Picker.h"

namespace Etoile
{

	void Picker::processPickUbyte(int cursorX, int cursorY, GLubyte pixel[3])
	{
		GLint viewport[4];
		glGetIntegerv(GL_VIEWPORT,viewport);
		glReadPixels(cursorX,viewport[3] - cursorY,1,1,GL_RGB,GL_UNSIGNED_BYTE,(void *)pixel);
		//printf("%d %d %d\n",pixel[0],pixel[1],pixel[2]);
	}

	void Picker::processPickInt(int cursorX, int cursorY, GLint pixel[3])
	{
		GLint viewport[4];
		glGetIntegerv(GL_VIEWPORT,viewport);
		glReadPixels(cursorX,viewport[3] - cursorY,1,1,GL_RGB,GL_INT,(void *)pixel);
		//printf("%d %d %d\n",pixel[0],pixel[1],pixel[2]);
	}
	void Picker::processPickFloat(int cursorX, int cursorY, GLfloat pixel[3])
	{
		GLint viewport[4];
		glGetIntegerv(GL_VIEWPORT,viewport);
		glReadPixels(cursorX,viewport[3] - cursorY,1,1,GL_RGB,GL_FLOAT,(void *)pixel);
		//printf("%f %f %f\n",pixel[0],pixel[1],pixel[2]);
	}
}
