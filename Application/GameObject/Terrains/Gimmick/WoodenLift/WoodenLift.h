#pragma once
#include"../../../Terrains/TerrainsBase.h"

class WoodenLift : public TerrainsBase
{
public:
	WoodenLift(){}
	~WoodenLift()override{}

	void Init()override;

	void Update()override;

	void PostUpdate()override;

	void DrawImGui()override;

	void DrawBright()override {}

	ObjectType GetObjType() { return ObjectType::WoodenLift; }

	static const float k_colisionRadius;	// 当たり判定の球の半径

	static const float k_colisionAdjustValueY;	// 当たり判定のY座標の補正値

	static const float k_distance;	// プレイヤーとリフトの距離

private:

	void JudgementPlayer();

	// モデルのサイズ
	float m_modelSize = 7.6f;

	// 回転角度
	float m_angle = 0.0f;

	// 移動スピード
	float m_moveSpeed = 1.5f;
};