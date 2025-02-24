#pragma once
#include "../3DObjBase.h"

class Magnet : public _3DObjectBase
{
public:
	Magnet(){}
	~Magnet()override {}

	void Init()override;

	void Update()override;

	void GenerateDepthMapFromLight()override;

	static const float k_addAmount;	// 角度の加算量

private:

	//回転角度
	float m_angle = 0.0f;
};