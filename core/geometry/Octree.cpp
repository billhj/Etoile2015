/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Octree.cpp
* @brief 
* @date 1/2/2011
*/


#include "Octree.h"

/**
* @brief For tracking memory leaks under windows using the crtdbg
*/
#if ( defined( _DEBUG ) || defined( DEBUG ) ) && defined( _MSC_VER )
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif

namespace Etoile
{
	/*
	*	Octree struct doing insert delete find, Traversal 
	**/
		template<class T>
		Octree<T>::Octree(AxisAlignedBoundingBox<T> aabb, int level = 0, int idx = 0, int maxlevel = -1, int maxNbOfElements = 16) : m_aabb(aabb), m_level(level), m_idx(idx), m_maxLevels(maxlevel), m_maxNbOfElements(maxNbOfElements), m_isLeaf(true)
		{
			memset(m_children, NULL, sizeof(m_children));
		}

		/*Octree(AxisAlignedBoundingBox aabb, int level, int idx) : m_aabb(aabb), m_maxLevels(-1), m_isLeaf(true), m_level(level), m_idx(idx), m_maxNbOfElements(16)
		{
			memset(m_children, NULL, sizeof(m_children));
		}*/

		template<class T>
		void Octree<T>::addPoint(Vec3<T>& point)
		{
			addPointIntoNode(this, point);
		}

		template<class T>
		static void Octree<T>::addPointIntoNode(Octree* node, Vec3<T>& point)
		{
			if(node->isLeaf())
			{
				if(node->getPoints().size() < node->getMaxNbOfElements() || (node->getLevel() >= node->getMaxLevels() || node->getMaxLevels() < 0))
				{
					node->getPoints().push_back(point);
				}else
				{
					splitNode(node);
					addPointIntoNode(node, point);
				}
			}else
			{
				for (int j= 0; j < 8; j++)
				{
					if(node->getChildren()[j]->getAABB().isInclude(point))
					{
						addPointIntoNode(node->getChildren()[j], point);
					}
				}
			}
		}

		template<class T>
		static void Octree<T>::splitNode(Octree* node)
		{
			if(node->isLeaf())
			{
				

				AxisAlignedBoundingBox aabb[8];
				T xMin = node->getAABB().minimum().x();
				T yMin = node->getAABB().minimum().y();
				T zMin = node->getAABB().minimum().y();
				T xC = node->getAABB().center().x();
				T yC = node->getAABB().center().y();
				T zC = node->getAABB().center().y();
				T xMax = node->getAABB().maximum().x();
				T yMax = node->getAABB().maximum().y();
				T zMax = node->getAABB().maximum().y();

				std::pair<T, T> x[2];
				x[0] = std::pair<T, T>(xMin, xC);
				x[1] = std::pair<T, T>(xC, xMax);

				std::pair<T, T> y[2];
				y[0] = std::pair<T, T>(yMin, yC);
				y[1] = std::pair<T, T>(yC, yMax);

				std::pair<T, T> z[2];
				z[0] = std::pair<T, T>(zMin, zC);
				z[1] = std::pair<T, T>(zC, zMax);


				aabb[0].setInterval(Vec3<T>(x[0][0],y[0][0],z[0][0]), Vec3<T>(x[0][1],y[0][1],z[0][1]));
				aabb[1].setInterval(Vec3<T>(x[1][0],y[0][0],z[0][0]), Vec3<T>(x[1][1],y[0][1],z[0][1]));

				aabb[2].setInterval(Vec3<T>(x[0][0],y[1][0],z[0][0]), Vec3<T>(x[0][1],y[1][1],z[0][1]));
				aabb[3].setInterval(Vec3<T>(x[1][0],y[1][0],z[0][0]), Vec3<T>(x[1][1],y[1][1],z[0][1]));

				aabb[4].setInterval(Vec3<T>(x[0][0],y[0][0],z[1][0]), Vec3<T>(x[0][1],y[0][1],z[1][1]));
				aabb[5].setInterval(Vec3<T>(x[1][0],y[0][0],z[1][0]), Vec3<T>(x[1][1],y[0][1],z[1][1]));

				aabb[6].setInterval(Vec3<T>(x[0][0],y[1][0],z[1][0]), Vec3<T>(x[0][1],y[1][1],z[1][1]));
				aabb[7].setInterval(Vec3<T>(x[1][0],y[1][0],z[1][0]), Vec3<T>(x[1][1],y[1][1],z[1][1]));
				
				for (int i= 0; i < 8; i++)
				{
					node->getChildren()[i] = new Octree(aabb[i], node->getLevel() + 1, i, node->getMaxLevels() - 1, node->getMaxNbOfElements());
				}

				for(int i = 0 ; i < node->getPoints().size(); ++i)
				{
					Vec3<T> p = node->getPoints()[i];
					for (int j= 0; j < 8; i++)
					{
						if(node->getChildren()[j]->getAABB().isInclude(p))
						{
							node->getChildren()[j]->getPoints().push_back(p);
						}
					}
				}
				node->getPoints().clear();
				node->m_isLeaf = false;
			}
		} 

		template<class T>
		bool Octree<T>::isLeaf(){return m_isLeaf;}

		template<class T>
		AxisAlignedBoundingBox<T> Octree<T>::getAABB(){return m_aabb;}

		template<class T>
		int Octree<T>::getLevel(){return m_level;}

		template<class T>
		int Octree<T>::getIdx(){return m_idx;}

		template<class T>
		int Octree<T>::getMaxLevels(){return m_maxLevels;}

		template<class T>
		int Octree<T>::getMaxNbOfElements(){return m_maxNbOfElements;}

		template<class T>
		std::vector<Vec3<T>>& Octree<T>::getPoints(){return m_points;}

		template<class T>
		Octree<T>** Octree<T>::getChildren(){return m_children;}
	
}