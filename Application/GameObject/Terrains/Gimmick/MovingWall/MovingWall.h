#pragma once
#include"../../TerrainsBase.h"
#include"../MaguneFloorBase/MaguneFloorBase.h"

class Player;

class MovingWall :public MagneFloorBase
{
public:
	MovingWall(){}
	~MovingWall(){}

	enum class Size
	{
		Small,
		Big
	};

	void Init()override;

	void Update()override;

	void PostUpdate()override;

	void DrawImGui()override;

	ObjectType GetObjType()override { return m_ObjType; }

	// モデルの詳細なサイズを決める関数
	void SetModelSize(const float _size) { m_modelSize = _size; }

	// モデルのサイズを決める関数
	void SetSize(const Size& _size);

	// 回転角度設定関数
	void SetRotAngle(const float _angle = 61.3f) { m_angle = _angle; }

	static const float k_moveSpeed;	// 移動速度

	static const float k_adjustAdValue;	// 斥力の補正値

	static const float k_repulsionMoveSpeed;	// 斥力処理の時の動く速さ

	static const float k_colisionGuard;	// 自分同士当たり判定するのを防ぐ

private:

	// 当たった時の処理
	void OnHit(Math::Vector3 _pos, UINT _maguneForce, Size _size);
	void OnHit([[maybe_unused]] Math::Vector3 _pos);

	// 反発、斥力の処理の時のプレイヤーの処理
	void PlayerReaction()override;

	enum class ColisionOption
	{
		DifferentForce,
		SameForce,
		WallDistance
	};

	void ColisionCheck(const float _radius, ColisionOption _option, Size _size);

	// モデルのサイズ
	float m_modelSize = 5.0f;

	// 動く速さ
	float m_moveSpeed = 0.5f;

	// 回転角度
	float m_angle = 61.3f;

	// 最初の座標
	Math::Vector3 m_firstPos;

private:

	class BaseSize
	{
	public:
		virtual ~BaseSize() {}

		virtual void Enter([[maybe_unused]] MovingWall& owner) {}
		virtual void Update([[maybe_unused]] MovingWall& owner) {}
		virtual void PostUpdate([[maybe_unused]] MovingWall& owner) {}
		virtual void Exit([[maybe_unused]] MovingWall& owner) {}
	};

	class BigSize : public BaseSize
	{
	public:
		~BigSize(){}
		void Enter(MovingWall& owner);
		void Update(MovingWall& owner);
		void PostUpdate(MovingWall& owner);
		void Exit([[maybe_unused]] MovingWall& owner) {}

		static const float k_sameForceBigColisionRadius;	// 同じ磁極同士の当たり判定の大きさ

		static const float k_differentForceBigColisionRadius;	// 違う磁極同士の当たり判定の大きさ

		static const float k_wallDistanceColisionRadius;	// オブジェクト同士が近づきすぎていないか判定する大きさ
	};

	class SmallSize : public BaseSize
	{
	public:
		~SmallSize(){}
		void Enter(MovingWall& owner);
		void Update([[maybe_unused]] MovingWall& owner) {}
		void PostUpdate(MovingWall& owner);
		void Exit([[maybe_unused]] MovingWall& owner) {}

		static const float k_sameForceSmallColisionRadius;	// 同じ磁極同士の当たり判定の大きさ

		static const float k_differentForceSmallColisionRadius;	// 違う磁極同士の当たり判定の大きさ

	private:

		enum class State
		{
			Gravity,
			Repulsion,
			NotState
		};

		// 今の状態管理変数
		State m_nowState = State::NotState;
		// 移動回数
		int m_moveCnt = 0;
	};

	// モードを決める関数
	void SetSize(const std::shared_ptr<BaseSize>& Size);
	std::shared_ptr<BaseSize> m_Size = nullptr;
};