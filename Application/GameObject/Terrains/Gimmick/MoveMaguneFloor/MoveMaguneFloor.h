#pragma once
#include"../MaguneFloorBase/MaguneFloorBase.h"

class MoveMaguneFloor : public MagneFloorBase
{
public:
	MoveMaguneFloor(){}
	~MoveMaguneFloor()override{}

	// 動きの状態
	enum class MoveState
	{
		nomal,
		special,
		none
	};

	void Init()override;

	void Update()override;

	void PostUpdate()override;

	void DrawBright()override;

	void DrawImGui()override;

	void SetMaguneForce(UINT _force) { m_maguneForce = _force; }

	void OnHit(ObjectType _obj)override;

	// プレイヤーの処理
	void PlayerReaction()override;

	// 移動回数の上限値を設定する関数
	void SetMoveMax(int _movemax) { m_moveMax = _movemax; }

	// 状態を指定する関数
	void SetMaguneState(MoveState state);

	enum class ColisionCircle
	{
		Top,
		Center,
		Under
	};

	//=============================定数関係==============================
	
	static const float k_specialMoveSpeed;	// 動く速さ
	static const float k_normalMoveSpeed;	// 動く速さ

	static const float k_addRotAngleAmount;	// 角度の加算量

	static const float k_initialOppoPowerValue;	// 反発力の初期値

	static const float k_correctionValueY;	// モデルのサイズの補正値

	static const float k_oppositionLimit;	// 反発の継続時間限界値

	static const float k_adsorptionPower;	// 斥力の大きさ

	//===================================================================

private:
	// モデルのサイズ
	float m_modelSize = 3.5f;

	// 最大移動回数
	int m_moveMax = 0;

	// 斥力の大きさ
	float m_adPow = 0.4f;

	// 斥力の補正値
	float m_adjustAdValueZ = 10.5f;
	float m_adjustAdValueY = 3.5f;

	// 磁力を帯びてない物体用の行列
	Math::Matrix m_noPowerObjMat;

	// サインカーブ用の角度
	float m_angle = 0.0f;

	// プレイヤーに行動制限をかけたかどうか判断するフラグ
	bool m_moveFlg = false;

	// 以前の座標
	Math::Vector3 m_previousPosition;

	// 初期状態の座標
	float m_firstPosZ = 0.0f;

	// モデルのサイズの補正値
	float CorrectionValueX = 10.0f;

	float m_circleRadius = 23;
	float m_circleDistance = 37;

	MoveState m_nowMoveState = MoveState::none;

private:
	class MoveStateBase
	{
	public:
		virtual ~MoveStateBase(){}

		virtual void Enter ([[maybe_unused]] MoveMaguneFloor& owner){}
		virtual void Update([[maybe_unused]] MoveMaguneFloor& owner) {}
		virtual void PostUpdate([[maybe_unused]] MoveMaguneFloor& owner) {}
		virtual void Exit([[maybe_unused]] MoveMaguneFloor& owner) {}
	};

	// 通常状態
	class NomalMove : public MoveStateBase
	{
	public:
		~NomalMove() {}

		void Enter(MoveMaguneFloor& owner);
		void Update(MoveMaguneFloor& owner);
		void PostUpdate([[maybe_unused]] MoveMaguneFloor& owner){}
		void Exit([[maybe_unused]] MoveMaguneFloor& owner){}
	};

	// 特殊な動き
	class SpecialMove : public MoveStateBase
	{
	public:
		~SpecialMove(){}

		void Enter(MoveMaguneFloor& owner);
		void Update(MoveMaguneFloor& owner);
		void PostUpdate(MoveMaguneFloor& owner);
		void Exit([[maybe_unused]] MoveMaguneFloor& owner) {}
	};
	void SetMoveState(std::shared_ptr<MoveStateBase> State);
	std::shared_ptr<MoveStateBase> m_moveState = nullptr;
};