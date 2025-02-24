#include "MaguneWall.h"
#include"../../../../Scene/SceneManager.h"
#include"../../../../Manager/ModelManager/ModelManager.h"

const float MaguneWall::k_modelSize = 3.0f;
const float MaguneWall::k_area = 30.f;

void MaguneWall::Init()
{
	// モデルの読み込み
	if (!m_model)
	{
		m_model = ModelManager::Instance().GetModel("MaguneFloor");
	}

	// まとっている磁力の初期化
	m_maguneForce = NoForce;

	// IMGUI用の初期化
	m_randomId = rand();

	// デバックラインの実体の作成
	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
}

void MaguneWall::Update()
{
	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix scaleMat = Math::Matrix::CreateScale(k_modelSize);
	m_mWorld = scaleMat * transMat;
}

void MaguneWall::PostUpdate()
{
	//プレイヤーが一定範囲内にいないなら早期リターン
	MagneFloorBase::PostUpdate();
}

void MaguneWall::DrawImGui()
{
	ImGui::PushID(m_randomId);
	if (ImGui::CollapsingHeader("MaguneWall", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat3("MaguneWall m_pos", &m_pos.x, 0.1f);
	}
	ImGui::PopID();
}

void MaguneWall::PlayerReaction()
{
	//オブジェクトが存在しないまたは磁力をまとっていないなら早期リターン
	if (m_nowState == State::NormalState || m_obj.expired() == true)return;

	// 進む方向
	Math::Vector3 moveDir = Math::Vector3::Zero;
	// 加算量
	Math::Vector3 addPos = m_obj.lock()->GetPos();

	// 反発状態の時
	if (m_nowState == State::Opposition)
	{

		if (m_oppCnt == 0)
		{
			//反発SE再生
			KdAudioManager::Instance().Play("Asset/Sounds/GameScene/Terrains/Gimmick/Repulsion.wav", false);
		}

		// 反発処理
		moveDir = m_obj.lock()->GetPos() - m_pos;
		moveDir.Normalize();
		addPos += moveDir * m_oppoPow;
		m_oppoPow += k_oppositionPower;
		m_obj.lock()->SettingPos(addPos);
		m_oppCnt++;

		// 反発の継続時間の限界値を超えたら初期状態に戻す
		if (m_oppCnt > k_OppoStateLimit)
		{
			m_nowState = State::NormalState;
			m_oppCnt = 0;
			m_oppoPow = k_initialOppoPowerValue;
		}
	}

	// 吸着状態の時
	else if (m_nowState == State::Adsorption)
	{
		//吸着処理
		m_adPow = k_adsorptionPower;
		moveDir = m_pos - m_obj.lock()->GetPos();
		if (moveDir.Length() < k_adsorptionPower)m_adPow = moveDir.Length();
		moveDir.Normalize();
		addPos += moveDir * m_adPow;
		m_obj.lock()->SettingPos(addPos);
	}
}
