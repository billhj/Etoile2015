/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file MaterialManager.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "ResourceManager.h"

namespace Etoile
{
	class Material;
	class MaterialManager : public ResourceManager<Material>
	{
	private:
		MaterialManager();
	public:
		static MaterialManager* getInstance()
		{
			static MaterialManager manager;
			return &manager;
		}

		unsigned int addMaterial(Material* t);
		Material* getMaterialByIndex(unsigned int idx);
		void addMaterials(std::vector<Material*> resources);
	};

}
