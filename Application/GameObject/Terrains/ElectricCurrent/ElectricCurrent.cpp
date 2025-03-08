#include "ElectricCurrent.h"
#include"../../Manager/UpdateObjManager/UpdateObjManager.h"
#include"../../Manager/GateManager/GateManager.h"

const float ElectricCurrent::k_moveSpeed = 0.5f;
const float ElectricCurrent::k_roopDistance = 0.1f;
const float ElectricCurrent::k_changeDistance = 0.1f;
const float ElectricCurrent::k_effectSize = 1.0f;
const float ElectricCurrent::k_effectSpeed = 1.0f;

void ElectricCurrent::Init()
{
	m_pos = m_startPos;
}

void ElectricCurrent::Update()
{

	// 移動速度の初期化
	m_moveSpeed = k_moveSpeed;

	if (m_nowDestination)
	{
		m_nowDestination->Update(*this);
	}

	// 対象のオブジェクトが作動しているならエフェクトを更新する
	if(JugmentEffect())
	{
		m_playCnt++;

		//　再生間隔をあける処理
		if (m_playCnt < k_playCntMax)return;

		// エフェクトの更新
		KdEffekseerManager::GetInstance().Play(
			"Thunder3.efkproj", m_pos, m_gimmickData["ElectricCurrent"].value("EffectSize",k_effectSize), 
			m_gimmickData["ElectricCurrent"].value("EffectSpeed", k_effectSpeed), false);

		m_playCnt = 0;
	}
	else
	{
		m_playCnt = 0;
	}
}

void ElectricCurrent::SetDestination(const Destination& _destination)
{
	if (_destination == Destination::End)
	{
		SetMoveState(std::make_shared<ElectricCurrent::DestinationForEnd>());
	}
	else if (_destination == Destination::StopOver)
	{
		SetMoveState(std::make_shared<ElectricCurrent::DestinationForStopover>());
	}
}

bool ElectricCurrent::JugmentEffect()
{
	switch (m_target)
	{
		// 対象のオブジェクトが木のリフトの時
	case ElectricCurrent::TargetObject::WoodenLift:
		// 木のリフトが作動しているならtrueを返す
		if (UpdateObjManager::Instance().GetLiftUpdate())
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
		// 対象のオブジェクトが銅の扉の時
	case ElectricCurrent::TargetObject::CopperDoor:
		// 銅の扉が作動しているならtrueを返す
		if (UpdateObjManager::Instance().GetDoorUpdate())
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
		// 対象のオブジェクトが扉の時
	case ElectricCurrent::TargetObject::Gate:
		// 扉が作動しているならtrueを返す
		if (GateManager::Instance().GetElectromagneticInduction())
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	default:
		break;
	}
}

void ElectricCurrent::SetMoveState(const std::shared_ptr<DestinationBase>& destination)
{
	if (m_nowDestination)m_nowDestination->Exit(*this);
	m_nowDestination = destination;
	m_nowDestination->Enter(*this);
}

void ElectricCurrent::DestinationForEnd::Update(ElectricCurrent& owner)
{
	// 向きを調べる
	Math::Vector3 movedir = owner.m_endPos - owner.m_pos;

	// 一定距離まで来たならエフェクトを開始地点に戻す処理
	if (movedir.Length() < k_roopDistance)
	{
		owner.m_pos = owner.m_startPos;
		if(owner.m_stopoverPos != Math::Vector3::Zero)
		{
			// 目的地を中継地点に変える
			owner.SetMoveState(std::make_shared<ElectricCurrent::DestinationForStopover>());
		}

		// 電流エフェクトが終着地点についてからオブジェクトの動作を始める
		switch (owner.m_target)
		{
		case ElectricCurrent::TargetObject::WoodenLift:
			if(UpdateObjManager::Instance().GetLiftUpdate() == false)
			{
				UpdateObjManager::Instance().SetLiftUpdate(true);
			}
			break;
		case ElectricCurrent::TargetObject::CopperDoor:
			if (UpdateObjManager::Instance().GetDoorUpdate() == false)
			{
				UpdateObjManager::Instance().SetDoorUpdate(true);
			}
			break;
		default:
			break;
		}
	}
	// 距離が速度の値より短いなら速度をその距離の値にする
	else if (movedir.Length() < k_moveSpeed)owner.m_moveSpeed = movedir.Length();
	movedir.Normalize();

	// 座標の更新
	owner.m_pos += movedir * owner.m_moveSpeed;
}

void ElectricCurrent::DestinationForStopover::Update(ElectricCurrent& owner)
{
	// 向きを調べる
	Math::Vector3 movedir = owner.m_stopoverPos - owner.m_pos;

	// 一定距離まで来たなら目的地を変える処理
	if (movedir.Length() < k_changeDistance)
	{
		// 目的地を終点に変える
		owner.SetMoveState(std::make_shared<ElectricCurrent::DestinationForEnd>());
		return;
	}
	// 距離が速度の値より短いなら速度をその距離の値にする
	else if (movedir.Length() < k_moveSpeed)owner.m_moveSpeed = movedir.Length();
	movedir.Normalize();

	// 座標の更新
	owner.m_pos += movedir * owner.m_moveSpeed;
}
