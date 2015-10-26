#include "Octree.h"
#include <queue>

#define USING_TBB
#define NOMINMAX
#ifdef USING_TBB
#include "tbb/tbb.h"
//using namespace tbb;
#endif

Octree::Octree()
{
	init(Vector3_(0,0,0), Vector3_(1,1,1));
}

Octree::Octree(const Vector3_& origin, const Vector3_& halfDim)
{
	init(origin, halfDim);
}

void Octree::init(const Vector3_& origin, const Vector3_& halfDim)
{
	m_max_level = 8;
	p_rootcell = boost::shared_ptr<OctreeCell>(new OctreeCell(origin, halfDim, this, -1));
	m_tree_cell.clear();
	m_tree_cell.push_back(p_rootcell);
}

void Octree::reset(const Vector3_& origin, const Vector3_& halfDim)
{
	init(origin, halfDim);
	for(unsigned int i = 0; i < m_tree_points.size(); ++i)
	{
		OctreePoint& p = m_tree_points[i];
		if(p_rootcell->isInside(p.m_position))
		{
			p_rootcell->insert(&p);
		}
	}
	updateParameters();
}

void Octree::updateParameters()
{
#ifdef USING_TBB
	tbb::parallel_for(size_t(0), size_t(m_tree_cell.size()), [&] (size_t i) {  m_tree_cell[i]->updateParameters(); });
#else

	for(unsigned int i = 0;  i < m_tree_cell.size(); ++i)
	{
		m_tree_cell[i]->updateParameters();
	}
#endif
}

void Octree::insertPoint(OctreePoint& point)
{
	if(p_rootcell->isInside(point.m_position))
	{
		point.m_idx = m_tree_points.size();
		m_tree_points.push_back(point);
		p_rootcell->insert(&point);
	}
}

void Octree::writeIntoTXT(const std::string& filename, int depthMax)
{
	std::cout<<"start writing TXT Struct! "<<std::endl;
	std::queue<OctreeCell*> cells;
	std::vector<OctreeCell*> cellsToCompute;
	cells.push(p_rootcell.get());
	while(!cells.empty())
	{
		OctreeCell* cell = cells.front();
		cellsToCompute.push_back(cell);
		if(cell->m_children[0] != NULL)
		{
			for(unsigned int i = 0; i < 8; ++i)
			{
				OctreeCell* cell0 = cell->m_children[i].get();
				if(! (cell0->m_level > depthMax))
				{
					cells.push(cell0);
				}
			}
		}
		cells.pop();
	}

	std::ofstream out;
	out.open(filename);
	for(unsigned int i = 0; i < cellsToCompute.size(); ++i)
	{
		OctreeCell* cell = cellsToCompute[i];
		out<<"cell "<<std::endl;;

		out<<"level ";
		for(unsigned int j = 0; j < cell->m_id_depthorder.size(); ++j)
		{
			out<< cell->m_id_depthorder[j]<<" ";
		}
		out<<std::endl;

		out<<"size "<<cell->m_origin[0]<<" "<<cell->m_origin[1]<<" "<<cell->m_origin[2]<<" "<<cell->m_halfDimension[0]<<" "<<cell->m_halfDimension[1]<<" "<<cell->m_halfDimension[2]<<std::endl;

		out<<"min ";
		for(unsigned int j = 0; j < cell->m_min.size(); ++j)
		{
			out<< cell->m_min[j]<<" ";
		}
		out<<std::endl;

		out<<"max ";
		for(unsigned int j = 0; j < cell->m_max.size(); ++j)
		{
			out<< cell->m_max[j]<<" ";
		}
		out<<std::endl;
		out<<"lambda ";
		for(unsigned int j = 0; j < cell->m_max.size(); ++j)
		{
			out<< cell->m_lambda[j]<<" ";
		}
		out<<std::endl;

	}
	out.close();
	std::cout<<"end writing TXT Struct! "<<std::endl;
}

OctreeCell::OctreeCell(const Vector3_& origin, const Vector3_& halfDim, Octree* octree, int parent, int localindx) : m_origin(origin), m_halfDimension(halfDim), p_octree(octree), m_parent(parent)
{
	for(int i=0; i<8; ++i)
	{
		m_children[i].reset();
	}
	if(parent >= 0)
	{
		m_level = p_octree->m_tree_cell[m_parent]->m_level + 1;
		m_index = p_octree->m_tree_cell.size();

		OctreeCell* parentcell = p_octree->m_tree_cell[m_parent].get();
		m_id_depthorder = parentcell->m_id_depthorder;
		m_id_depthorder.push_back(localindx);
	}else
	{
		m_level = 0;
		m_index = 0;
		m_id_depthorder.push_back(0);
	}
	
	p_currentPoint = NULL;
}


bool OctreeCell::isInside(const Vector3_& point)
{
	if(m_origin[0] + m_halfDimension[0] < point[0]  ||  m_origin[0] - m_halfDimension[0] > point[0]  ||
		m_origin[1] + m_halfDimension[1] < point[1]  ||  m_origin[1] - m_halfDimension[1] > point[1]  ||
		m_origin[2] + m_halfDimension[2] < point[2]  ||  m_origin[2] - m_halfDimension[2] > point[2]  )
		return false;
	return true;
}

