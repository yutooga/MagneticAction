#include "ChangeFloor.h"
#include "../../../../Character/Player/Player.h"
#include"../../../../../Manager/ModelManager/ModelManager.h"
#include"../../../../../Scene/SceneManager.h"

const float ChangeFloor::m_updateArea = 70.0f;
const float ChangeFloor::k_adsorptionPower = 0.2f;
const float ChangeFloor::k_decreaseInterval = 1.f;
const float ChangeFloor::k_modelScale = 4.f;
const float ChangeFloor::k_colisionRadius = 20.f;
const float ChangeFloor::k_floatingSpeed = 0.1f;
const float ChangeFloor::k_floatingAmplitude = 3.f;

void ChangeFloor::Init()
{
	// モデルの読み込み
	if(!m_model)
	{
		m_model = ModelManager::Instance().GetModel("ChangeFloor");
	}

	// 形状登録
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("ChangeFloor", m_model, KdCollider::TypeGround);

	// まとっている磁力の初期化
	m_maguneForce = MagunePowerS;

	// ImGui用のランダムなIdの生成
	m_randomId = rand();

	// 斥力の大きさの初期化
	m_adPow = m_gimmickData["ChangeFloor"]["Adsorption"].value("Pow", k_adsorptionPower);

	// 斥力の補正値の初期化
	m_adjustAdValue = m_gimmickData["ChangeFloor"]["Adsorption"].value("AdjustValue", 6.f);
}

void ChangeFloor::Update()
{
	// 纏っている磁力の切り替え
	ChangeForce();

	// SEが再生中かどうか判断する処理
	SeCheck();

	// 行列の確定
	Math::Matrix scaleMat = Math::Matrix::CreateScale(k_modelScale);
	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix rotMat = Math::Matrix::CreateRotationY(m_angle);
	m_mWorld = scaleMat * rotMat * transMat;
}

void ChangeFloor::PostUpdate()
{
	//プレイヤーが一定範囲内にいないなら早期リターン
	if (PlayerManager::Instance().GetPlayer().expired() == false)
	{
		if ((GetPos() - PlayerManager::Instance().GetPlayer().lock()->GetPos()).Length() > m_updateArea)return;
	}

	// 反発範囲内にプレイヤーがいるかどうか判断する
	JugmentPlayer();

	MagneFloorBase::PostUpdate();
}

void ChangeFloor::PlayerReaction()
{
	//オブジェクトが存在しないまたは磁力をまとっていないなら早期リターン
	if (m_nowState == State::NormalState || m_obj.expired() == true)return;

	Math::Vector3 moveDir = Math::Vector3::Zero;
	Math::Vector3 addPos = m_obj.lock()->GetPos();
	
	// 吸着状態の時
	if (m_nowState == State::Adsorption)
	{
		// SEの再生
		CheckSe();

		//吸着処理
		m_adPow = k_adsorptionPower;
		moveDir = m_pos - m_obj.lock()->GetPos();
		Math::Vector3 playerPos = m_obj.lock()->GetPos();

		// プレイヤーがオブジェクトの上下にいる場合XZ軸の引き寄せる力を０にする
		if (m_pos.y > playerPos.y || m_pos.y < playerPos.y)
		{
			moveDir.x = 0;
			moveDir.z = 0;
		}
		else
		{
			moveDir.y = 0;
			// プレイヤーがオブジェクトの左右にいるならZ方向の引き寄せる力を０にする
			if (playerPos.z < (m_pos.z + m_adjustAdValue) && playerPos.z >(m_pos.z - m_adjustAdValue))
			{
				if (m_pos.x > playerPos.x || m_pos.x < playerPos.x)
				{
					moveDir.z = 0;
				}
			}
			// プレイヤーがオブジェクトの前後にいるならX方向の引き寄せる力を０にする
			else if (m_pos.z > playerPos.z || m_pos.z < playerPos.z) { moveDir.x = 0; }
		}

		// 引き寄せる力の大きさの値より距離の値が小さくなったら引き寄せる力の値を距離の値にする
		if (moveDir.Length() < k_adsorptionPower)m_adPow = moveDir.Length();
		moveDir.Normalize();
		addPos += moveDir * m_adPow;

		// プレイヤーに更新後の座標をセットする
		m_obj.lock()->SettingPos(addPos);
	}
}

void ChangeFloor::DrawBright()
{
	//磁力をまとっていないなら描画しない
	if ((m_maguneForce & NoForce) != 0)return;

	if ((m_maguneForce & MagunePowerN) != 0)
	{
		//S極の場合赤色に光らせる
		m_color = { m_changeInterval/ m_intervalMax,0,1.f- (m_changeInterval / m_intervalMax)};
	}
	else if ((m_maguneForce & MagunePowerS) != 0)
	{
		//N極の場合青色に光らせる
		m_color = { 1.f - (m_changeInterval / m_intervalMax),0, m_changeInterval / m_intervalMax };
	}
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld, m_color);
}

