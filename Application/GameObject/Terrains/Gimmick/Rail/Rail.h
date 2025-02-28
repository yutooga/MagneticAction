#pragma once
#include"../../../Terrains/TerrainsBase.h"

class Rail : public TerrainsBase
{
public:
	Rail(){}
	~Rail()override{}

	void Init()override;

	void Update()override;

	void DrawImGui()override;

	void DrawBright()override{}

	void SetRotAngle(const float _angle) { m_angle = _angle; }

private:

	// モデルのサイズ
	float m_modelSize = 0.f;
	
	// 回転角度
	float m_angle = 0.0f;
};