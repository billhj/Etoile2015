/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Camera.cpp
* @brief 
* @date 1/2/2011
*/

#include "Camera.h"
/**
* @brief For tracking memory leaks under windows using the crtdbg
*/
#if ( defined( _DEBUG ) || defined( DEBUG ) ) && defined( _MSC_VER )
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif

#include "ModelTransform.h"
#include "CameraManipulator.h"
#include "Frustum.h"

namespace Etoile
{
	
		Camera::Camera(const Vec3f& position, const Quaternionf& orientation, const std::string& name) : Entity(name)
		{
			reset();
			p_transform->setPosition(position);
			p_transform->setOrientation(orientation);
			m_pivot = getViewDirection() * 2 + p_transform->getPosition();
		}

		Camera::Camera(const Vec3f& target, const Vec3f& up, const Vec3f& position, const std::string& name): Entity(name)
		{
			reset();
			this->setupCameraOrientation(target, up, position);
		}

		Camera::Camera(const std::string& name): Entity(name)
		{
			reset();
			m_pivot = getViewDirection() * 2 + p_transform->getPosition();
		}

		void Camera::reset()
		{
			p_transform = new ModelTransform();
			this->setComponent(ComponentType::TRANSFORM_COMPONENT, p_transform);
			m_type = PERSPECTIVE;
			m_fieldOfView = M_PI / 4.0;
			m_nearPlane = 0.1f;
			m_farPlane = 10000.0f;
			m_screenWidth = 800;
			m_screenHeight = 600;
			p_frustum = new FrustumG();
			updateFrustum();
		}

		void Camera::updateFrustum()
		{
			if(p_frustum != NULL)
			{
				p_frustum->setCamInternals(m_fieldOfView, (float)m_screenWidth/(float)m_screenHeight, m_nearPlane, m_farPlane);
				Quaternionf orientation = p_transform->getOrientation();
				p_frustum->setCamDef(p_transform->getPosition(), orientation * Vec3f(0,0,-1), orientation * Vec3f(0,1,0));
			}
		}

		void Camera::setManipulator(CameraManipulator* manipulator)
		{
			manipulator->setCamera(this);
			p_manipulator = manipulator;
			this->setComponent(ComponentType::MANIPULATOR_COMPONENT, p_manipulator);
		}

		CameraType Camera::type(){return m_type;}
		void Camera::setType(CameraType type){m_type = type;}
		float Camera::getAspectRatio() const{return static_cast<float>(m_screenWidth) / static_cast<float>(m_screenHeight);}
		void Camera::setZNearPlane(float zNear){m_nearPlane = zNear;}
		float Camera::getZNearPlane() const{return m_nearPlane;}
		void Camera::setZFarPlane(float zFar){m_farPlane = zFar;}
		float Camera::getZFarPlane() const{return m_farPlane;}
		void Camera::setFieldOfView(float fov){m_fieldOfView = fov;}
		float Camera::getFieldOfView() const{return m_fieldOfView;}

		int Camera::getWidth() const{return m_screenWidth;}
		void Camera::setWidth(int screenWidth){m_screenWidth = screenWidth;}
		int Camera::getHeight() const{return m_screenHeight;}
		void Camera::setHeight(int screenHeight){m_screenHeight = screenHeight;}

		void Camera::setPosition(const Vec3f& position)
		{
			p_transform->setPosition(position);
		}

		Vec3f Camera::getPosition()
		{
			return p_transform->getPosition();
		}


		void Camera::setOrientation(const Quaternionf& q)
		{
			p_transform->setOrientation(q);
		}

		Quaternionf Camera::getOrientation()
		{
			return p_transform->getOrientation();
		}

		void Camera::setTarget(const Vec3f& target)
		{
			setupCameraOrientation(target - p_transform->getPosition(), getUpVector(), p_transform->getPosition());
		}
		void Camera::setUpVector(const Vec3f& up)
		{
			setupCameraOrientation(getViewDirection(), up, p_transform->getPosition());
		}
		Vec3f Camera::getUpVector() const{return p_transform->getOrientation() * Vec3f(0,1,0);}
		void Camera::setViewDirection(const Vec3f& direction)
		{
			m_pivot = direction * 2 + p_transform->getPosition();
			setupCameraOrientation(m_pivot, getUpVector(), p_transform->getPosition());
		}
		Vec3f Camera::getViewDirection() const{return p_transform->getOrientation() * Vec3f(0,0,-1);}