void ChangeFloor::DrawImGui()
{
	ImGui::PushID(m_randomId);
	if (ImGui::CollapsingHeader("ChangeFloor", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat3("m_pos", &m_pos.x, 0.1f);
		ImGui::DragFloat("m_angle", &m_angle, 0.01f);
		ImGui::DragFloat("Value", &m_adjustAdValue, 0.01f);
	}
	ImGui::PopID();
}

void ChangeFloor::JugmentPlayer()
{
	//球判定用の変数を作成
	KdCollider::SphereInfo sphere[k_colliderNum];

	for (int i = 0; i < k_colliderNum; i++)
	{
		sphere[i].m_sphere.Center = m_pos;	//球の中心位置を設定
		sphere[i].m_sphere.Radius = k_colisionRadius;	//球の半径を設定
		sphere[i].m_type = KdCollider::TypeSight;	//当たり判定をしたいタイプを設定
	}

	sphere[0].m_sphere.Center.y += m_gimmickData["ChangeFloor"]["Colision"]["SphereCenter"].value("Under", 30.f);
	sphere[1].m_sphere.Center.y += m_gimmickData["ChangeFloor"]["Colision"]["SphereCenter"].value("Middle", 70.f);
	sphere[2].m_sphere.Center.y += m_gimmickData["ChangeFloor"]["Colision"]["SphereCenter"].value("Top", 120.f);

	//当たり判定
	for (auto& obj : SceneManager::Instance().GetObjList())
	{
		// プレイヤーでない場合読み飛ばす
		if (obj->GetObjType() != ObjectType::Player)continue;
		// まとっている磁力が違うなら抜ける
		else if (obj->GetMaguneForce() != m_maguneForce)
		{
			// 前のフレームまで反発していたなら状態を初期化する
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

		for (int i = 0; i < k_colliderNum; i++)
		{
			hitFlg = obj->Intersects(sphere[i], nullptr);
			if (hitFlg)break;	// 当たったのならfor文を抜ける
		}

		// もし当たっているなら
		if (hitFlg)
		{
			// このマグネットがプレイヤーを動かすほど近くにいるか
			Math::Vector3 playerPos = obj->GetPos();
			float distance = (obj->GetPos() - m_pos).Length();
			obj->SetReaction(true); // 重力は一旦無視
			if (distance <= k_magnetRange)
			{
				playerPos.y += k_magnetPower;// 問答無用でプレイヤーを動かす
			}
			else
			{
				// 上まで持ち上げられた
				m_addAmount += DirectX::XMConvertToRadians(k_floatingAmplitude);
				playerPos.y += sin(m_addAmount) * k_floatingSpeed;

				// SE再生
				if (!m_seFlg)
				{
					m_wpSe = KdAudioManager::Instance().Play(m_gimmickData["Se"]["Floating"]["URL"], false);
					m_seFlg = true;
				}
			}
			obj->SettingPos(playerPos);

			m_updateFlg = true;

			break;
		}
		
		else if (!hitFlg)
		{
			// 1フレーム前まで当たっていたなら状態を初期化する
			if (m_updateFlg)
			{
				obj->SetReaction(false);
				m_updateFlg = false;
				m_addFlg = false;
				m_addAmount = 0;
			}
		}
				
	}
}

void ChangeFloor::ChangeForce()
{
	// クールタイムの更新
	if (m_coolTime > 0)
	{
		m_coolTime--;
		if (m_coolTime < 0)
		{
			m_coolTime = 0;
		}
	}

	// まとう磁力が切り替わる間のカウントの更新
	else if (m_changeInterval >= 0.0f && m_coolTime <= 0.0f)
	{
		m_changeInterval -= k_decreaseInterval;
	}

	//まとう磁力を切り替える処理
	if (m_changeInterval < 0.0f)
	{
		if ((m_maguneForce & MagunePowerN) != 0)
		{
			m_maguneForce = MagunePowerS;
		}
		else
		{
			m_maguneForce = MagunePowerN;
		}
		m_changeInterval = m_intervalMax;
		m_coolTime = k_coolTimeLength;
	}
}

void ChangeFloor::SeCheck()
{
	std::shared_ptr<KdSoundInstance> _spSe = m_wpSe.lock();

	// SEの実態がないなら処理しない
	if (!_spSe)return;

	// SEの再生が終わったらならSE再生フラグをOFFにするs
	if (!_spSe->IsPlaying())
	{
		m_seFlg = false;
	}
}
