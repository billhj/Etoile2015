/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Camera.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "Entity.h"
#include "math/MathHead.h"

namespace Etoile
{
	enum CameraType {PERSPECTIVE, ORTHOGRAPHIC};
	class ModelTransform;
	class CameraManipulator;
	class Frustum;
	class Camera : public Entity
	{
		int m_screenWidth, m_screenHeight;  // size of the window, in pixels
		float m_fieldOfView; // in radians
		float m_nearPlane;
		float m_farPlane;

		Matrix4f m_modelviewMatrix;
		Matrix4f m_projectionMatrix;
		ModelTransform* p_transform;
		CameraManipulator* p_manipulator;
		Frustum* p_frustum;
		CameraType m_type;
		Vec3f m_pivot;
	public:
		Camera(const Vec3f& position, const Quaternionf& orientation, const std::string& name = "");
		Camera(const Vec3f& target, const Vec3f& up, const Vec3f& position, const std::string& name = "");
		Camera(const std::string& name = "");
		void reset();
		CameraType type();
		void setType(CameraType type);
		float getAspectRatio() const;
		void setZNearPlane(float zNear);
		float getZNearPlane() const;
		void setZFarPlane(float zFar);
		float getZFarPlane() const;
		void setFieldOfView(float fov);
		float getFieldOfView() const;

		void setPosition(const Vec3f& position);
		Vec3f getPosition();
		void setOrientation(const Quaternionf& q);
		Quaternionf getOrientation();

		int getWidth() const;
		void setWidth(int screenWidth);
		int getHeight() const;
		void setHeight(int m_screenHeight);

		void setTarget(const Vec3f& target);
		Vec3f getTarget(){return m_pivot;}
		void setUpVector(const Vec3f& up);
		Vec3f getUpVector() const;
		void setViewDirection(const Vec3f& direction);
		Vec3f getViewDirection() const;

		void setPerspective(int width = 800, int height = 600, float near = 0.1f, float far = 10000.0f, float fieldOfView =  M_PI / 4.0);
		void setOrthogonal(int width = 800, int height = 600, float near = 0.1f, float far = 10000.0f);

		Matrix4f getModelViewMatrix() const;
		Matrix4f getProjectionMatrix() const;
		float* getGLModelViewMatrix() const;
		float* getGLProjectionMatrix() const;

		void setupCameraOrientation(const Vec3f& target, const Vec3f& upVector, const Vec3f& position);
		void computeTransformationMatrix();
		void computeProjectionMatrix();
		void computeModelViewMatrix();

		ModelTransform* getTransform(){return p_transform;}
		CameraManipulator* getManipulator(){return p_manipulator;}
		void setManipulator(CameraManipulator* manipulator);
		void setFrustum(Frustum* frustum){p_frustum = frustum;}
		Frustum* getFrustum(){return p_frustum;}
		void updateFrustum();

		float sceneRadius();
	};


	class FrontViewCamera: public Camera
	{
	public:
		FrontViewCamera(const Vec3f& position);
	};

	class LeftViewCamera: public Camera
	{
	public:
		LeftViewCamera(const Vec3f& position);
	};

	class TopViewCamera: public Camera
	{
	public:
		TopViewCamera(const Vec3f& position);
	};
}