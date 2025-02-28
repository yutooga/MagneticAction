#pragma once
#include"../Floor/MagneFloorBase/MagneFloorBase.h"

class Cage : public MagneFloorBase
{
public:
	Cage(){}
	~Cage()override{}

	void Init()override;

	void Update()override;

	void PostUpdate()override;

	void DrawLit()override;

	void DrawBright()override;

	void DrawImGui()override;

	void OnHit(ObjectType _obj)override;

	ObjectType GetObjType()override{ return ObjectType::Cage; }

	// 回転角度セット関数
	void SetAngle(const float _angle) { m_angle = _angle; }

	static const float k_correctionValue;	// 引っ付く処理の補正値

	static const float k_sameForceColisionRadius;	// 同じ磁極同士の当たり判定の大きさ

	static const float k_differentForceColisionRadius;	// 違う磁極同士の当たり判定の大きさ

private:

	// 当たり判定のオプション
	enum class ColisionOption
	{
		DifferentForce,
		SameForce
	};

	// HITチェック
	void CheckColision(const float _radius, const ColisionOption _option);

	// 当たられたときの処理
	void OnHit(Math::Vector3 _pos, UINT _maguneForce);

	// モデルの回転角度
	float m_angle = 0.0f;

	// モデルの移動スピード
	float m_moveSpeed = 0.0f;

	// モデルのサイズ
	float m_modelSize = 0.f;

	// 最初の座標
	Math::Vector3 m_firstPos;

	// 表示色
	Math::Color m_color;

	Math::Color m_brightColor;
};