#include "MagneticFloor.h"
#include"../../../../../Scene/SceneManager.h"
#include"../../../../Character/Player/Player.h"
#include"../../../../../Manager/ModelManager/ModelManager.h"

const float MaguneticFloor::k_oppositionPower = 0.1f;
const float MaguneticFloor::k_adsorptionPower = 0.4f;
const float MaguneticFloor::k_adjustValue = 3.0f;
const float MaguneticFloor::k_oppositionLimit = 30;

void MaguneticFloor::Init()
{
	// モデルの読み込み
	if (!m_model)
	{
		m_model = ModelManager::Instance().GetModel("MaguneFloor");
	}

	// IMGUI用の初期化
	m_randomId = rand();

	// まとう磁力の初期化
	m_maguneForce = NoForce;

	// モデルのサイズの初期化
	m_modelSize = m_gimmickData["MaguneticFloor"].value("ModelSize", 3.5f);

	// 磁力の範囲の初期化
	m_maguneScope = m_gimmickData["MaguneticFloor"].value("MagneScope", 20.0f);

	// 斥力の初期化
	m_adPow = m_gimmickData["MaguneticFloor"]["Adsorption"].value("Pow", 0.4f);

	// 当たり判定の形状登録
	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("MaguneticFloor", m_model, KdCollider::TypeGround | KdCollider::TypeDamage);
}

void MaguneticFloor::Update()
{
	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix scaleMat = Math::Matrix::CreateScale({m_modelSize,m_modelSize + k_adjustValue ,m_modelSize });
	m_mWorld = scaleMat * transMat;
}

void MaguneticFloor::PostUpdate()
{
	// オブジェクトが磁力をまとっていないなら早期リターン
	if ((m_maguneForce & NoForce) != 0)return;

	//エリア判定
	MaguneScope();

	//エリア内にプレイヤーが入った時の処理
	PlayerReaction();
}

void MaguneticFloor::DrawImGui()
{
	ImGui::PushID(m_randomId);
	if (ImGui::CollapsingHeader("MaguneticFloor", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat3("MaguneticFloor m_posX", &m_pos.x, 0.1f);
		ImGui::DragFloat("MaguneticFloor size", &m_modelSize, 0.1f);
		ImGui::DragFloat("MaguneticFloor scope", &m_maguneScope, 0.1f);
		ImGui::DragFloat("MaguneticFloor value", &m_adjustAdValue, 0.01f);
	}
	ImGui::PopID();
}

void MaguneticFloor::MaguneScope()
{

	//===============================
	//              球判定
	//===============================

	//球判定用の変数を作成
	KdCollider::SphereInfo sphere;

	//球の中心位置を設定
	sphere.m_sphere.Center = m_pos;
	//球の半径を設定
	sphere.m_sphere.Radius = m_maguneScope;
	//当たり判定をしたいタイプを設定
	sphere.m_type = KdCollider::TypeSight;

	//当たり判定
	for (auto& obj : SceneManager::Instance().GetObjList())
	{
		//プレイヤー以外なら次のオブジェクトへ
		if (obj->GetObjType() != ObjectType::Player)continue;

		bool hitFlg = false;
		hitFlg = obj->Intersects(sphere, nullptr);
		
		if (hitFlg)
		{
			OnHit(obj);
		}
		else
		{
			// 引っ付いた状態のままなら初期状態に戻す
			if (m_nowState == State::Adsorption)
			{
				m_nowState = State::NoState;
				obj->SetUpdate(false);
			}
		}
	}
}

void MaguneticFloor::PlayerReaction()
{

	//オブジェクトが存在しないまたは磁力をまとっていないなら早期リターン
	if (m_nowState == State::NoState || m_obj.expired() == true)return;

	Math::Vector3 moveDir = Math::Vector3::Zero;
	Math::Vector3 addPos = m_obj.lock()->GetPos();

	//反発処理
	if (m_nowState == State::Opposition)
	{
		// 反発処理継続時間
		static int oppCount = 0;

		if (oppCount == 0)
		{
			//反発SE再生
			KdAudioManager::Instance().Play(m_gimmickData["Se"]["Repulsion"]["URL"], false);
		}

		moveDir = m_obj.lock()->GetPos() - m_pos;
		moveDir.Normalize();
		addPos += moveDir * m_oppoPow;
		m_oppoPow += k_oppositionPower;
		m_obj.lock()->SettingPos(addPos);
		oppCount++;

		// 反発の継続時間を過ぎたら初期状態に戻す
		if (oppCount > k_oppositionLimit)
		{
			m_nowState = State::NoState;
			oppCount = 0;
			m_oppoPow = m_gimmickData["MaguneticFloor"]["Opposition"].value("Pow", 0.5f);
		}
	}
	//吸着処理
	else if (m_nowState == State::Adsorption)
	{

		// 斥力の初期化
		m_adPow = k_adsorptionPower;
		moveDir = m_pos - m_obj.lock()->GetPos();

		// プレイヤーの座標
		Math::Vector3 playerPos = m_obj.lock()->GetPos();

		// オブジェクトの前後の座標
		float front = m_pos.z + m_adjustAdValue;
		float back = m_pos.z - m_adjustAdValue;

		//プレイヤーがオブジェクトの上下にいる場合
		if (m_pos.y > playerPos.y || m_pos.y < playerPos.y)
		{
			moveDir.x = 0;
			moveDir.z = 0;
			if(m_updateFlg)
			{
				if (playerPos.z > front || playerPos.z < back)
				{
					moveDir.y = 0;
				}
			}
		}
		else
		{
			moveDir.y = 0;

			// プレイヤーがオブジェクトの左右にいる場合
			if (playerPos.z < front && playerPos.z > back)
			{
				if (m_pos.x > playerPos.x || m_pos.x < playerPos.x)
				{
					moveDir.z = 0;
				}
			}
			// プレイヤーがオブジェクトの前後にいる場合
			else if(m_pos.z > playerPos.z || m_pos.z < playerPos.z)
			{ 
				moveDir.x = 0; 
			}
		}

		// プレイヤーをオブジェクト方向に引っ張る
		if (moveDir.Length() < k_adsorptionPower)m_adPow = moveDir.Length();
		moveDir.Normalize();
		addPos += moveDir * m_adPow;
		m_obj.lock()->SettingPos(addPos);
	}
}

void MaguneticFloor::OnHit(std::weak_ptr<KdGameObject> _obj)
{
	//オブジェクトが存在しないまたは磁力をまとっていないなら早期リターン
	if (_obj.expired() == true)return;
	else if ((_obj.lock()->GetMaguneForce() & NoForce) != 0)return;

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
