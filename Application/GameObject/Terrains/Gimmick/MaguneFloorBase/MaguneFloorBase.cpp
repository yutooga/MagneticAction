#include "MaguneFloorBase.h"
#include"../../../../Scene/SceneManager.h"

const float MagneFloorBase::k_adsorptionPower = 0.4f;
const float MagneFloorBase::k_oppositionPower = 0.1f;
const float MagneFloorBase::k_initialOppoPowerValue = 0.5f;
const float MagneFloorBase::k_OppoStateLimit = 30.f;

void MagneFloorBase::PostUpdate()
{
	//エリア判定
	MaguneScope();
	//エリア内にプレイヤーが入った時の処理
	PlayerReaction();
}

void MagneFloorBase::MaguneScope()
{
	//===============================
	//              球判定
	//===============================

	//球判定用の変数を作成
	KdCollider::SphereInfo sphere;

	//球の中心位置を設定
	sphere.m_sphere.Center = m_pos;
	//球の半径を設定
	sphere.m_sphere.Radius = 20.0f;
	//当たり判定をしたいタイプを設定
	sphere.m_type = KdCollider::TypeSight;
	//デバック用
	//m_pDebugWire->AddDebugSphere(sphere.m_sphere.Center, sphere.m_sphere.Radius);

	//当たり判定
	for (auto& obj : SceneManager::Instance().GetObjList())
	{
		if (obj->GetObjType() != ObjectType::Player)continue;
		bool hitFlg = false;
		hitFlg = obj->Intersects(sphere, nullptr);
		//	当たっているときの処理
		if (hitFlg)
		{
			OnHit(obj);
		}
		else
		{
			// 直前のフレームまで引き寄せあっている状態だったのなら初期状態に戻す
			if (m_nowState == State::Adsorption)
			{
				m_nowState = State::NormalState;
				obj->SetUpdate(false);
			}
		}
	}
}

void MagneFloorBase::PlayerReaction()
{
	//オブジェクトが存在しないまたは磁力をまとっていないなら早期リターン
	if (m_nowState == State::NormalState || m_obj.expired() == true)return;

	Math::Vector3 moveDir = Math::Vector3::Zero;
	Math::Vector3 addPos = m_obj.lock()->GetPos();

	//反発処理
	if (m_nowState == State::Opposition)
	{
		if (m_oppCnt == 0)
		{
			//反発SE再生
			KdAudioManager::Instance().Play("Asset/Sounds/GameScene/Terrains/Gimmick/Repulsion.wav", false);
		}

		moveDir = m_obj.lock()->GetPos() - m_pos;
		moveDir.Normalize();
		addPos += moveDir * m_oppoPow;
		m_oppoPow += k_oppositionPower;
		m_obj.lock()->SettingPos(addPos);
		m_oppCnt++;

		if (m_oppCnt > k_OppoStateLimit)
		{
			m_nowState = State::NormalState;
			m_oppCnt = 0;
			m_oppoPow = k_initialOppoPowerValue;
		}
	}
	//吸着処理
	else if (m_nowState == State::Adsorption)
	{
		m_adPow = k_adsorptionPower;
		moveDir = m_pos - m_obj.lock()->GetPos();
		if (moveDir.Length() < k_adsorptionPower)m_adPow = moveDir.Length();
		moveDir.Normalize();
		addPos += moveDir * m_adPow;
		m_obj.lock()->SettingPos(addPos);
	}
}

void MagneFloorBase::OnHit(std::weak_ptr<KdGameObject> _obj)
{
	//オブジェクトが存在しないまたは磁力をまとっていないなら早期リターン
	if (_obj.expired() == true)return;
	else if (_obj.lock()->GetMaguneForce() == NoForce || m_maguneForce == NoForce)return;

	if (_obj.lock()->GetMaguneForce() == m_maguneForce)
	{
		//状態を反発にする
		m_nowState = State::Opposition;
	}
	else
	{
		//状態を吸着にする
		m_nowState = State::Adsorption;
		_obj.lock()->SetUpdate(true);
	}

	m_obj = _obj;
}