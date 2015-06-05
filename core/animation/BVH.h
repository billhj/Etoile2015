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

namespace Etoile
{
	struct Frame
	{
		std::vector<float> m_values;
	};

	struct BVH
	{
		struct Dim
		{
			std::string m_name;
			int m_index;
		};

		struct Joint
		{
			Joint(BVH* bvh, int parent, int dof, const std::string& name);
			Joint(BVH* bvh, int parent, const std::string& name);
			void init(int dof);
			std::string m_name;
			int m_index;
			int m_index_parent;
			BVH* p_owner;
			int m_dof;
			std::vector<Dim> m_dims;
			float m_offset[3];
			bool m_isleaf;
			int m_level;
			int m_childrenNb;
		};

		std::vector<Joint*> m_joints;
		std::vector<Frame> m_frames;
		float m_frametime; 
		int m_frameNb;
		int m_dims;
		bool m_isMeter;
	public:
		BVH():m_dims(0){}
		bool loadFromBVHFile(const std::string& filepath);
		bool saveToBVHFile(const std::string& filepath);
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

}
