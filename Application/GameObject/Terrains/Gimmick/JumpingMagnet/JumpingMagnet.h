#pragma once

#include"../MaguneFloorBase/MaguneFloorBase.h"

class JumpingMagnet : public MagneFloorBase
{
public:
	JumpingMagnet() {}
	~JumpingMagnet() {}

	void Init()override;

	void Update()override;

	void PostUpdate()override;

	void DrawImGui()override;

	ObjectType GetObjType()override { return ObjectType::JumpingFloor; }

	static const float k_sinCurveAngle;	// サインカーブの角度
	static const float k_addAngle;	// 角度の加算量
	static const float k_colisionRadius;	// 当たり判定の大きさ
	static const float k_colisionCircleNum;	// 当たり判定の球の個数

	enum class ColisionAdjustValue
	{
		TopCircle = 120,
		CenterCircle = 70,
		UnderCircle = 30
	};

	enum class ColisionCircle
	{
		Top,
		Center,
		Under
	};

private:

	// 反発状態処理のエリアにプレイヤーが入っているか判断するフラグ
	void JudgmentPlayer();

	// プレイヤーが受ける影響を書く処理
	void PlayerReaction()override;

	// プレイヤーの浮遊処理
	void FloatingPlayer(const std::weak_ptr<KdGameObject>& _obj);

	// 影響を与える範囲内にいるかどうか判断する関数
	void MaguneScope()override;

	// モデルのサイズ
	float m_modelSize = 8.4f;

	// 加算値
	float m_addAmount = 0;

	// 斥力の処理の補正値
	float m_adjustAdValue = 25.0f;

	// 加算判断フラグ
	bool m_addFlg = false;
};