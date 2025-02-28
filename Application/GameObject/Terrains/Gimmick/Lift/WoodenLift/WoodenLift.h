#pragma once
#include"../../../../Terrains/TerrainsBase.h"

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

	static const float k_distance;	// プレイヤーとリフトの距離

private:

	// 範囲内にプレイヤーがいるかどうか判断する処理
	void JudgementPlayer();

	// モデルのサイズ
	float m_modelSize = 0.f;

	// 回転角度
	float m_angle = 0.0f;

	// 移動スピード
	float m_moveSpeed = 0.f;
};