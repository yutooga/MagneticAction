#pragma once
#include"../../Terrains/TerrainsBase.h"

class ElectricCurrent : public TerrainsBase
{
public:
	ElectricCurrent(){}
	~ElectricCurrent()override{}

	enum class Destination
	{
		StopOver,
		End
	};

	enum class TargetObject
	{
		WoodenLift,
		CopperDoor,
		Gate,
		None
	};

	void Init()override;

	void Update()override;

	// 開始地点
	void SetStartPos(const Math::Vector3& _pos) { m_startPos = _pos; }

	// 中継地点
	void SetStopoverPos(const Math::Vector3& _pos) { m_stopoverPos = _pos; }

	// 終着地点
	void SetEndPos(const Math::Vector3& _pos) { m_endPos = _pos; }

	// 目的地設定関数
	void SetDestination(const Destination& _destination);

	// 対象のオブジェクトを設定する関数
	void SetTargetObject(const TargetObject& _obj) { m_target = _obj; }

	void DrawLit()override{}

	void DrawBright()override{}

	static const float k_moveSpeed;	// 移動速度
	static const float k_roopDistance;	// エフェクトが開始地点に戻るタイミング
	static const float k_changeDistance;	// エフェクトの目的地を変えるタイミング
	static const float k_effectSize;	// エフェクトのサイズ
	static const float k_effectSpeed;	// エフェクトの再生スピード
	static const int k_playCntMax = 5;	// 再生間隔

private:

	// エフェクトを再生するかどうか判断する処理
	bool JugmentEffect();

	// エフェクトのでる座標
	Math::Vector3 m_startPos;

	// エフェクトの中継地点
	Math::Vector3 m_stopoverPos;

	// エフェクトの終着地点
	Math::Vector3 m_endPos;

	// うごく速さ
	float m_moveSpeed = 1.0f;

	int m_playCnt = 0;

private:
	class DestinationBase
	{
	public:
		virtual ~DestinationBase() {}

		virtual void Enter([[maybe_unused]] ElectricCurrent& owner) {}
		virtual void Update([[maybe_unused]] ElectricCurrent& owner) {}
		virtual void PostUpdate([[maybe_unused]] ElectricCurrent& owner) {}
		virtual void Exit([[maybe_unused]] ElectricCurrent& owner) {}
	};

	// 目的地が中継地点の時
	class DestinationForStopover : public DestinationBase
	{
	public:
		void Enter([[maybe_unused]] ElectricCurrent& owner) {}
		void Update(ElectricCurrent& owner);
		void PostUpdate([[maybe_unused]] ElectricCurrent& owner) {}
		void Exit([[maybe_unused]] ElectricCurrent& owner) {}
	};

	// 目的地が終点の時
	class DestinationForEnd : public DestinationBase
	{
	public:
		void Enter([[maybe_unused]] ElectricCurrent& owner) {}
		void Update(ElectricCurrent& owner);
		void PostUpdate([[maybe_unused]] ElectricCurrent& owner) {}
		void Exit([[maybe_unused]] ElectricCurrent& owner) {}
	};

	// 目的地を切り替える関数
	void SetMoveState(const std::shared_ptr<DestinationBase>& State);

	// 現在の目的地を保持する関数
	std::shared_ptr<DestinationBase> m_nowDestination = nullptr;

	// 対象のオブジェクトを記録する変数
	TargetObject m_target = TargetObject::None;
};