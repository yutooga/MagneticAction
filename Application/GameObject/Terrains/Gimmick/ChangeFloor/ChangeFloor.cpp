#include "ChangeFloor.h"
#include "../../../Character/Player/Player.h"
#include"../../../../Manager/ModelManager/ModelManager.h"
#include"../../../../Scene/SceneManager.h"

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
}

void ChangeFloor::Update()
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
	else if(m_changeInterval >= 0.0f && m_coolTime <= 0.0f)
	{
		m_changeInterval -= k_decreaseInterval;
	}

	//まとう磁力を切り替える処理
	if (m_changeInterval < 0.0f)
	{
		if (m_maguneForce == MagunePowerN)
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
		//吸着処理
		m_adPow = k_adsorptionPower;
		moveDir = m_pos - m_obj.lock()->GetPos();

		// プレイヤーがオブジェクトの上下にいる場合XZ軸の引き寄せる力を０にする
		if (m_pos.y > m_obj.lock()->GetPos().y || m_pos.y < m_obj.lock()->GetPos().y)
		{
			moveDir.x = 0;
			moveDir.z = 0;
		}
		else
		{
			moveDir.y = 0;
			// プレイヤーがオブジェクトの左右にいるならZ方向の引き寄せる力を０にする
			if (m_obj.lock()->GetPos().z < (m_pos.z + m_adjustAdValue) && m_obj.lock()->GetPos().z >(m_pos.z - m_adjustAdValue))
			{
				if (m_pos.x > m_obj.lock()->GetPos().x || m_pos.x < m_obj.lock()->GetPos().x)
				{
					moveDir.z = 0;
				}
			}
			// プレイヤーがオブジェクトの前後にいるならX方向の引き寄せる力を０にする
			else if (m_pos.z > m_obj.lock()->GetPos().z || m_pos.z < m_obj.lock()->GetPos().z) { moveDir.x = 0; }
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
	if (m_maguneForce == NoForce)return;

	Math::Color color;
	if (m_maguneForce == MagunePowerN)
	{
		//S極の場合赤色に光らせる
		color = { m_changeInterval/ m_intervalMax,0,1.f- (m_changeInterval / m_intervalMax)};
	}
	else if (m_maguneForce == MagunePowerS)
	{
		//N極の場合青色に光らせる
		color = { 1.f - (m_changeInterval / m_intervalMax),0, m_changeInterval / m_intervalMax };
	}
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld, color);
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

	sphere[0].m_sphere.Center.y += 30.0f;
	sphere[1].m_sphere.Center.y += 70.0f;
	sphere[2].m_sphere.Center.y += 120.0f;

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
