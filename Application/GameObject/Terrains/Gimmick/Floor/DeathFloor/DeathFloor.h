#pragma once
#include"../../Floor/MagneFloorBase/MagneFloorBase.h"

class DeathFloor : public MagneFloorBase
{
public:
	DeathFloor(){}
	~DeathFloor()override {}

	enum class MoveState
	{
		nomal,
		change,
		none
	};

	void Init()override;

	void Update()override;

	void PostUpdate()override;

	void DrawImGui()override;

	void OnHit(KdGameObject::ObjectType _obj)override;

	void ChangeMoveState(MoveState state);

	void SetModelSize(const float _size) { m_modelSize = _size; }

	//====================================================================

	static const float k_oppositionLimit;	// 反発の継続時間の限界

	static const float k_oppositionPow;	// 反発の大きさ

	static const float k_correctionValue;	// 引っ付く処理の補正値

	static const int k_intervalMax = 180;	// クールタイムの最大値

	static const float k_addAngleValue;	// 角度の加算量

	static const float k_sameForceColisionRadius;	// 同じ磁極同士の当たり判定の大きさ

	static const float k_differentForceColisionRadius;	// 違う磁極同士の当たり判定の大きさ

	static const float k_opppsitionSpeed;	// 反発の時の移動スピード

	static const float k_adsorptionSpeed;	// 引き寄せあっているときの移動スピード

	static const float k_unitVector;	// 単位ベクトル

	//====================================================================

private:

	// 当たり判定のオプション
	enum class ColisionOption
	{
		DifferentForce,
		SameForce
	};

	// 当たり判定
	void ColisionCheck(const float _radius, const ColisionOption& _option, std::list<std::weak_ptr<KdGameObject>>& _hitList);

	// 一定範囲内にプレイヤーがいるかどうか判定
	void MagneScope()override;

	// 範囲内にいた時のプレイヤーの処理
	void PlayerReaction()override;

	// 当たられた時の判定処理
	void OnHit(Math::Vector3 _pos, UINT _maguneForce,const ObjectType& _type);
	void OnHit(Math::Vector3& _pos);

	// 動く速さ
	float m_moveSpeed = 0.f;

	// モデルの大きさ
	float m_modelSize = 0.0f;

	// オブジェクトの最初のX座標
	float m_firstPosX = 0.0f;

	// 最初の座標
	Math::Vector3 m_firstPos;

	//======================================
	//移動する死の床の変数
	//======================================

	// sinカーブ用の角度保持変数
	float m_moveAngle = 0.0f;

	//移動する前の位置
	Math::Vector3 m_previousPosition;

	float m_length = 0.0f;

	// 角度の振れ幅
	float m_amplitude = 0.0f;

private:

	class MoveStateBase
	{
	public:
		virtual ~MoveStateBase() {}

		virtual void Enter([[maybe_unused]] DeathFloor& owner) {}
		virtual void Update([[maybe_unused]] DeathFloor& owner) {}
		virtual void PostUpdate([[maybe_unused]] DeathFloor& owner) {}
		virtual void Exit([[maybe_unused]] DeathFloor& owner) {}
	};

	class NormalMove : public MoveStateBase
	{
		void Enter([[maybe_unused]] DeathFloor& owner){}
		void Update(DeathFloor& owner);
		void PostUpdate([[maybe_unused]] DeathFloor& owner);
		void Exit([[maybe_unused]] DeathFloor& owner) {}
	};

	class ChangeMove : public MoveStateBase
	{
		void Enter(DeathFloor& owner);
		void Update(DeathFloor& owner);
		void PostUpdate(DeathFloor& owner);
		void Exit([[maybe_unused]] DeathFloor& owner) {}
	};

	// 移動モードの設定関数
	void SetMoveState(std::shared_ptr<MoveStateBase> State);

	// 現在の移動モードを保持する関数
	std::shared_ptr<MoveStateBase> m_moveState = nullptr;

	MoveState m_nowMoveState = MoveState::none;
};