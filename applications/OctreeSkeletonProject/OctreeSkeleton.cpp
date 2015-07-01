#include "OctreeSkeleton.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <queue>

OctreeSkeleton::OctreeSkeleton(void)
{
	std::cout<<"start"<<std::endl;
	m_bvh.loadFromBVHFile("Ag1CS_Brian.bvh");
	m_ikchain.loadFromFile("ikchain.sk");
	m_dataIsLoaded = false;
	p_tree = new Octree(Vec3(-0.15, 0.2, 0.15), Vec3(0.25, 0.4, 0.45), true);

	loadFromCSVFile("angle_right_anger_brian.csv");
	computePoints();

	loadDataIntoOctree();
	computeMinMaxAverage();
	computeMinMaxAverageByDepth(5);
	std::cout<<"end"<<std::endl;
}


OctreeSkeleton::~OctreeSkeleton(void)
{
}


void OctreeSkeleton::computePoints()
{
	std::cout << " start compute points!"<<std::endl;
	for(unsigned int j = 0; j < m_framesData.size(); ++j)
	{
		for(int i = 0 ; i < m_ikchain.m_localRotations.size(); ++i)
		{
			m_ikchain.m_localRotations[i] = Eigen::AngleAxis<double>(m_framesData[j].m_values[i], m_ikchain.m_axis[i]);
		}
		m_ikchain.update();
		int end = m_ikchain.m_globalPositions.size() - 1;
		m_framesData[j].points[0] = m_ikchain.m_globalPositions[end][0];
		m_framesData[j].points[1] = m_ikchain.m_globalPositions[end][1];
		m_framesData[j].points[2] = m_ikchain.m_globalPositions[end][2];
		//std::cout<< j<<" "<<m_ikchain.m_globalPositions[end].transpose() <<std::endl;
		if(j % 100 == 0)
		{
			std::cout<<".";
		}
	}
	std::cout << "  end compute points!"<<std::endl;
}

void OctreeSkeleton::loadFromCSVFile(const std::string& filename)
{
	std::fstream in(filename.c_str(), std::ios_base::in );

	if (!in.is_open() || !in.good())
	{
		std::cerr << "[OctreeSkeleton] : cannot not open file "
			<< filename
			<< std::endl;
	}
	std::cout << " read CSV file starts!"<<std::endl;
	int lineidx = 0;
	std::string line;
	while( in && !in.eof() )
	{
		std::getline(in,line);
		if ( in.bad() ){
			std::cout << "  Warning! Could not read file properly!"<<std::endl;
		}
		else
		{
			if(lineidx == 0)
			{
				if ( line.size() == 0 || line[0] == '#' || isspace(line[0]) || line.empty() ) {
					continue;
				}
				m_headers.clear();
				std::stringstream stream(line);
				std::string valueString;
				while (std::getline(stream, valueString, ','))
				{
					m_headers.push_back(valueString);
				}
			}
			else
			{
				if ( line.size() == 0 || line[0] == '#' || isspace(line[0]) || line.empty() ) {
					continue;
				}
				int indx = m_framesData.size();
				m_framesData.push_back(FrameData());

				std::stringstream stream(line);
				std::string valueString;
				while (std::getline(stream, valueString, ','))
				{
					double value = std::stod(valueString);
					m_framesData[indx].m_values.push_back(value);
				}
			}
		}
		++lineidx;

	}

	in.close();
	std::cout << " read CSV file finished!"<<std::endl;
	m_dataIsLoaded = true;
}


void OctreeSkeleton::saveCSVFileByFrameIdx(const std::string& filename, std::vector<int> indx)
{
	std::cout << " save " << filename <<" file starts!"<<std::endl;
	std::ofstream out;
	out.open(filename);

	for(unsigned int i = 0; i < m_headers.size(); ++i)
	{
		out<<m_headers[i]<<",";
	}
	out<<std::endl;
	for(unsigned int i = 0; i < indx.size(); ++i)
	{
		FrameData& frame = m_framesData[indx[i]];
		for(unsigned int j = 0; j < frame.m_values.size(); ++j)
		{
			out<<frame.m_values[j]<<",";
		}
		out<<std::endl;
	}
	out.close();
	std::cout << " save " << filename <<" file ended!"<<std::endl;
}


