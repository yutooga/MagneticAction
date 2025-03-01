#include "MagneFloorBase.h"
#include"../../../../../Scene/SceneManager.h"

const float MagneFloorBase::k_adsorptionPower = 0.4f;
const float MagneFloorBase::k_oppositionPower = 0.1f;
const float MagneFloorBase::k_initialOppoPowerValue = 0.5f;
const float MagneFloorBase::k_OppoStateLimit = 30.f;

void MagneFloorBase::PostUpdate()
{
	//エリア判定
	MagneScope();
	//エリア内にプレイヤーが入った時の処理
	PlayerReaction();
}

void MagneFloorBase::MagneScope()
{
	//===============================
	//              球判定
	//===============================

	//球判定用の変数を作成
	KdCollider::SphereInfo sphere;

	//球の中心位置を設定
	sphere.m_sphere.Center = m_pos;
	//球の半径を設定
	sphere.m_sphere.Radius = m_gimmickData["MagneFloorBase"]["Colision"].value("Radius", 20.f);
	//当たり判定をしたいタイプを設定
	sphere.m_type = KdCollider::TypeSight;

	//当たり判定
	for (auto& obj : SceneManager::Instance().GetObjList())
	{
		// プレイヤー以外とは判定をしない
		if (obj->GetObjType() != ObjectType::Player)continue;

		bool hitFlg = false;
		hitFlg = obj->Intersects(sphere, nullptr);

		if (hitFlg)
		{
			OnHit(obj);	//	当たっているときの処理
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

	switch (m_nowState)
	{
	case MagneFloorBase::State::Opposition:	//反発処理
		if (m_oppCnt == 0)
		{
			//反発SE再生
			KdAudioManager::Instance().Play(m_gimmickData["Se"]["Repulsion"]["URL"], false);
		}

		moveDir = m_obj.lock()->GetPos() - m_pos;	// プレイヤーをオブジェクトと反対の方向に吹っ飛ばす
		moveDir.Normalize();
		addPos += moveDir * m_oppoPow;
		m_oppoPow += k_oppositionPower;
		m_obj.lock()->SettingPos(addPos);
		m_oppCnt++;

		// 一定時間たったら反発処理を終了する
		if (m_oppCnt > k_OppoStateLimit)
		{
			m_nowState = State::NormalState;
			m_oppCnt = 0;
			m_oppoPow = k_initialOppoPowerValue;
		}
		break;
	case MagneFloorBase::State::Adsorption:	//吸着処理

		// SEの再生
		CheckSe();

		m_adPow = k_adsorptionPower;
		moveDir = m_pos - m_obj.lock()->GetPos();	// プレイヤーをオブジェクトの方向に引っ張る
		if (moveDir.Length() < k_adsorptionPower)m_adPow = moveDir.Length();
		moveDir.Normalize();
		addPos += moveDir * m_adPow;
		m_obj.lock()->SettingPos(addPos);
		break;
	default:
		break;
	}
}

void MagneFloorBase::OnHit(std::weak_ptr<KdGameObject> _obj)
{
	//オブジェクトが存在しないまたは磁力をまとっていないなら早期リターン
	if (_obj.expired() == true)return;
	else if ((_obj.lock()->GetMaguneForce() & NoForce) != 0)return;
	else if ((m_maguneForce & NoForce) != 0)return;

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

	// 対象のオブジェクトを記憶しておく
	m_obj = _obj;
}