int OctreeCell::getOctreeCellContainingPoint(const Vector3_& point) const {
	int oct = 0;
	if(point[0] >= m_origin[0]) oct |= 4;
	if(point[1] >= m_origin[1]) oct |= 2;
	if(point[2] >= m_origin[2]) oct |= 1;
	return oct;
}

std::vector<OctreeCell*> OctreeCell::getSubTreeCellsWithPointAndDepth(const Vector3_& point, int depth)
{
	std::vector<OctreeCell*> trees;
	if(!isInside(point)) return trees;
	OctreeCell* tree = this;
	trees.push_back(tree);
	int level = 0;
	while(tree != NULL && level < depth)
	{
		OctreeCell* temp = tree->m_children[tree->getOctreeCellContainingPoint(point)].get();
		if(temp == NULL) break;
		//        if(temp->m_dataIndex.size() < 2)
		//        {
		//            break;
		//        }
		tree = temp;
		++level;
		trees.push_back(tree);
	}
	return trees;
}

OctreeCell* OctreeCell::getSubTreeCellWithPointAndDepth(const Vector3_& point, int depth)
{
	OctreeCell* tree = this;
	if(!isInside(point)) return NULL;
	int level = 0;
	while(tree != NULL && level < depth)
	{
		OctreeCell* temp = tree->m_children[tree->getOctreeCellContainingPoint(point)].get();
		if(temp == NULL) break;

		tree = temp;
		++level;
	}
	return tree;
}

void OctreeCell::insert(OctreePoint* point)
{
	assert(isInside(point->m_position));
	m_pointsIndexes.push_back(point->m_idx);
	if(isLeafNode()) {
		if(p_currentPoint == NULL || m_level >= p_octree->m_max_level) {
			p_currentPoint = point;
			return;
		} else {
			// We're at a leaf, but there's already something here
			// We will split this node so that it has 8 child octants
			// and then insert the old data that was here, along with
			// this new data point

			// Save this data point that was here for a later re-insert
			OctreePoint *oldPoint = p_currentPoint;

			// Split the current node and create new empty trees for each
			// child octant.
			for(int i = 0; i < 8; ++i) {
				// Compute new bounding box for this child
				Vector3_ newOrigin = m_origin;
				newOrigin[0] += m_halfDimension[0] * (i&4 ? .5f : -.5f);
				newOrigin[1] += m_halfDimension[1] * (i&2 ? .5f : -.5f);
				newOrigin[2] += m_halfDimension[2] * (i&1 ? .5f : -.5f);
				m_children[i] = boost::shared_ptr<OctreeCell>(new OctreeCell(newOrigin, m_halfDimension*.5f, p_octree, this->m_index, i));
				p_octree->m_tree_cell.push_back(m_children[i]);
			}

			// Re-insert the old point, and insert this new point
			// (We wouldn't need to insert from the root, because we already
			// know it's guaranteed to be in this section of the tree)
			m_children[getOctreeCellContainingPoint(oldPoint->m_position)]->insert(oldPoint);
			m_children[getOctreeCellContainingPoint(point->m_position)]->insert(point);
		}
	} else {
		// We are at an interior node. Insert recursively into the
		// appropriate child octant
		int octant = getOctreeCellContainingPoint(point->m_position);
		m_children[octant]->insert(point);
	}
}


void OctreeCell::updateParameters()
{
	if(p_octree->m_tree_points.size() <= 0) return;
	if(m_pointsIndexes.size() <= 0) return;
	int vsize = p_octree->m_tree_points[0].m_data.m_values.size();
	m_min.resize(vsize);
	m_max.resize(vsize);
	m_avg.resize(vsize);
	m_lambda.resize(vsize);
	for(int i = 0; i < vsize; ++i)
	{
		m_min[i] = 10000;
		m_max[i] = -10000;
		m_avg[i] = 0;
		m_lambda[i] = 0;
	}
	int sizeLambda = 0;
	for(unsigned int j = 0; j < m_pointsIndexes.size(); ++j)
	{
		int idx = m_pointsIndexes[j];
		OctreePoint& point = p_octree->m_tree_points[idx];
		
		if(point.m_data.m_lambda_values.size() > 0)
		{
			++sizeLambda;
			for(int i = 0; i < vsize; ++i)
			{
				m_lambda[i] += point.m_data.m_lambda_values[i];
			}
		}
		for(int i = 0; i < vsize; ++i)
		{
			m_min[i] = std::min(point.m_data.m_values[i], m_min[i]);
			m_max[i] = std::max(point.m_data.m_values[i], m_max[i]);
			m_avg[i] += point.m_data.m_values[i];
		}
	}
	for(int i = 0; i < vsize; ++i)
	{
		m_avg[i] /= m_pointsIndexes.size();
	}
	if(sizeLambda > 0)
	{
		for(int i = 0; i < vsize; ++i)
		{
			m_lambda[i] /= sizeLambda;
		}
	}
}