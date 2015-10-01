#include "Octree.h"

Octree::Octree()
{
    m_max_level = 8;
}


OctreeCell::OctreeCell(const Vector3_& origin, const Vector3_& halfDim, Octree* octree, int parent) : m_origin(origin), m_halfDimension(halfDim), p_octree(octree), m_parent(parent)
{
    for(int i=0; i<8; ++i)
    {
        m_children[i] = NULL;
    }
    if(parent >= 0)
    {
        m_level = p_octree->m_tree_cell[m_parent]->m_level + 1;
        m_index = p_octree->m_tree_cell.size();
    }else
    {
        m_level = 0;
        m_index = 0;
    }
    p_octree->m_tree_cell.push_back(this);
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
        OctreeCell* temp = tree->m_children[tree->getOctreeCellContainingPoint(point)];
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
        OctreeCell* temp = tree->m_children[tree->getOctreeCellContainingPoint(point)];
        if(temp == NULL) break;

        tree = temp;
        ++level;
    }
    return tree;
}

void OctreeCell::insert(OctreePoint* point)
{
    assert(isInside(point->m_position));
    m_pointsIndexes.push_back(point->m_current);
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
                m_children[i] = new OctreeCell(newOrigin, m_halfDimension*.5f, p_octree, this->m_index);
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
