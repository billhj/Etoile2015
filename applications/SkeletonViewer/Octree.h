#ifndef Octree_H
#define Octree_H
#include "renderer/OpenGL/glhead.h"
#include <cstddef>
#include <vector>
#include "OctreePoint.h"
#include <iostream>
#include "animation\IK\IKChain.h"

#ifdef USING_BOOST
#include <boost/serialization/serialization.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/map.hpp>
#endif

/**!
*
*/
class Octree;

struct OctreeOwner
{
public:
	Octree* p_octreeRoot;
	std::vector<Octree*> m_alltree;
	
	//temp data
	std::vector<std::vector<int>> m_dataIndex;

#ifdef USING_BOOST
		friend class boost::serialization::access;

		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			ar & p_octreeRoot;
			ar & m_alltree;
			ar & m_dataIndex;
		}
#endif
};


class Octree 
{
	
public:

	// Physical position/size. This implicitly defines the bounding 
	// box of this node
	Vec3 origin;         //! The physical center of this node
	Vec3 halfDimension;  //! Half the width/height/depth of this node

	// The tree has up to eight children and can additionally store
	// a point, though in many applications only, the leaves will store data.
	Octree *children[8]; //! Pointers to child octants
	OctreePoint *data;   //! Data point to be stored at a node
	
	/*
	Children follow a predictable pattern to make accesses simple.
	Here, - means less than 'origin' in that dimension, + means greater than.
	child:	0 1 2 3 4 5 6 7
	x:      - - - - + + + +
	y:      - - + + - - + +
	z:      - + - + - + - +
	*/

	std::vector<double> m_cell_min;
	std::vector<double> m_cell_max;
	std::vector<double> m_cell_average;
	std::vector<double> m_cell_drLimits_positive;
	std::vector<double> m_cell_drLimits_negative;

	/*std::vector<std::vector<Etoile::Vector4_, Eigen::aligned_allocator<Etoile::Vector4_> >> m_drData_positive;
	std::vector<std::vector<double>> m_drrhs_positive;
	std::vector<Etoile::Vector4_, Eigen::aligned_allocator<Etoile::Vector4_> > m_drParameter_positive;
	std::vector<std::vector<Etoile::Vector4_, Eigen::aligned_allocator<Etoile::Vector4_> >> m_drData_negative;
	std::vector<std::vector<double>> m_drrhs_negative;
	std::vector<Etoile::Vector4_, Eigen::aligned_allocator<Etoile::Vector4_> > m_drParameter_negative;*/

	OctreeOwner* p_owner;
	int m_parent;
	int m_index;
	int m_level;


#ifdef USING_BOOST
		friend class boost::serialization::access;

		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			ar & origin;
			ar & halfDimension;
			ar & children;
			ar & data;

			ar & m_cell_min;
			ar & m_cell_max;
			ar & m_cell_average;
			ar & m_cell_drLimits_positive;
			ar & m_cell_drLimits_negative;

			ar & p_owner;
			ar & m_parent;
			ar & m_index;
			ar & m_level;
		}
