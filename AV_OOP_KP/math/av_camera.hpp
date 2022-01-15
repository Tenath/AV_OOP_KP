#ifndef AV_CAMERA_HPP
#define AV_CAMERA_HPP

#include "av_vector_t.hpp"
#include "av_polar.hpp"
#include "av_matrix_t.hpp"
#include <cmath>

// (c) 2014 Andrei Veeremaa  

namespace av {

// Some required vector ops, move these elsewhere.
inline Vector2f NormalizeVec2(Vector2f v)
{
	float len=sqrt(v.X()*v.X()+v.Y()*v.Y());
	return (len==0.0f ? v : Vector2f(v.X()/len,v.Y()/len));
}

inline Vector3f NormalizeVec3(Vector3f v)
{
	float len=sqrt(v.X()*v.X()+v.Y()*v.Y()+v.Z()*v.Z());
	return (len==0.0f ? v : Vector3f(v.X()/len,v.Y()/len,v.Z()/len));
}

inline Vector3f CrossProductVec3(Vector3f v1,Vector3f v2)
{
	Vector3f res;
	res.X()=(v1.Y()*v2.Z())-(v1.Z()*v2.Y());
	res.Y()=(v1.Z()*v2.X())-(v1.X()*v2.Z());
	res.Z()=(v1.X()*v2.Y())-(v1.Y()*v2.X());
	return res;
}

class Camera
{
	private:
	
	av::Vector3f focus; // focus (looked at) point
	av::Polar3f cam_pos_rel; // camera position relative to focus (in spherical coordinates)
	Matrix4f view_matrix;
	
	public:
	
	Vector3f GetFocus() { return focus; }
	Polar3f GetCameraPos() { return cam_pos_rel; }
	void MoveFocus(Vector3f offset) { focus+=offset; }
	void ShiftFocus(Vector3f point) { focus=point; }
	
	void AdvanceFocus(float dist)
	{
		Vector3f campos=cam_pos_rel.ToVector3f();
		Vector2f campos_xy=NormalizeVec2({campos.X(),campos.Y()});
		
		focus.X()+=dist*campos_xy.X();
		focus.Y()+=dist*campos_xy.Y();
	}
	
	void SidestepFocus(float dist)
	{
		Vector3f campos=CrossProductVec3(cam_pos_rel.ToVector3f(),{0.0f,0.0f,1.0f});
		Vector2f campos_xy=NormalizeVec2({campos.X(),campos.Y()});
		focus.X()+=dist*campos_xy.X();
		focus.Y()+=dist*campos_xy.Y();
	}
	
	// Camera relative movement
	void CamRotateVertical(float angle) { cam_pos_rel.SetThetaRad(cam_pos_rel.Theta()+angle); }
	void CamRotateHorizontal(float angle) {	cam_pos_rel.SetPhiRad(cam_pos_rel.Phi()+angle); }
	void CamChangeDistance(float dist) { cam_pos_rel.SetRadius(cam_pos_rel.Radius()+dist); }
	
	// Camera absolute shift
	void CamSetVertical(float angle) { cam_pos_rel.SetThetaRad(angle); }
	void CamSetHorizontal(float angle) { cam_pos_rel.SetPhiRad(angle); }
	void CamSetDistance(float dist) { cam_pos_rel.SetRadius(dist); }
	
	Vector3f GetCameraPosVector() { return cam_pos_rel.ToVector3f(); }
	
	Matrix4f CalcTransformMatrix()
	{
		Vector3f UpVec(0.0f, 1.0f, 0.0f);
		
		Vector3f rfocus(focus.X(), focus.Z(), focus.Y());
		Vector3f rcam_pos=cam_pos_rel.ToVector3f();
		float tempf=rcam_pos.Y();
		rcam_pos.Y()=rcam_pos.Z();
		rcam_pos.Z()=tempf;
		Vector3f cam_pos=rfocus+rcam_pos;
		
		Vector3f zaxis = NormalizeVec3(cam_pos - rfocus);
		Vector3f xaxis = NormalizeVec3(CrossProductVec3(UpVec,zaxis));
		Vector3f yaxis = CrossProductVec3(zaxis,xaxis);
		
		Matrix4f orientation;
		orientation.Set(0,0,xaxis.X()); orientation.Set(1,0,yaxis.X()); orientation.Set(2,0,zaxis.X());
		orientation.Set(0,1,xaxis.Y()); orientation.Set(1,1,yaxis.Y()); orientation.Set(2,1,zaxis.Y());
		orientation.Set(0,2,xaxis.Z()); orientation.Set(1,2,yaxis.Z()); orientation.Set(2,2,zaxis.Z());
		orientation.Set(3,3,1.0f);
		
		Matrix4f translation=DiagonalMatrix<float,4>(1.0f);
		translation.Set(0,3,-cam_pos.X());
		translation.Set(1,3,-cam_pos.Y());
		translation.Set(2,3,-cam_pos.Z());
		
		return (orientation*translation);
	}

	void UpdateViewMatrix()
	{
		view_matrix = CalcTransformMatrix();
	}

	Matrix4f& GetViewMatrix() { return view_matrix; }
};

} // namespace av

#endif