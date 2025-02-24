#include "MoveMaguneFloor.h"
#include"../../../../Manager/ModelManager/ModelManager.h"
#include"../../../../Scene/SceneManager.h"
#include"../../../Manager/UpdateObjManager/UpdateObjManager.h"

const float MoveMaguneFloor::k_specialMoveSpeed = 2.f;
const float MoveMaguneFloor::k_normalMoveSpeed = 350.f;
const float MoveMaguneFloor::k_addRotAngleAmount = 0.5f;
const float MoveMaguneFloor::k_initialOppoPowerValue = 0.1f;
const float MoveMaguneFloor::k_correctionValueY = 3.0f;
const float MoveMaguneFloor::k_oppositionLimit = 30.f;
const float MoveMaguneFloor::k_adsorptionPower = 0.4f;

void MoveMaguneFloor::Init()
{
	// まとっている磁力の初期化
	m_maguneForce = NoForce;

	// ImGui用のランダムなIdの生成
	m_randomId = rand();
}

void MoveMaguneFloor::Update()
{

	if (m_moveState)
	{
		m_moveState->Update(*this);
	}
}

void MoveMaguneFloor::PostUpdate()
{
	// 更新フラグの初期化
	m_reactionFlg = false;

	if (m_moveState)
	{
		m_moveState->PostUpdate(*this);
	}
}

void MoveMaguneFloor::DrawBright()
{
	//磁力をまとっていないなら描画しない
	if ((m_maguneForce & NoForce) != 0) return;
	else if (m_nowMoveState == MoveState::nomal)return;	//	通常移動の時は早期リターン

	Math::Color color;
	if ((m_maguneForce & MagunePowerN) != 0)
	{
		//N極の場合赤色に光らせる
		color = { 1,0,0 };
	}
	else if ((m_maguneForce & MagunePowerS) != 0)
	{
		//S極の場合青色に光らせる
		color = { 0,0,1 };
	}
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld, color);
}

