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

	ObjectType GetObjType()override { return ObjectType::CopperDoor; }

private:

	// モデルのサイズ
	float m_modelSize = 0.0f;

	// 回転角度
	float m_rotAngle = 0.f;

	// 更新角度
	float m_angle = 0.0f;

	// 移動スピード
	float m_moveSpeed = 0.f;
};