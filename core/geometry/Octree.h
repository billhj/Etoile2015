/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Octree.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <vector>
#include "AxisAlignedBoundingBox.h"

namespace Etoile
{
	/*
	*	Octree struct doing insert delete find, Traversal 
	**/
	template<class T>
	class Octree
	{
		AxisAlignedBoundingBox<T> m_aabb;
		
		int m_level;
		int m_idx;
		int m_maxLevels;	
		int m_maxNbOfElements;
		bool m_isLeaf;
		Octree* m_children[8];
		std::vector<Vec3<T>> m_points;
	public:
		Octree(AxisAlignedBoundingBox<T> aabb, int level = 0, int idx = 0, int maxlevel = -1, int maxNbOfElements = 16);

		/*Octree(AxisAlignedBoundingBox aabb, int level, int idx) : m_aabb(aabb), m_maxLevels(-1), m_isLeaf(true), m_level(level), m_idx(idx), m_maxNbOfElements(16)
		{
			memset(m_children, NULL, sizeof(m_children));
		}*/

		void addPoint(Vec3<T>& point);

		static void addPointIntoNode(Octree* node, Vec3<T>& point);

		static void splitNode(Octree* node);

		bool isLeaf();
		AxisAlignedBoundingBox<T> getAABB();
		int getLevel();
		int getIdx();
		int getMaxLevels();
		int getMaxNbOfElements();
		std::vector<Vec3<T>>& getPoints();
		Octree** getChildren();
	};

	typedef Octree<double> Octreed;
	typedef Octree<float> Octreef;
}