#endif


	Octree(const Vec3& origin, const Vec3& halfDimension, OctreeOwner* owner, int parent) 
		: origin(origin), halfDimension(halfDimension),p_owner(owner), m_parent(parent) 
	{
			// Initially, there are no children
			for(int i=0; i<8; ++i) 
			{
				children[i] = NULL;
			}
			if(parent >= 0)
			{
				m_level = p_owner->m_alltree[m_parent]->m_level + 1;
				m_index = p_owner->m_alltree.size();	
			}else
			{
				m_level = 0;
				m_index = 0;
			}
			p_owner->m_alltree.push_back(this);
			p_owner->m_dataIndex.push_back(std::vector<int>());
			data = NULL;
	}

	Octree(const Octree& copy)
		: origin(copy.origin), halfDimension(copy.halfDimension), data(copy.data) {

	}

	~Octree() {
		// Recursively destroy octants
		for(int i=0; i<8; ++i) 
			delete children[i];
	}

	/*double getDrLimitPositiveByJacobi(int index, double dx, double dy, double dz)
	{
		if(m_drData_positive[index].size() > 1)
			return m_drParameter_positive[index].dot(Etoile::Vector4_(dx,dy,dz,1));
		return m_cell_drLimits_positive[index];

	}

	double getDrLimitNegativeByJacobi(int index, double dx, double dy, double dz)
	{

		if(m_drData_negative[index].size() > 1)
			return m_drParameter_negative[index].dot(Etoile::Vector4_(dx,dy,dz,1));
		return m_cell_drLimits_negative[index];
	}*/

	// Determine which octant of the tree would contain 'point'
	int getOctantContainingPoint(const Vec3& point) const {
		int oct = 0;
		if(point.x >= origin.x) oct |= 4;
		if(point.y >= origin.y) oct |= 2;
		if(point.z >= origin.z) oct |= 1;
		return oct;
	}

	Octree* getSubTreeWithPointAndDepth(const Vec3& point, int depth = 10000)
	{
		Octree* tree = this;
		int level = 0;
		while(tree != NULL && level < depth)
		{
			Octree* temp = tree->children[tree->getOctantContainingPoint(point)];
			//std::cout<<"points: " <<temp->dataIndx.size() <<std::endl;
			if(temp == NULL) break;
			if(p_owner->m_dataIndex[temp->m_index].size() <100)
			{
				break;
			}
			tree = temp;
			++level;
		}
		//std::cout<<"level: " << level <<std::endl;
		return tree;
	}

	bool isLeafNode() const {
		// This is correct, but overkill. See below.
		/*
		for(int i=0; i<8; ++i)
		if(children[i] != NULL) 
		return false;
		return true;
		*/

		// We are a leaf iff we have no children. Since we either have none, or 
		// all eight, it is sufficient to just check the first.
		return children[0] == NULL;
	}

	void insert(OctreePoint* point) {
		// If this node doesn't have a data point yet assigned 
		// and it is a leaf, then we're done!
		p_owner->m_dataIndex[m_index].push_back(point->m_frameIdx);
		if(isLeafNode()) {
			if(data==NULL) {
				data = point;
				return;
			} else {
				// We're at a leaf, but there's already something here
				// We will split this node so that it has 8 child octants
				// and then insert the old data that was here, along with 
				// this new data point

				// Save this data point that was here for a later re-insert
				OctreePoint *oldPoint = data;
				data = NULL;

				// Split the current node and create new empty trees for each
				// child octant.
				for(int i=0; i<8; ++i) {
					// Compute new bounding box for this child
					Vec3 newOrigin = origin;
					newOrigin.x += halfDimension.x * (i&4 ? .5f : -.5f);
					newOrigin.y += halfDimension.y * (i&2 ? .5f : -.5f);
					newOrigin.z += halfDimension.z * (i&1 ? .5f : -.5f);
					children[i] = new Octree(newOrigin, halfDimension*.5f, p_owner, this->m_index);
				}

				// Re-insert the old point, and insert this new point
				// (We wouldn't need to insert from the root, because we already
				// know it's guaranteed to be in this section of the tree)
				children[getOctantContainingPoint(oldPoint->getPosition())]->insert(oldPoint);
				children[getOctantContainingPoint(point->getPosition())]->insert(point);
			}
		} else {
			// We are at an interior node. Insert recursively into the 
			// appropriate child octant
			int octant = getOctantContainingPoint(point->getPosition());
			children[octant]->insert(point);
		}
	}

	// This is a really simple routine for querying the tree for points
	// within a bounding box defined by min/max points (bmin, bmax)
	// All results are pushed into 'results'
	void getPointsInsideBox(const Vec3& bmin, const Vec3& bmax, std::vector<OctreePoint*>& results) {
		// If we're at a leaf node, just see if the current data point is inside
		// the query bounding box
		if(isLeafNode()) {
			if(data!=NULL) {
				const Vec3& p = data->getPosition();
				if(p.x>bmax.x || p.y>bmax.y || p.z>bmax.z) return;
				if(p.x<bmin.x || p.y<bmin.y || p.z<bmin.z) return;
				results.push_back(data);
			}
		} else {
			// We're at an interior node of the tree. We will check to see if
			// the query bounding box lies outside the octants of this node.
			for(int i=0; i<8; ++i) {
				// Compute the min/max corners of this child octant
				Vec3 cmax = children[i]->origin + children[i]->halfDimension;
				Vec3 cmin = children[i]->origin - children[i]->halfDimension;

				// If the query rectangle is outside the child's bounding box, 
				// then continue
				if(cmax.x<bmin.x || cmax.y<bmin.y || cmax.z<bmin.z) continue;
				if(cmin.x>bmax.x || cmin.y>bmax.y || cmin.z>bmax.z) continue;

				// At this point, we've determined that this child is intersecting 
				// the query bounding box
				children[i]->getPointsInsideBox(bmin,bmax,results);
			} 
		}
	}

	void drawAABB()
	{
		float xMin = origin.x - halfDimension.x;
		float xMax = origin.x + halfDimension.x;
		float yMin = origin.y - halfDimension.y;
		float yMax = origin.y + halfDimension.y;
		float zMin = origin.z - halfDimension.z;
		float zMax = origin.z + halfDimension.z;

			glLineWidth(0.01f); 

			glBegin(GL_LINE_STRIP);
			glVertex3f(xMin,yMin,zMin);
			glVertex3f(xMin,yMin,zMax);
			glVertex3f(xMax,yMin,zMax);
			glVertex3f(xMax,yMin,zMin);
			glVertex3f(xMin,yMin,zMin);
			glEnd();
			glBegin(GL_LINE_STRIP);
			glVertex3f(xMin,yMax,zMin);
			glVertex3f(xMin,yMax,zMax);
			glVertex3f(xMax,yMax,zMax);
			glVertex3f(xMax,yMax,zMin);
			glVertex3f(xMin,yMax,zMin);
			glEnd();
			glBegin(GL_LINES);
			glVertex3f(xMin,yMin,zMin);
			glVertex3f(xMin,yMax,zMin);

			glVertex3f(xMin,yMin,zMax);
			glVertex3f(xMin,yMax,zMax);

			glVertex3f(xMax,yMin,zMax);
			glVertex3f(xMax,yMax,zMax);

			glVertex3f(xMax,yMin,zMin);
			glVertex3f(xMax,yMax,zMin);
			glEnd();
	}

	void drawAABBBox()
	{
		drawBox(&origin[0], &halfDimension[0]);
	}


	void drawBox (const float center[3], const float halfsides[3])
	{
		glPushMatrix();
		glTranslatef(center[0],center[1],center[2]);
		drawBox(halfsides);
		glPopMatrix();
	}

	void drawBox (const float halfsides[3])
	{
	  float lx = halfsides[0];
	  float ly = halfsides[1];
	  float lz = halfsides[2];

	  // sides
	  glBegin (GL_TRIANGLE_STRIP);
	  glNormal3f (-1,0,0);
	  glVertex3f (-lx,-ly,-lz);
	  glVertex3f (-lx,-ly,lz);
	  glVertex3f (-lx,ly,-lz);
	  glVertex3f (-lx,ly,lz);
	  glNormal3f (0,1,0);
	  glVertex3f (lx,ly,-lz);
	  glVertex3f (lx,ly,lz);
	  glNormal3f (1,0,0);
	  glVertex3f (lx,-ly,-lz);
	  glVertex3f (lx,-ly,lz);
	  glNormal3f (0,-1,0);
	  glVertex3f (-lx,-ly,-lz);
	  glVertex3f (-lx,-ly,lz);
	  glEnd();

	  // top face
	  glBegin (GL_TRIANGLE_FAN);
	  glNormal3f (0,0,1);
	  glVertex3f (-lx,-ly,lz);
	  glVertex3f (lx,-ly,lz);
	  glVertex3f (lx,ly,lz);
	  glVertex3f (-lx,ly,lz);
	  glEnd();

	  // bottom face
	  glBegin (GL_TRIANGLE_FAN);
	  glNormal3f (0,0,-1);
	  glVertex3f (-lx,-ly,-lz);
	  glVertex3f (-lx,ly,-lz);
	  glVertex3f (lx,ly,-lz);
	  glVertex3f (lx,-ly,-lz);
	  glEnd();
	}

};



#endif