#include "Frustum.h"

namespace Etoile
{
	void Frustum::setCamInternals(float angle, float ratio, float nearD, float farD)
	{
		// store the information
		m_ratio = ratio;
		m_angle = angle;
		m_nearD = nearD;
		m_farD = farD;

		// compute width and height of the near and far plane sections
		m_tang = (float)tan(ANG2RAD * angle * 0.5) ;
		m_nh = nearD * m_tang;
		m_nw = m_nh * ratio; 
		m_fh = farD  * m_tang;
		m_fw = m_fh * ratio;
	}

	/*
	* camera position, target, up vector
	*/
	void Frustum::setCamDef(Vec3f& p, Vec3f& l, Vec3f& u)
	{

		Vec3f dir,nc,fc,X,Y,Z;

		// compute the Z axis of camera
		// this axis points in the opposite direction from 
		// the looking direction
		Z = p - l;
		Z.normalize();

		// X axis of camera with given "up" vector and Z axis
		X = u.cross3(Z);
		X.normalize();

		// the real "up" vector is the cross product of Z and X
		Y = Z.cross3(X);

		// compute the centers of the near and far planes
		nc = p - Z * m_nearD;
		fc = p - Z * m_farD;

		// compute the 4 corners of the frustum on the near plane
		m_ntl = nc + Y * m_nh - X * m_nw;
		m_ntr = nc + Y * m_nh + X * m_nw;
		m_nbl = nc - Y * m_nh - X * m_nw;
		m_nbr = nc - Y * m_nh + X * m_nw;

		// compute the 4 corners of the frustum on the far plane
		m_ftl = fc + Y * m_fh - X * m_fw;
		m_ftr = fc + Y * m_fh + X * m_fw;
		m_fbl = fc - Y * m_fh - X * m_fw;
		m_fbr = fc - Y * m_fh + X * m_fw;

		// compute the six planes
		// the function set3Points assumes that the points
		// are given in counter clockwise order
		m_plane[TOP].set3Points(m_ntr,m_ntl,m_ftl);
		m_plane[BOTTOM].set3Points(m_nbl,m_nbr,m_fbr);
		m_plane[LEFT].set3Points(m_ntl,m_nbl,m_fbl);
		m_plane[RIGHT].set3Points(m_nbr,m_ntr,m_fbr);
		m_plane[NEARP].set3Points(m_ntl,m_ntr,m_nbr);
		m_plane[FARP].set3Points(m_ftr,m_ftl,m_fbl);



		/* optimization
		*	pl[NEARP].setNormalAndPoint(-Z,nc);
		pl[FARP].setNormalAndPoint(Z,fc);

		Vec3 aux,normal;

		aux = (nc + Y*nh) - p;
		aux.normalize();
		normal = aux * X;
		pl[TOP].setNormalAndPoint(normal,nc+Y*nh);

		aux = (nc - Y*nh) - p;
		aux.normalize();
		normal = X * aux;
		pl[BOTTOM].setNormalAndPoint(normal,nc-Y*nh);

		aux = (nc - X*nw) - p;
		aux.normalize();
		normal = aux * Y;
		pl[LEFT].setNormalAndPoint(normal,nc-X*nw);

		aux = (nc + X*nw) - p;
		aux.normalize();
		normal = Y * aux;
		pl[RIGHT].setNormalAndPoint(normal,nc+X*nw);
		*
		*/
	}

	int FrustumG::pointInFrustum(Vec3f& p) {

		int result = INSIDE;
		for(int i=0; i < 6; i++) 
		{
			if (m_plane[i].distance(p) < 0)
				return OUTSIDE;
		}
		return(result);
	}


	int FrustumG::sphereInFrustum(Vec3f& p, float radius) {

		int result = INSIDE;
		float distance;

		for(int i=0; i < 6; i++) 
		{
			distance = m_plane[i].distance(p);
			if (distance < -radius)
				return OUTSIDE;
			else if (distance < radius)
				result =  INTERSECT;
		}
		return(result);

	}

	/*
	* get Positive corner
	*/
	Vec3f getVertexP(AxisAlignedBoundingBoxf box, Vec3f &normal) {
		Vec3f res = box.minimum();
		if (normal.x() > 0)
			res[0] = box.maximum().x();

		if (normal.y() > 0)
			res[1] = box.maximum().y();

		if (normal.z() > 0)
			res[2] = box.maximum().z();

		return(res);
	}


	/*
	* get negative corner
	*/
	Vec3f getVertexN(AxisAlignedBoundingBoxf box, Vec3f &normal) {

		Vec3f res = box.minimum();

		if (normal.x() < 0)
			res[0] = box.maximum().x();

		if (normal.y() < 0)
			res[1] = box.maximum().y();

		if (normal.z() < 0)
			res[2] = box.maximum().z();

		return(res);
	}

	int FrustumG::boxInFrustum(AxisAlignedBoundingBoxf& b) {

		int result = INSIDE;
		for(int i=0; i < 6; i++) {

			if (m_plane[i].distance(getVertexP(b, m_plane[i].m_normal)) < 0)
				return OUTSIDE;
			else if (m_plane[i].distance(getVertexN(b, m_plane[i].m_normal)) < 0)
				result =  INTERSECT;
		}
		return(result);

		/***
		int result = INSIDE, out,in;

		// for each plane do ...
		for(int i=0; i < 6; i++) {

		// reset counters for corners in and out
		out=0;in=0;
		// for each corner of the box do ...
		// get out of the cycle as soon as a box as corners
		// both inside and out of the frustum
		for (int k = 0; k < 8 && (in==0 || out==0); k++) {

		// is the corner outside or inside
		if (pl[i].distance(b.getVertex(k)) < 0)
		out++;
		else
		in++;
		}
		//if all corners are out
		if (!in)
		return (OUTSIDE);
		// if some corners are out and others are in	
		else if (out)
		result = INTERSECT;
		}
		return(result);

		*/

	}
}