/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file AnimationManager.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <string>
#include <vector>
#include "geometry/ResourceManager.h"
#include "geometry/Component.h"
namespace Etoile
{
	class Animator : public Component
	{
	public:
		Animator():Component(ComponentType::ANIMATION_COMPONENT){}
		virtual void update() = 0;
		virtual void update(int) = 0;
	};

	class AnimationManager : public ResourceManager<Animator>
	{
		AnimationManager(){m_lastFrame = -1;}
	public:
		static AnimationManager* getInstance()
		{
			static AnimationManager manager;
			return &manager;
		}
		
		virtual ~AnimationManager(){}
		
		unsigned int  addIntoAnimatorList(Animator* animator)
		{
			return add(animator);
		}

		void removeFromObjectAnimatorList(Animator* animator)
		{
			remove(animator);
		}

		void update()
		{
			for(unsigned int i = 0; i < m_datas.size(); ++i)
			{
				m_datas[i] ->update();
			}
		}

		void update(int idx)
		{
			if(idx == m_lastFrame) return;
			m_lastFrame = idx;
			for(unsigned int i = 0; i < m_datas.size(); ++i)
			{
				m_datas[i] ->update(idx);
			}
		}

	protected:
		std::string m_name;
		int m_lastFrame;
	};
}
