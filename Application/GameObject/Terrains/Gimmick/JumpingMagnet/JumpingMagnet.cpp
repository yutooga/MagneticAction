#include "JumpingMagnet.h"
#include"../../../../Manager/ModelManager/ModelManager.h"
#include"../../../../Scene/SceneManager.h"

const float JumpingMagnet::k_sinCurveAngle = 3.0f;
const float JumpingMagnet::k_addAngle = 0.1f;
const float JumpingMagnet::k_colisionCircleNum = 3.f;

void JumpingMagnet::Init()
{
	// モデルの読み込み
	if (!m_model)
	{
		m_model = ModelManager::Instance().GetModel("MaguneFloor");
	}

	// 当たり判定の形状登録
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("JumpingMagnet", m_model, KdCollider::TypeGround | KdCollider::TypeDamage);

	// ImGui用のランダムなIdの生成
	m_randomId = rand();

	// まとう磁力の初期化
	m_maguneForce = NoForce;

	// モデルの大きさの初期化
	m_modelSize = m_gimmickData["JumpingMagnet"].value("ModelSize", 8.4f);

	// 補正値の初期化
	m_adjustAdValue = m_gimmickData["JumpingMagnet"].value("AdjustValue", 25.f);

	// 更新フラグの初期化
	m_updateFlg = false;

	// デバックラインの初期化
	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
}

void JumpingMagnet::Update()
{
	// SE再生チェック処理
	SeCheck();

	// 行列の確定
	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix scaleMat = Math::Matrix::CreateScale(m_modelSize);
	m_mWorld = scaleMat * transMat;
}

void JumpingMagnet::PostUpdate()
{
	// プレイヤーが反発範囲内にいるかどうか判断する関数
	JudgmentPlayer();

	MagneFloorBase::PostUpdate();
}

