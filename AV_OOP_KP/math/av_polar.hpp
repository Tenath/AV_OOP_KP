#ifndef AV_POLAR_HPP
#define AV_POLAR_HPP

#include "av_vector_t.hpp"
#include <cmath>

// 3-dimensional Polar coordinate class
// Defined in the context of coordinate uniqueness, so that
// radius >= 0
// 0 <= theta <= Pi
// 0 <= phi <= 2Pi
// Angles are stored in radians

namespace av {

class Polar3f
{
	private:
	
	float m_theta;
	float m_phi;
	float m_radius;
	
	public:
	
	static constexpr float Eps=0.000001f;
	static constexpr float Pi=3.14159268f;
	
	Polar3f() : m_theta(0.0f), m_phi(0.0f), m_radius(1.0f) {};
	
	explicit Polar3f(float theta, float phi, float radius) : m_theta(theta), m_phi(phi), m_radius(radius) {}
	
	// From cartesian coordinates
	explicit Polar3f(av::Vector3f dec)
	{
		m_radius=sqrt(dec.X()*dec.X()+dec.Y()*dec.Y()+dec.Z()*dec.Z());
		m_theta=(m_radius!=0.0f ? acos(dec.Z()/m_radius) : 0.0f);
		m_phi=(dec.X()!=0.0f ? atan(dec.Y()/dec.X()) : 0.0f);
	};
	
	Polar3f& operator=(av::Vector3f dec)
	{
		m_radius=sqrt(dec.X()*dec.X()+dec.Y()*dec.Y()+dec.Z()*dec.Z());
		m_theta=(m_radius!=0.0f ? acos(dec.Z()/m_radius) : 0.0f);
		m_phi=(dec.X()!=0.0f ? atan(dec.Y()/dec.X()) : 0.0f);
		return *this;
	}
	
	av::Vector3f ToVector3f() const
	{
		av::Vector3f result;
		result.X()=m_radius*sin(m_theta)*cos(m_phi);
		result.Y()=m_radius*sin(m_theta)*sin(m_phi);
		result.Z()=m_radius*cos(m_theta);
		return result;
	}
	
	void SetThetaDeg(float deg)
	{
		// Truncate the excess turns
		if(fabs(deg)>=90.0f) //deg=fmod(deg,90.0f);
			deg=(deg<0.0f ? -90.0f : 90.0f);
		m_theta=(deg/180.0f)*Pi;
	}
	
	void SetThetaRad(float rad)
	{
		if(rad>0.0f && rad<Pi)
		{
			//printf("Theta: %f\n", rad);
			m_theta=rad;
		}
	}
	
	// Increment theta - optional, convenience
	// Actually p.SetThetaRad(p.Theta()+Pi/2) sounds just as great.
	//void IncThetaDeg(float deg);
	//void IncThetaRad(float rad );
	
	void SetPhiDeg(float deg)
	{
		if(fabs(deg)>=360.0f) deg=fmod(deg,360.0f);
		if(deg<0.0f) deg=360.0f-deg;
		m_phi=(deg/180.0f)*Pi;
	}
	
	void SetPhiRad(float rad)
	{
		if(fabs(rad)>=2.0f*Pi) rad=fmod(rad,2.0f*Pi);
		if(rad<0.0f) rad=2.0f*Pi+rad;
		m_phi=rad;
	}
	
	void SetRadius(float r) 
	{
		if(r<0.0f) r=-r;
		m_radius=r;
	}
	
	float Theta() const { return m_theta; }
	float Phi() const { return m_phi; }
	float Radius() const { return m_radius; }
};

} // namespace av

#endif