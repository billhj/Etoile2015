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

	class AnimationManager
	{
		AnimationManager(){}
	public:
		static AnimationManager* getInstance()
		{
			static AnimationManager manager;
			return &manager;
		}
		
		virtual ~AnimationManager(){}
		virtual void clear()
		{
			for(unsigned int i = 0; i < m_animatorList.size(); ++i)
			{
				delete m_animatorList[i];
			}
			m_animatorList.clear();
		}
		
		void addIntoAnimatorList(Animator* animator)
		{
			if(m_animatorList.size() > 0)
			{
				int offset = sizeof(animator - m_animatorList[0]) / sizeof(Animator);
				if(animator == m_animatorList[offset - 1])
				{
					return;
				}
			}
			m_animatorList.push_back(animator);
		}

		void removeFromObjectAnimatorList(Animator* animator)
		{
			if(m_animatorList.size() > 0)
			{
				int offset = sizeof(animator - m_animatorList[0]) / sizeof(Animator);
				if(animator == m_animatorList[offset])
				{
					m_animatorList.erase(m_animatorList.begin()+offset);
				}
			}
		}

		void update()
		{
			for(unsigned int i = 0; i < m_animatorList.size(); ++i)
			{
				m_animatorList[i] ->update();
			}
		}

		void update(int idx)
		{
			for(unsigned int i = 0; i < m_animatorList.size(); ++i)
			{
				m_animatorList[i] ->update(idx);
			}
		}

	protected:
		std::string m_name;
		std::vector<Animator*> m_animatorList;
	};
}
