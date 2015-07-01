#ifndef Octree_H
#define Octree_H

#include <cstddef>
#include <vector>
#include "OctreePoint.h"
#include <iostream>

/**!
*
*/
class Octree 
{
	
public:
	int m_level;
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

	std::vector<int> dataIndx;
	/*
	* attributes in Root to all cells
	**/
	std::vector<Octree*> p_alltree;
	
	std::vector<double> m_cell_min;
	std::vector<double> m_cell_max;
	std::vector<double> m_cell_average;
	Octree* p_root;
	int m_index;

	Octree(const Vec3& origin, const Vec3& halfDimension) 
		: origin(origin), halfDimension(halfDimension), data(NULL) {
			// Initially, there are no children
			for(int i=0; i<8; ++i) 
				children[i] = NULL;
	}

	Octree(const Vec3& origin, const Vec3& halfDimension, bool root) 
		: origin(origin), halfDimension(halfDimension), data(NULL) {
			// Initially, there are no children
			for(int i=0; i<8; ++i) 
				children[i] = NULL;
			if(root)
			{
				p_root = this;
				m_index = 0;
				m_level = 0;
				p_alltree.push_back(this);
			}
	}

	Octree(const Octree& copy)
		: origin(copy.origin), halfDimension(copy.halfDimension), data(copy.data) {

	}

	~Octree() {
		// Recursively destroy octants
		for(int i=0; i<8; ++i) 
			delete children[i];
	}

	void setRoot(Octree* root)
	{
		p_root = root;
	}

	Octree* getRoot()
	{
		return p_root;
	}

	// Determine which octant of the tree would contain 'point'
	int getOctantContainingPoint(const Vec3& point) const {
		int oct = 0;
		if(point.x >= origin.x) oct |= 4;
		if(point.y >= origin.y) oct |= 2;
		if(point.z >= origin.z) oct |= 1;
		return oct;
	}

	Octree* getSubTreeWithPointAndDepth(const Vec3& point, int depth)
	{
		Octree* tree = this;
		int level = 0;
		while(tree != 0 && level < depth)
		{
			Octree* temp = tree->children[tree->getOctantContainingPoint(point)];
			std::cout<<"points: " <<temp->dataIndx.size() <<std::endl;
			if(temp->dataIndx.size() == 0)
			{
				break;
			}
			tree = temp;
			++level;
		}
		std::cout<<"level: " << level <<std::endl;
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
		dataIndx.push_back(point->m_frameIdx);
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
					children[i] = new Octree(newOrigin, halfDimension*.5f);
					children[i]->m_level = this->m_level + 1;
					children[i]->setRoot(p_root);
					children[i]->m_index = p_root->p_alltree.size();
					p_root->p_alltree.push_back(children[i]);
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

};

#endif