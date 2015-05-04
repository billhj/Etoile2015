/**
* Copyright(C) 2009-2012                
* @author Jing HUANG (original author: bert buchholz)
* @file BinaryHeap.h
* @brief 
* @date 1/2/2011
*/
#pragma once
#include <iostream>
#include <vector>
#include <cmath>

namespace Etoile
{
	// - Dimensions, size of each dimension
	// - Max depth or max amount of points per cell
	// - Enter a point into the tree
	// - When adding a point, check first if the max amount of points is
	//   excessed:
	//   - If not, add the point to the cell.
	//   - If yes, split the cell and redistribute the points into the new
	//     cells.

	template < class Point, size_t dim, class Data = int >
	class RegularBspTree
	{
	public:
		RegularBspTree(Point const& min, Point const& max, int maxDepth, int maxPoints) :
		  m_cellMin(min),
			  m_cellMax(max),
			  m_isLeaf(true),
			  m_depth(0),
			  m_maxDepth(maxDepth),
			  m_maxPoints(maxPoints),
			  m_childrenCount(std::pow(2, dim))
		  { }

		  RegularBspTree() :
		  m_isLeaf(true),
			  m_depth(0),
			  m_maxDepth(10),
			  m_maxPoints(10),
			  m_childrenCount(std::pow(2, dim))
		  { }

		  void getLeafs(std::vector< RegularBspTree<Point, dim>* >& leafs)
		  {
			  if (!m_isLeaf)
			  {
				  for (int i = 0; i < m_childrenCount; ++i)
				  {
					  m_children[i].getLeafs(leafs);
				  }
			  }
			  else
			  {
				  leafs.push_back(this);
			  }
		  }

		  void setDepth(int d)
		  {
			  m_depth = d;
		  }

		  bool getIsLeaf() const
		  {
			  return m_isLeaf;
		  }

		  void setMin(Point const& min)
		  {
			  m_cellMin = min;
		  }

		  void setMax(Point const& max)
		  {
			  m_cellMax = max;
		  }

		  Point const& getMin() const
		  {
			  return m_cellMin;
		  }

		  Point const& getMax() const
		  {
			  return m_cellMax;
		  }

		  std::vector< RegularBspTree<Point, dim> > const& getChildren() const
		  {
			  return m_children;
		  }

		  std::vector<Point> const& getPoints() const
		  {
			  return m_points;
		  }

		  RegularBspTree<Point, dim> & addPoint(Point const& point, Data const& data = Data())
		  {
			  // std::cout << "Depth: " << m_depth << " adding point: " << point << std::endl;

			  if (m_isLeaf)
			  {
				  // add point into cell directly or if full, split the cell
				  // and add then
				  if ((int)m_points.size() >= m_maxPoints && m_depth < m_maxDepth)
				  {
					  // std::cout << "Full leaf, splitting." << std::endl;
					  split();

					  return addPoint(point, data);
				  }
				  else
				  {
					  // std::cout << "Leaf okay, pushing." << std::endl;
					  m_points.push_back(point);
					  m_data.push_back(data);

					  assert(m_points.size() == m_data.size());
				  }
			  }
			  else
			  {
				  // find the right child and add point there
				  // std::cout << "Not a leaf, adding to child." << std::endl;
				  for (int i = 0; i < m_childrenCount; ++i)
				  {
					  if (m_children[i].isPointIn(point))
					  {
						  return m_children[i].addPoint(point, data);
					  }
				  }
			  }

			  return *this;
		  }


		  bool isPointIn(Point const& point) const
		  {
			  // return (point >= m_cellMin && point <= m_cellMax);

			  for (unsigned int i = 0; i < dim; ++i)
			  {
				  if (point[i] < m_cellMin[i] || point[i] > m_cellMax[i]) return false;
			  }

			  return true;
		  }

