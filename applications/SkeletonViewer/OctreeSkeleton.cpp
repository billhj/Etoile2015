#include "OctreeSkeleton.h"
#include <iostream>
#include <fstream>

OctreeSkeleton::OctreeSkeleton(void)
{
	m_leftHand_index = 48;
	m_rightHand_index = 33;
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
}


void OctreeSkeleton::saveCSVFileByFrameIdx(const std::string& filename, std::vector<int> indx)
{
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
}
