#pragma once
#include"../MagneFloorBase/MagneFloorBase.h"

class ChangeFloor : public MagneFloorBase
{
public:
	ChangeFloor(){}
	~ChangeFloor()override{}

	void Init()override;

	void Update()override;

	void PostUpdate()override;

	void PlayerReaction()override;

	void DrawBright()override;

	void DrawImGui()override;

	ObjectType GetObjType()override { return ObjectType::ChangeFloor; }

	// 回転角度調整関数
	void SetAngle(const float _angle) { m_angle = _angle; }

	// クールタイムの限界値調整関数
	void SetIntervalMax(const float _interval) { m_intervalMax = _interval; }

	//================================================

	static const float m_updateArea;	// プレイヤー影響を及ぼす範囲

	static const float k_adsorptionPower;	// 斥力の大きさ

	static const float k_decreaseInterval;	// クールタイムの減算量

	static const float k_modelScale;	// モデルの大きさ

	static const float k_colisionRadius;	// 当たり判定の大きさ

	static const float k_floatingSpeed;	// 浮遊速度

	static const float k_floatingAmplitude;	// 浮遊時の振れ幅

	static const int k_colliderNum = 3; // 当たり判定用の球の数

	static const int k_coolTimeLength = 300;	// クールタイムの長さ

	//================================================

private:

	// プレイヤーが影響を受ける範囲内にいるかどうか判断する関数
	void JugmentPlayer();

	// 纏っている時局を切り替える関数
	void ChangeForce();

	// クールタイムの最大
	float m_intervalMax = 0.0f;

	// クールタイムカウンタ用変数
	float m_changeInterval = m_intervalMax;

	// 回転角度
	float m_angle = 0.0f;

	// 加算値
	float m_addAmount = 0;

	// 加算判断フラグ
	bool m_addFlg = false;

	// 斥力の大きさ
	float m_adPow = 0.f;

	// 斥力の補正値
	float m_adjustAdValue = 0.0f;

	// クールタイム
	int m_coolTime = 0;

	// 表示色
	Math::Color m_color;
};