		  void split()
		  {
			  if (!m_isLeaf) return;

			  m_children.resize(m_childrenCount, RegularBspTree<Point, dim>(m_cellMin, m_cellMax, m_maxDepth, m_maxPoints));

			  m_isLeaf = false;

			  m_children[0].setDepth(m_depth + 1);

			  for (unsigned int d = 1; d <= dim; ++d)
			  {
				  int spacing = m_childrenCount / std::pow(2, d);

				  for (unsigned int j = 0; j < std::pow(2, d - 1); ++j)
				  {
					  int parentIndex = j * spacing * 2;
					  int childIndex = parentIndex + spacing;
					  RegularBspTree const& parent = m_children[parentIndex];

					  RegularBspTree leftChild = parent;
					  leftChild.m_children.clear();
					  RegularBspTree rightChild = parent;
					  rightChild.m_children.clear();

					  leftChild.m_cellMax[d - 1]  -= 0.5f * (parent.m_cellMax[d - 1] - parent.m_cellMin[d - 1]);
					  rightChild.m_cellMin[d - 1] += 0.5f * (parent.m_cellMax[d - 1] - parent.m_cellMin[d - 1]);

					  /*
					  std::cout <<
					  "d: " << d <<
					  " j: " << j <<
					  " spacing: " << spacing <<
					  " from: " << parentIndex <<
					  " to: " << parentIndex << ", " << childIndex << std::endl;
					  */

					  m_children[parentIndex] = leftChild;
					  m_children[childIndex] = rightChild;
				  }
			  }

			  for (unsigned int i = 0; i < m_points.size(); ++i)
			  {
				  Point const& point = m_points[i];
				  Data const& data = m_data[i];

				  for (int j = 0; j < m_childrenCount; ++j)
				  {
					  if (m_children[j].isPointIn(point))
					  {
						  m_children[j].addPoint(point, data);
						  break;
					  }
				  }
			  }

			  m_points.clear();
			  m_data.clear();
		  }

		  friend std::ostream& operator<< (std::ostream& stream, RegularBspTree const& q)
		  {
			  stream << q.m_depth << " " << q.m_cellMin << " " << q.m_cellMax << std::endl;

			  for (unsigned int i = 0; i < q.m_points.size(); ++i)
			  {
				  std::cout << "[" << q.m_points[i] << "] Data: " << q.m_data[i] << std::endl;
			  }


			  if (!q.m_isLeaf)
			  {
				  std::cout << "Children:" << std::endl;

				  for (int i = 0; i < q.m_childrenCount; ++i)
				  {
					  stream << q.m_children[i] << std::endl;
				  }
			  }

			  return stream;
		  }

		  RegularBspTree const& query(Point const& point) const
		  {
			  if (!m_isLeaf)
			  {
				  for (int j = 0; j < m_childrenCount; ++j)
				  {
					  if (m_children[j].isPointIn(point))
					  {
						  return m_children[j].query(point);
					  }
				  }
			  }

			  return *this;
		  }

	private:
		Point m_cellMin;
		Point m_cellMax;
		std::vector< RegularBspTree<Point, dim> > m_children;

		std::vector<Point> m_points;
		std::vector<Data> m_data;

		bool m_isLeaf;
		int m_depth;
		int m_maxDepth;
		int m_maxPoints;

		int m_childrenCount;
	};
}

//template <class Point>
//void draw(QPainter & p, RegularBspTree<Point, 2> const& quadTree, Point const& min, Point const& max)
//{
//    // std::cout << "Drawing" << std::endl;
//
//    p.setPen(Qt::black);
//
//    float xRange = max[0] - min[0];
//    float yRange = max[1] - min[1];
//
//    QRect rect(
//                std::floor((quadTree.getMin()[0] - min[0]) / xRange * p.window().width() + 0.5f),
//                std::floor((quadTree.getMin()[1] - min[1]) / yRange * p.window().height() + 0.5f),
//                std::floor((quadTree.getMax()[0] - quadTree.getMin()[0]) / xRange * p.window().width() + 0.5f),
//                std::floor((quadTree.getMax()[1] - quadTree.getMin()[1]) / yRange * p.window().width() + 0.5f)
//                );
//
//    p.drawRect(rect);
//
//    std::vector<Point> const& points = quadTree.getPoints();
//
//    p.setPen(Qt::red);
//
//    for (unsigned int i = 0; i < points.size(); ++i)
//    {
//        p.drawEllipse(
//                    (points[i][0] - min[0]) / xRange * p.window().width(),
//                    (points[i][1] - min[1]) / xRange * p.window().width(),
//                    2, 2);
//    }
//
//    if (!quadTree.getIsLeaf())
//    {
//        std::vector<RegularBspTree<Point, 2> > const& children = quadTree.getChildren();
//
//        for (unsigned int i = 0; i < children.size(); ++i)
//        {
//            draw(p, children[i], min, max);
//        }
//    }
//}
