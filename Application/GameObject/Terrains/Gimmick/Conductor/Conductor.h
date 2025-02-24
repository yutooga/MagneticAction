#pragma once
#include"../../TerrainsBase.h"

class Conductor : public TerrainsBase
{
public:
	Conductor(){}
	~Conductor()override{}

	void Init()override;

	void Update()override;

	void DrawImGui()override;

	void DrawBright()override {}

	// 回転角度セット関数
	void SetAngle(const float _angle) { m_angle = _angle; }

	// モデルの大きさセット関数
	void SetModelSize(const float _size) { m_modelSize = _size; }

private:

	// モデルの大きさ
	float m_modelSize = 1.0f;

	// 回転角度
	float m_angle = 0.0f;
};