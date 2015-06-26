#pragma once
#include <vcg/space/index/octree.h>
#include <string>
#include <vector>

struct FrameData
{
	std::vector<double> m_values;
};

struct DataPoint
{
	DataPoint(double x, double y, double z, int frameIdx)
	{
		m_x = x;
		m_y = y;
		m_z = z;
		m_frameIdx = frameIdx;
	}

	double m_x, m_y, m_z;
	int m_frameIdx;
};

class OctreeSkeleton
{
public:
	OctreeSkeleton(void);
	~OctreeSkeleton(void);
	void loadFromCSVFile(const std::string& filename);
	void saveCSVFileByFrameIdx(const std::string& filename, std::vector<int> indx);


	std::vector<std::string> m_headers;
	std::vector<FrameData> m_framesData;

	int m_leftHand_index;
	int m_rightHand_index;
};