void MoveMaguneFloor::DrawImGui()
{
	ImGui::PushID(m_randomId);
	if (ImGui::CollapsingHeader("MoveMaguneFloor", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat3("MoveMaguneFloor m_posX", &m_pos.x, 0.1f);
		ImGui::DragFloat("MoveMaguneFloor size", &m_modelSize, 0.1f);
		ImGui::DragFloat("MoveMaguneFloor ValueZ", &m_adjustAdValueZ, 0.01f);
		ImGui::DragFloat("MoveMaguneFloor ValueY", &m_adjustAdValueY, 0.01f);
		ImGui::DragFloat("MoveMaguneFloor Correction", &CorrectionValueX, 0.1f);
		ImGui::DragFloat("MoveMaguneFloor distance", &m_circleDistance, 0.1f);
		ImGui::DragFloat("MoveMaguneFloor radius", &m_circleRadius, 0.1f);
	}
	ImGui::PopID();
}

void MoveMaguneFloor::OnHit(ObjectType _obj)
{
	// 通常移動の場合は早期リターン
	if (m_nowMoveState == MoveState::nomal)return;

	if (_obj == ObjectType::MaguneBulletS)
	{
		// SE再生
		KdAudioManager::Instance().Play("Asset/Sounds/GameScene/Terrains/Gimmick/MagunePower.wav",false);
		m_maguneForce = MagunePowerN;
	}
	else if (_obj == ObjectType::MaguneBulletN)
	{
		// SE再生
		KdAudioManager::Instance().Play("Asset/Sounds/GameScene/Terrains/Gimmick/MagunePower.wav",false);
		m_maguneForce = MagunePowerS;
	}
}

void MoveMaguneFloor::PlayerReaction()
{
	//オブジェクトが存在しないまたは磁力をまとっていないなら早期リターン
	if (m_nowState == State::NormalState || m_obj.expired() == true)return;

	Math::Vector3 moveDir = Math::Vector3::Zero;
	Math::Vector3 addPos = m_obj.lock()->GetPos();
	Math::Vector3 playerPos = addPos;

	//反発処理
	if (m_nowState == State::Opposition)
	{
		if (m_oppCnt == 0)
		{
			//反発SE再生
			KdAudioManager::Instance().Play("Asset/Sounds/GameScene/Terrains/Gimmick/Repulsion.wav", false);
		}

		moveDir = playerPos - m_pos;
		moveDir.Normalize();
		if (moveDir.y <= 0.f)
		{
			moveDir.y = 0;
		}


		addPos += moveDir * m_oppoPow;
		m_oppoPow += k_oppositionPower;

		m_obj.lock()->SettingPos(addPos);
		m_oppCnt++;

		// 反発の継続時間の限界値を超えたら初期状態に戻す
		if (m_oppCnt > k_oppositionLimit)
		{
			m_nowState = State::NormalState;
			m_oppCnt = 0;
			m_oppoPow = MoveMaguneFloor::k_initialOppoPowerValue;
		}
	}

	// 吸着状態のとき
	else if (m_nowState == State::Adsorption)
	{

		//吸着処理
		m_adPow = k_adsorptionPower;
		moveDir = m_pos - m_obj.lock()->GetPos();

		// プレイヤーがオブジェクトの上下にいるとき
		if ((m_pos.y + m_adjustAdValueY) > playerPos.y || (m_pos.y + m_adjustAdValueY) < playerPos.y)
		{
			moveDir.x = 0;
			moveDir.z = 0;
		}
		else
		{
			//moveDir.y = 0;
			// プレイヤーが左右にいるとき
			if (playerPos.z < (m_pos.z + m_adjustAdValueZ) && playerPos.z >(m_pos.z - m_adjustAdValueZ))
			{
				if (m_pos.x > playerPos.x || m_pos.x < playerPos.x)
				{
					moveDir.z = 0;
				}
			}
			// プレイヤーが前後にいるとき
			else if (m_pos.z > playerPos.z || m_pos.z < playerPos.z) { moveDir.x = 0; }
		}

		if (moveDir.Length() < k_adsorptionPower)m_adPow = moveDir.Length();
		moveDir.Normalize();

		addPos += moveDir * m_adPow;
		addPos.x += m_pos.x - m_previousPosition.x;
		m_obj.lock()->SettingPos(addPos);

		if (m_ObjType == ObjectType::MoveMagunet)
		{
			m_reactionFlg = true;
		}
	}
}

void MoveMaguneFloor::SetMaguneState(MoveState state)
{
	// 各状態のセット
	if (state == MoveState::nomal)
	{
		SetMoveState(std::make_shared<MoveMaguneFloor::NomalMove>());
	}
	else if (state == MoveState::special)
	{
		SetMoveState(std::make_shared<MoveMaguneFloor::SpecialMove>());
	}

	// 今の状態の記録
	m_nowMoveState = state;
}

void MoveMaguneFloor::SetMoveState(std::shared_ptr<MoveStateBase> nextState)
{
	if (m_moveState)m_moveState->Exit(*this);
	m_moveState = nextState;
	m_moveState->Enter(*this);
}

void MoveMaguneFloor::SpecialMove::Enter(MoveMaguneFloor& owner)
{
	// モデルの読み込み
	if (!owner.m_model)
	{
		owner.m_model = ModelManager::Instance().GetModel("MaguneFloor");
	}

	// 当たり判定の形状登録
	owner.m_pDebugWire = std::make_unique<KdDebugWireFrame>();
	owner.m_pCollider = std::make_unique<KdCollider>();
	owner.m_pCollider->RegisterCollisionShape("MoveMaguneFloor", owner.m_model, KdCollider::TypeGround | KdCollider::TypeDamage);
}

void MoveMaguneFloor::SpecialMove::Update(MoveMaguneFloor& owner)
{
	// まとっている磁極のセット
	if (owner.m_keyObject == KeyObject::One)
	{
		UpdateObjManager::Instance().SetMaguneForce(owner.m_maguneForce, UpdateObjManager::one);
	}
	else if(owner.m_keyObject == KeyObject::Two)
	{
		UpdateObjManager::Instance().SetMaguneForce(owner.m_maguneForce, UpdateObjManager::two);
	}
	else if (owner.m_keyObject == KeyObject::Three)
	{
		UpdateObjManager::Instance().SetMaguneForce(owner.m_maguneForce, UpdateObjManager::three);
	}
	else if (owner.m_keyObject == KeyObject::Four)
	{
		UpdateObjManager::Instance().SetMaguneForce(owner.m_maguneForce, UpdateObjManager::four);
	}

	// 位置の更新
	owner.m_previousPosition = owner.m_pos;

	// 角度の更新
	owner.m_angle++;

	// 限界角度を超えたらその分引く
	if (owner.m_angle >= k_rotAngleMax)
	{
		owner.m_angle -= k_rotAngleMax;
	}
	
	// 座標の更新
	owner.m_pos.x += sin(DirectX::XMConvertToRadians(owner.m_angle)) * k_specialMoveSpeed;


	// 行列の確定
	Math::Matrix transMat = Math::Matrix::CreateTranslation(owner.m_pos);
	Math::Matrix scaleMat = Math::Matrix::CreateScale({ owner.m_modelSize ,owner.m_modelSize + owner.k_correctionValueY ,owner.m_modelSize });
	owner.m_mWorld = scaleMat * transMat;
}

void MoveMaguneFloor::SpecialMove::PostUpdate(MoveMaguneFloor& owner)
{
	//エリア判定
	owner.MaguneScope();
	//エリア内にプレイヤーが入った時の処理
	owner.PlayerReaction();
}

void MoveMaguneFloor::NomalMove::Enter(MoveMaguneFloor& owner)
{
	// 最初の座標を記録しておく
	owner.m_firstPosZ = owner.m_pos.z;

	// モデルの読み込み
	if (!owner.m_model)
	{
		owner.m_model = ModelManager::Instance().GetModel("WoodenFloor");
	}

	// 当たり判定の形状登録
	owner.m_pDebugWire = std::make_unique<KdDebugWireFrame>();
	owner.m_pCollider = std::make_unique<KdCollider>();
	owner.m_pCollider->RegisterCollisionShape("MoveMaguneFloor", owner.m_model, KdCollider::TypeGround);
}

void MoveMaguneFloor::NomalMove::Update(MoveMaguneFloor& owner)
{
	// 位置の更新
	owner.m_previosPos = owner.m_pos;

	// 角度の更新
	owner.m_angle += k_addRotAngleAmount;

	// 限界角度を超えたらその分引く
	if (owner.m_angle >= k_rotAngleMax)
	{
		owner.m_angle -= k_rotAngleMax;
	}

	// 座標の更新
	owner.m_pos.z = owner.m_firstPosZ + sin(DirectX::XMConvertToRadians(owner.m_angle)) * k_normalMoveSpeed;

	// 行列の確定
	Math::Matrix transMat = Math::Matrix::CreateTranslation(owner.m_pos);
	Math::Matrix scaleMat = Math::Matrix::CreateScale({ owner.m_modelSize + owner.CorrectionValueX ,owner.m_modelSize + 3.0f ,owner.m_modelSize });
	owner.m_mWorld = scaleMat * transMat;
}