		void Camera::setPerspective(int width, int height, float near, float far, float fieldOfView)
		{
			this->setWidth(width);
			this->setHeight(height);
			this->setZNearPlane(near);
			this->setZFarPlane(far);
			this->setFieldOfView(fieldOfView);
			this->setType(CameraType::PERSPECTIVE);
			computeProjectionMatrix();
		}
		void Camera::setOrthogonal(int width, int height, float near, float far)
		{
			this->setWidth(width);
			this->setHeight(height);
			this->setZNearPlane(near);
			this->setZFarPlane(far);
			this->setType(CameraType::ORTHOGRAPHIC);
			computeProjectionMatrix();
		}

		Matrix4f Camera::getModelViewMatrix() const{return m_modelviewMatrix;}
		Matrix4f Camera::getProjectionMatrix() const{return m_projectionMatrix;}
		float* Camera::getGLModelViewMatrix() const{return m_modelviewMatrix.getGLMatrix();}
		float* Camera::getGLProjectionMatrix() const{return m_projectionMatrix.getGLMatrix();}

		void Camera::setupCameraOrientation(const Vec3f& target, const Vec3f& upVector, const Vec3f& position)
		{
			m_pivot = target;
			p_transform->setPosition(position);
			Vec3f direction = target - position;
			if (direction.length() < 1E-10)
				return;

			Vec3f xAxis = direction.cross3(upVector);
			if (xAxis.length() < 1E-10)
			{
				// target is aligned with upVector, this means a rotation around X axis
				// X axis is then unchanged, let's keep it !
				xAxis = p_transform->getRotation().inverseRotate( Vec3f(1.0, 0.0, 0.0)); //computeGlobalPosition(Vec3f(1.0, 0.0, 0.0));
			}

			Quaternionf q;
			q.setFromRotatedBasis(xAxis, xAxis.cross3(direction), -direction);
			p_transform->setOrientation(q);
		}

		float Camera::sceneRadius()
		{
			return 2;
		}

		void Camera::computeTransformationMatrix()
		{
			computeProjectionMatrix();
			computeModelViewMatrix();
		}

		void Camera::computeProjectionMatrix()
		{
			switch (m_type)
			{
			case CameraType::PERSPECTIVE:
				{
					const float f = 1.0/tan(m_fieldOfView/2.0);
					m_projectionMatrix[0][0]  = f/getAspectRatio();
					m_projectionMatrix[1][1]  = f;
					m_projectionMatrix[2][2] = (m_nearPlane + m_farPlane) / (m_nearPlane - m_farPlane);
					m_projectionMatrix[3][2] = -1.0;
					m_projectionMatrix[2][3] = 2.0 * m_nearPlane * m_farPlane / (m_nearPlane - m_farPlane);
					m_projectionMatrix[3][3] = 0.0;
					// same as gluPerspective( 180.0*fieldOfView()/M_PI, aspectRatio(), zNear(), zFar() );
					break;
				}
			case CameraType::ORTHOGRAPHIC:
				{
					m_projectionMatrix[0][0]  = 1.0/static_cast<float>(m_screenWidth);
					m_projectionMatrix[1][1]  = 1.0/static_cast<float>(m_screenHeight);
					m_projectionMatrix[2][2] = -2.0/(m_farPlane - m_nearPlane);
					m_projectionMatrix[3][2] = 0.0;
					m_projectionMatrix[2][3] = (m_farPlane + m_nearPlane)/(m_nearPlane - m_farPlane);
					m_projectionMatrix[3][3] = 1.0;
					// same as glOrtho( -w, w, -h, h, zNear(), zFar() );
					break;
				}
			}
		}

		void Camera::computeModelViewMatrix()
		{
			const Quaternionf q = p_transform->getOrientation();
			q.getMatrix(m_modelviewMatrix);      
			m_modelviewMatrix.transpose(); //inverse matrix for camera that is modelview for obj
			Vec3f pos = p_transform->getPosition();
			Vec3f t = q.inverseRotate(pos);  // distance in project space  
			m_modelviewMatrix[0][3] = -t[0];
			m_modelviewMatrix[1][3] = -t[1];
			m_modelviewMatrix[2][3] = -t[2];
			m_modelviewMatrix[3][3] = 1.0;
		}

		FrontViewCamera::FrontViewCamera(const Vec3f& position): Camera(Vec3f(0,0,-1),Vec3f(0,1,0),position)
		{
		
		}

		LeftViewCamera::LeftViewCamera(const Vec3f& position): Camera(Vec3f(1,0,0),Vec3f(0,1,0),position)
		{
		
		}

		TopViewCamera::TopViewCamera(const Vec3f& position): Camera(Vec3f(0,-1,0),Vec3f(0,0,-1),position)
		{
		
		}
}