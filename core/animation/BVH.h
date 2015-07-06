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
			Dim()
			{
				m_limits[0] = -3.14;
				m_limits[1] = 3.14;
			}
			std::string m_name;
			int m_index;
			float m_axis[3];
			float m_limits[2];
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
			//int m_childrenNb;

			void changeOrdertoZYX();
		};

		std::vector<Joint*> m_joints;
		std::vector<Frame> m_frames;
		float m_frametime; 
		int m_frameNb;
		int m_dims;
	public:
		BVH():m_dims(0){}
		BVH(BVH& bvh)
		{
			m_frames = bvh.m_frames;
			m_frameNb = bvh.m_frameNb;
			m_frametime = bvh.m_frametime;
			m_dims = bvh.m_dims;
			m_joints = bvh.m_joints;
		}

		bool loadFromBVHFile(const std::string& filepath);
		bool saveToBVHFile(const std::string& filepath);
		void changeOrderToZYX();

		bool loadTextFile(const std::string& filepath);
		bool saveTextFile(const std::string& filepath);

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
