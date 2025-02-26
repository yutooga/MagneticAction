#pragma once
#include"../../../Terrains/TerrainsBase.h"

class StoneWall :public TerrainsBase
{
public:
	StoneWall(){}
	~StoneWall()override{}

	void Init()override;

	void Update()override;

	void DrawImGui()override;

	void DrawBright()override {}

	// モデルのサイズセット
	void SetModelSize(const float _size) { m_modelSize = _size; }

	// 回転角度設定関数
	void SetAngle(const float _angle) { m_angle = _angle; }

private:

	// モデルのサイズ
	float m_modelSize = 1.0f;

	// 回転角度
	float m_angle = 0.0f;
};