void OctreeSkeleton::loadDataIntoOctree()
{
	if(!m_dataIsLoaded || m_framesData.size() < 1)
	{
		std::cout<<"data is not loaded"<<std::endl;
		return;
	}

	std::cout<<"loading data into Octree"<<std::endl;
	for(unsigned int i = 0; i < m_framesData.size(); ++i)
	{
		FrameData& frame = m_framesData[i];
		//right
		{
			OctreePoint* point = new OctreePoint();
			point->position = Vec3(frame.points[0],frame.points[1],frame.points[2]);
			point->m_frameIdx = i;
			p_tree->insert(point);
		}

		if(i % 100 == 0)
		{
			std::cout<<".";
		}
	}

	std::cout<<"right hand data finished"<<std::endl;

	//int Nsize = m_framesData.size();
	//for(unsigned int i = 0; i < m_framesData.size(); ++i)
	//{
	//	FrameData& frame = m_framesData[i];
	//	//right
	//	{
	//		OctreePoint* point = new OctreePoint();
	//		point->position = Vec3(frame.m_values[m_leftHand_index], frame.m_values[m_leftHand_index + 1], frame.m_values[m_leftHand_index + 2]);
	//		point->m_frameIdx = i + Nsize;
	//		p_tree->insert(point);
	//	}

	//	if(i % 100 == 0)
	//	{
	//		std::cout<<".";
	//	}
	//}
	std::cout<<"left hand data finished"<<std::endl;

	std::cout<<std::endl<<"loading data into Octree is finished"<<std::endl;
}

static void debugValue(Octree* cell)
{
	std::cout <<std::endl<<cell->m_index  <<" "<<cell->dataIndx.size() <<std::endl;
	std::cout<<"r_elbow_Z_18"<<cell->m_cell_min[18] <<" " << cell->m_cell_max[18] <<" "<< cell->m_cell_average[18] <<" " <<std::endl;
	std::cout<<"r_elbow_Y_19"<<cell->m_cell_min[19] <<" " << cell->m_cell_max[19] <<" "<< cell->m_cell_average[19] <<" " <<std::endl;
	std::cout<<"r_elbow_X_20"<<cell->m_cell_min[20] <<" " << cell->m_cell_max[20] <<" "<< cell->m_cell_average[20] <<" " <<std::endl;
}

void OctreeSkeleton::computeCellAtributes(Octree* cell)
{
	if(cell->dataIndx.size() > 0)
	{
		int vSize = m_framesData[0].m_values.size();
		cell->m_cell_average.resize(vSize);
		cell->m_cell_min.resize(vSize);
		cell->m_cell_max.resize(vSize);
		for(int j = 0; j < vSize; ++j)
		{
			cell->m_cell_min[j] = 100;
			cell->m_cell_max[j] = -100;
		}

		for(unsigned int j = 0; j < cell->dataIndx.size(); ++j)
		{
			FrameData& data = m_framesData[j];
			for(int h = 0; h < vSize; ++h)
			{
				cell->m_cell_min[h] = std::min(cell->m_cell_min[h], data.m_values[h]);
				cell->m_cell_max[h] = std::max(cell->m_cell_max[h], data.m_values[h]);
				cell->m_cell_average[h] += data.m_values[h];
			}
		}

		for(int j = 0; j < vSize; ++j)
		{
			cell->m_cell_average[j] /= cell->dataIndx.size();
		}
		//debugValue(cell, out);
	}

}

void OctreeSkeleton::computeMinMaxAverage()
{
	std::cout<<std::endl<<"compute Cell attributes starts"<<std::endl;
	std::ofstream out;
	out.open("cell.txt");
	int vSize = m_framesData[0].m_values.size();
	std::vector<Octree*>& octrees = p_tree->p_alltree;
	for(unsigned int i = 0; i < octrees.size(); ++i)
	{
		Octree* cell = octrees[i];
		computeCellAtributes(cell);
	}
	out.close();
	std::cout<<std::endl<<"compute Cell attributes ended"<<std::endl;
}


void OctreeSkeleton::computeMinMaxAverageByDepth(int depth)
{
	std::cout<<std::endl<<"compute Cell attributes starts"<<std::endl;
	std::ofstream out;
	out.open("cell.txt");
	int vSize = m_framesData[0].m_values.size();
	std::vector<Octree*>& octrees = p_tree->p_alltree;
	//Octree* cell = p_tree;
	//computeCellAtributes(cell);
	std::queue<Octree*> cells;
	cells.push(p_tree);
	while(!cells.empty())
	{
		Octree* cell = cells.front();
		computeCellAtributes(cell);
		if(cell->dataIndx.size() > 0)
		{
			//debugValue(cell);
			{
					out <<std::endl<<cell->m_index  <<" "<<cell->dataIndx.size() <<std::endl;
					out<<"r_elbow_Z_18"<<cell->m_cell_min[18] <<" " << cell->m_cell_max[18] <<" "<< cell->m_cell_average[18] <<" " <<std::endl;
					out<<"r_elbow_Y_19"<<cell->m_cell_min[19] <<" " << cell->m_cell_max[19] <<" "<< cell->m_cell_average[19] <<" " <<std::endl;
					out<<"r_elbow_X_20"<<cell->m_cell_min[20] <<" " << cell->m_cell_max[20] <<" "<< cell->m_cell_average[20] <<" " <<std::endl;
			}
		}
		
		if(cell->children[0] != NULL)
		{
			for(unsigned int i = 0; i < 8; ++i)
			{
				Octree* cell0 = cell->children[i];
				if(! (cell0->m_level > depth))
				{
					cells.push(cell0);
				}
			}
		}
		cells.pop();
	}
	out.close();
	std::cout<<std::endl<<"compute Cell attributes ended"<<std::endl;
}