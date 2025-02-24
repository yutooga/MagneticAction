#pragma once
#include "../../TerrainsBase.h"

class Coil : public TerrainsBase
{
public:
	Coil(){}
	~Coil()override{}

	void Init()override;

	void Update()override;

	void PostUpdate()override;

	void DrawImGui()override;

	void DrawBright()override {}

	static const float k_ColisionRadius;	// 当たり判定の球の半径

	static const float k_ColisionAdjustValueY;	// 当たり判定の球のY座標の補正値

	void SetRotAngle(const float _angle) { m_rotAngle = _angle; }	// 回転角度の設定関数

private:
	
	// プレイヤーが一定範囲内いるかどうかの判定
	void JudgmentMove();

	// モデルの大きさ
	float m_modelSize = 26.0f;

	// モデルの回転角度
	float m_rotAngle = 0.0f;
};