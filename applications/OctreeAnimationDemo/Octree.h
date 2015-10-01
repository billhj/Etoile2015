#ifndef OCTREE_H
#define OCTREE_H

#include <vector>
#include <Eigen/Dense>

typedef Eigen::MatrixXd MatrixX_;
typedef Eigen::Matrix3d Matrix3_;
typedef Eigen::VectorXd VectorX_;
typedef Eigen::Vector3d Vector3_;
typedef Eigen::Vector4d Vector4_;
typedef Eigen::Vector2d Vector2_;
typedef Eigen::AngleAxisd AngleAxis_;

#define USING_BOOST
#ifdef USING_BOOST
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/map.hpp>
#include <iostream>
#include <fstream>
#endif


namespace boost
{
    template<class Archive, typename _Scalar, int _Rows, int _Cols, int _Options, int _MaxRows, int _MaxCols>
    inline void serialize(
        Archive & ar,
        Eigen::Matrix<_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols> & t,
        const unsigned int file_version
    )
    {
        for(size_t i=0; i<t.size(); i++)
            ar & t.data()[i];
    }
}


class OctreeCell;

class OctreePoint
{
public:
    OctreePoint(Vector3_ position = Vector3_(0,0,0), int current = -1, int previous = -1) : m_position(position),
        m_current(current), m_previous(previous)
    {

    }
    Vector3_ m_position;
    int m_current;
    int m_previous;

#ifdef USING_BOOST
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & m_position;
        ar & m_current;
        ar & m_previous;
    }
#endif
};

class Octree
{
public:
    Octree();\

    OctreeCell * p_rootcell;
    std::vector<OctreeCell*> m_tree_cell;
    int m_max_level;

#ifdef USING_BOOST

    void save(const std::string& filename){
        std::ofstream ofs(filename);
        boost::archive::text_oarchive oa(ofs);
        oa << *this;
        ofs.close();
    }

    void load(const std::string& filename){
        std::ifstream ifs(filename);
        boost::archive::text_iarchive ia(ifs);
        ia >> *this;
        ifs.close();
    }

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & p_rootcell;
        ar & m_tree_cell;
        ar & m_max_level;
    }
#endif
};


class OctreeCell
{
public:
    Vector3_ m_origin;
    Vector3_ m_halfDimension;
    Octree* p_octree;
    OctreeCell * m_children[8];
    int m_index;
    int m_level;
    int m_parent;
    std::vector<int> m_pointsIndexes;
    OctreePoint * p_currentPoint;

    OctreeCell(){}
    OctreeCell(const Vector3_& origin, const Vector3_& halfDim, Octree* octree, int parent);
    ~OctreeCell() {
        // Recursively destroy octants
        for(int i=0; i<8; ++i)
            delete m_children[i];
    }
    bool isInside(const Vector3_& point);
    int getOctreeCellContainingPoint(const Vector3_& point) const;
    std::vector<OctreeCell*> getSubTreeCellsWithPointAndDepth(const Vector3_& point, int depth = 10000);
    OctreeCell* getSubTreeCellWithPointAndDepth(const Vector3_& point, int depth = 10000);
    bool isLeafNode() const {
        return m_children[0] == NULL;
    }

    void insert(OctreePoint* point);



#ifdef USING_BOOST
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & m_origin;
        ar & m_halfDimension;
        ar & m_children;
        ar & p_currentPoint;
        ar & p_octree;

        ar & m_parent;
        ar & m_index;
        ar & m_level;

        ar & m_pointsIndexes;
    }
#endif
};


#endif // OCTREE_H
