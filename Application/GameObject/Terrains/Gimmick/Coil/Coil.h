#pragma once
#include "../../TerrainsBase.h"

class Coil : public TerrainsBase
{
public:
	Coil(){}
	~Coil()override{}

	void Init()override;

	void Update()override;

	void DrawImGui()override;

	void DrawBright()override {}

	void SetRotAngle(const float _angle) { m_rotAngle = _angle; }	// 回転角度の設定関数

private:

	// モデルの大きさ
	float m_modelSize = 0.0f;

	// モデルの回転角度
	float m_rotAngle = 0.0f;
};