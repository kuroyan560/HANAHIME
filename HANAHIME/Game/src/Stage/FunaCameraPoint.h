#pragma once
#include"Common/Vec.h"
class FunaCameraPoint
{
public:
	float m_radius = 0.0f;
	KuroEngine::Vec3<float>m_pos = { 0,0,0 };
	int m_camMode = 0;

	FunaCameraPoint(float arg_radius, KuroEngine::Vec3<float>arg_pos, int arg_camMode)
		:m_radius(arg_radius), m_pos(arg_pos), m_camMode(arg_camMode) {}
};
