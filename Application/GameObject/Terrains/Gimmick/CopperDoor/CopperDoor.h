#pragma once
#include"../../../Terrains/TerrainsBase.h"

class CopperDoor : public TerrainsBase
{
public:
	CopperDoor(){}
	~CopperDoor()override{}

	void Init()override;

	void Update()override;

	void DrawImGui()override;

	void DrawBright()override {}

private:

	// モデルのサイズ
	float m_modelSize = 15.0f;

	// 回転角度
	float m_rotAngle = 90.f;

	// 更新角度
	float m_angle = 0.0f;

	// 移動スピード
	float m_moveSpeed = 1.f;
};