void JumpingMagnet::DrawImGui()
{
	ImGui::PushID(m_randomId);
	if (ImGui::CollapsingHeader("JumpingMagunet", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat3("JumpingMagunet m_pos", &m_pos.x, 0.01f);
		ImGui::DragFloat("JumpingMagunet m_size", &m_modelSize, 0.01f);
		ImGui::DragFloat("JumpingMagunet Value", &m_adjustAdValue, 0.01f);
	}
	ImGui::PopID();
}

void JumpingMagnet::JudgmentPlayer()
{

	//球判定用の変数を作成
	std::vector<KdCollider::SphereInfo> spheres;

	for (int i = 0; i < k_colisionCircleNum; i++)
	{
		KdCollider::SphereInfo sphere;
		sphere.m_sphere.Center = m_pos;
		sphere.m_sphere.Radius = m_gimmickData["JumpingMagnet"]["Colision"].value("Radius", 25.f);
		sphere.m_type = KdCollider::TypeSight;
		spheres.push_back(sphere);
	}

	spheres[static_cast<int>(ColisionCircle::Top)].m_sphere.Center.y += static_cast<float>(ColisionAdjustValue::TopCircle);
	spheres[static_cast<int>(ColisionCircle::Center)].m_sphere.Center.y += static_cast<float>(ColisionAdjustValue::CenterCircle);
	spheres[static_cast<int>(ColisionCircle::Under)].m_sphere.Center.y += static_cast<float>(ColisionAdjustValue::UnderCircle);



	// プレイヤーと磁力が同じ場合当たり判定
	for (auto& obj : SceneManager::Instance().GetObjList())
	{
		// プレイヤーでない場合読み飛ばす
		if (obj->GetObjType() != ObjectType::Player)continue;
		else if (obj->GetMaguneForce() != m_maguneForce)
		{
			// 前のフレームで反発をしていたなら初期化してから抜ける
			if (m_updateFlg)
			{
				obj->SetReaction(false);
				m_updateFlg = false;
				m_addFlg = false;
				m_addAmount = 0;
			}
			break;
		}

		bool hitFlg = false;
		hitFlg = obj->Intersects(spheres[static_cast<int>(ColisionCircle::Top)], nullptr);
		if (!hitFlg)
		{
			hitFlg = obj->Intersects(spheres[static_cast<int>(ColisionCircle::Center)], nullptr);
		}
		if (!hitFlg)
		{
			hitFlg = obj->Intersects(spheres[static_cast<int>(ColisionCircle::Under)], nullptr);
		}

		if (hitFlg)
		{
			// プレイヤーの浮遊処理
			FloatingPlayer(obj);
			break;
		}
		// 当たってないなら状態を初期化する
		else if (!hitFlg)
		{
			if (m_updateFlg)
			{
				obj->SetReaction(false);
				m_updateFlg = false;
				m_addFlg = false;
				m_addAmount = 0;
			}
			break;
		}
	}
}

void JumpingMagnet::PlayerReaction()
{
	//オブジェクトが存在しないまたは磁力をまとっていないなら早期リターン
	if (m_nowState == State::NormalState || m_obj.expired() == true)return;

	// 吸着状態でない場合早期リターン
	if (m_nowState != State::Adsorption)return;

	Math::Vector3 moveDir = Math::Vector3::Zero;	// 動く方向
	Math::Vector3 addPos = m_obj.lock()->GetPos();	// 加算量
	Math::Vector3 playerPos = m_obj.lock()->GetPos();	// playerの座標

	//吸着処理

	// SEの再生
	CheckSe();

	m_adPow = k_adsorptionPower;
	moveDir = m_pos - playerPos;

	// プレイヤーがオブジェクトの上下にいる場合
	if (m_pos.y > playerPos.y || m_pos.y < playerPos.y)
	{
		moveDir.x = 0;
		moveDir.z = 0;
	}
	// プレイヤーが左右の側面にいる場合
	else
	{
		moveDir.y = 0;

		// なおかつX座標が大きいまたは小さい側面に張り付いている時
		if (playerPos.z < (m_pos.z + m_adjustAdValue) && playerPos.z >(m_pos.z - m_adjustAdValue))
		{
			if (m_pos.x > playerPos.x || m_pos.x < playerPos.x)
			{
				moveDir.z = 0;
			}
		}
		// なおかつZ座標が大きいまたは小さい側面に張り付いている時
		else if (m_pos.z > playerPos.z || m_pos.z < playerPos.z) { moveDir.x = 0; }
	}

	if (moveDir.Length() < k_adsorptionPower)m_adPow = moveDir.Length();
	moveDir.Normalize();
	addPos += moveDir * m_adPow;
	m_obj.lock()->SettingPos(addPos);
}

void JumpingMagnet::FloatingPlayer(const std::weak_ptr<KdGameObject>& _obj)
{
	// 実態が存在しない場合は早期リターン
	if (_obj.expired() != false)return;

	// このマグネットがプレイヤーを動かすほど近くにいるか
	Math::Vector3 playerPos = _obj.lock()->GetPos();
	float distance = (_obj.lock()->GetPos() - m_pos).Length();

	_obj.lock()->SetReaction(true); // 重力は一旦無視
	if (distance <= k_magnetRange)
	{
		playerPos.y += k_magnetPower;// 問答無用でプレイヤーを動かす
	}
	else
	{
		// 上まで持ち上げられた
		m_addAmount += DirectX::XMConvertToRadians(k_sinCurveAngle);
		playerPos.y += sin(m_addAmount) * k_addAngle;
		if(!m_seFlg)
		{
			m_wpSe = KdAudioManager::Instance().Play(m_gimmickData["Se"]["Floating"]["URL"], false);
			m_seFlg = true;
		}
	}

	_obj.lock()->SettingPos(playerPos);

	m_updateFlg = true;
}

void JumpingMagnet::MagneScope()
{
	//===============================
	//              球判定
	//===============================

	//球判定用の変数を作成
	KdCollider::SphereInfo sphere;

	//球の中心位置を設定
	sphere.m_sphere.Center = m_pos;
	//球の半径を設定
	sphere.m_sphere.Radius = m_gimmickData["JumpingMagnet"]["Scope"].value("Radius", 41.f);
	//当たり判定をしたいタイプを設定
	sphere.m_type = KdCollider::TypeSight;

	//当たり判定
	for (auto& obj : SceneManager::Instance().GetObjList())
	{
		// プレイヤー以外は当たり判定しない
		if (obj->GetObjType() != ObjectType::Player)continue;

		bool hitFlg = false;
		hitFlg = obj->Intersects(sphere, nullptr);

		if (hitFlg)
		{
			OnHit(obj);
			break;
		}
		else
		{
			// １フレーム前まで吸着状態だった場合初期状態に戻す
			if (m_nowState == State::Adsorption)
			{
				m_nowState = State::NormalState;
				obj->SetUpdate(false);
			}
			break;
		}
	}
}

void JumpingMagnet::SeCheck()
{
	std::shared_ptr<KdSoundInstance> _spSe = m_wpSe.lock();

	// SEの実態がないなら処理しない
	if (!_spSe)return;

	// SEの再生が終わったらならSE再生フラグをOFFにする
	if (!_spSe->IsPlaying())
	{
		m_seFlg = false;
	}
}
