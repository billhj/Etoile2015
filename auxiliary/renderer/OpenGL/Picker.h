/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Picker.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "glhead.h"
#include <string>

namespace Etoile
{
	class Picker
	{
	public:
		static void processPickUbyte(int cursorX, int cursorY, GLubyte pixel[3]);
		static void processPickInt(int cursorX, int cursorY, GLint pixel[3]);
		static void processPickFloat(int cursorX, int cursorY, GLfloat pixel[3]);
	};


}
