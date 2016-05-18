/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file BVH.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <string>
#include <vector>
#include <stack>
#include <map>

#include "Skeleton.h"

struct Frame
{
	std::vector<double> m_values;
};

struct BVH
{
	Skeleton m_skeleton;
	std::vector<Frame> m_frames;
	float m_frametime; 
	int m_frameNb;
	double scale;
public:
	BVH(){scale = 0.01;}
	BVH(BVH& bvh)
	{
		m_frames = bvh.m_frames;
		m_frameNb = bvh.m_frameNb;
		m_frametime = bvh.m_frametime;
		m_skeleton = bvh.m_skeleton;
		scale = bvh.scale;
	}

	bool loadFromBVHFile(const std::string& filepath);
	bool saveToBVHFile(const std::string& filepath);
	void changeOrderToZYX();

	bool loadTextFile(const std::string& filepath);
	bool saveTextFile(const std::string& filepath);
	std::string name(){return m_filepath;}
private:
	void trimString( std::string& string);
	void read(std::istream& in);
	void readJoint(std::istream& in, const std::string& name);
	void readEnd(std::istream& in, const std::string& name);
	void readLine(std::istream& in, std::string& line);
	void readFrames(std::istream& in);

	void write(std::ostream& out);
	std::string m_filepath;
	std::stack<int> m_index;
};

