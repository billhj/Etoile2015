#include "OctreeSkeleton.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <queue>

OctreeSkeleton::OctreeSkeleton(void)
{
	m_dataIsLoaded = false;
	m_leftHand_index = 48;
	m_rightHand_index = 33;
	p_righthandTree = new Octree(Vec3(-0.15, 0.2, 0.15), Vec3(0.25, 0.4, 0.45), true);
}


OctreeSkeleton::~OctreeSkeleton(void)
{
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
		OctreePoint* point = new OctreePoint();
		point->position = Vec3(frame.m_values[m_rightHand_index], frame.m_values[m_rightHand_index + 1], frame.m_values[m_rightHand_index + 2]);
		point->m_frameIdx = i;
		p_righthandTree->insert(point);
		if(i % 100 == 0)
		{
			std::cout<<".";
		}
	}
	std::cout<<std::endl<<"loading data into Octree is finished"<<std::endl;
}

static void debugValue(Octree* cell)
{
	std::cout <<std::endl<<cell->m_index <<std::endl;
	std::cout<<"r_elbow_Z_114"<<cell->m_cell_min[114] <<" " << cell->m_cell_max[114] <<" "<< cell->m_cell_average[114] <<" " <<std::endl;
	std::cout<<"r_elbow_Y_115"<<cell->m_cell_min[115] <<" " << cell->m_cell_max[115] <<" "<< cell->m_cell_average[115] <<" " <<std::endl;
	std::cout<<"r_elbow_X_116"<<cell->m_cell_min[116] <<" " << cell->m_cell_max[116] <<" "<< cell->m_cell_average[116] <<" " <<std::endl;
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
	std::vector<Octree*>& octrees = p_righthandTree->p_alltree;
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
	std::vector<Octree*>& octrees = p_righthandTree->p_alltree;
	//Octree* cell = p_righthandTree;
	//computeCellAtributes(cell);
	std::queue<Octree*> cells;
	cells.push(p_righthandTree);
	while(!cells.empty())
	{
		Octree* cell = cells.front();
		computeCellAtributes(cell);
		/*if(cell->dataIndx.size() > 0)
		{
			debugValue(cell);
			{
					out <<std::endl<<cell->m_index <<std::endl;
					out<<"r_elbow_Z_114"<<cell->m_cell_min[114] <<" " << cell->m_cell_max[114] <<" "<< cell->m_cell_average[114] <<" " <<std::endl;
					out<<"r_elbow_Y_115"<<cell->m_cell_min[115] <<" " << cell->m_cell_max[115] <<" "<< cell->m_cell_average[115] <<" " <<std::endl;
					out<<"r_elbow_X_116"<<cell->m_cell_min[116] <<" " << cell->m_cell_max[116] <<" "<< cell->m_cell_average[116] <<" " <<std::endl;
			}
		}*/
		
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