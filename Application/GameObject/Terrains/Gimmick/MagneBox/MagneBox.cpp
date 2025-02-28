#include "MagneBox.h"
#include"../../../../Manager/ModelManager/ModelManager.h"

const float MagneBox::k_changeMoment = 100.f;

void MagneBox::Init()
{
	// モデルの読み込み
	if (!m_model)
	{
		m_model = ModelManager::Instance().GetModel("MagneBox");
	}

	// IMGUI用の初期化
	m_randomId = rand();

	// まとっている磁力の初期化
	m_maguneForce = MagunePowerS;

	// モデルの大きさの初期化
	m_modelSize = m_gimmickData["MagneBox"].value("ModelSize", 13.4f);

	// 角度の初期化
	m_angle = m_gimmickData["MagneBox"].value("Angle", 180.f);

	// 当たり判定の形状登録
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("MagneBox", m_model, KdCollider::TypeGround);
}

void MagneBox::Update()
{
	// 纏っている磁力を切り替える処理
	ChangeForce();

	// 表示色の更新
	BrightColorUpdate();

	// 行列の確定
	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix scaleMat = Math::Matrix::CreateScale(m_modelSize);
	Math::Matrix rotMat = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angle));
	m_mWorld = scaleMat * rotMat * transMat;
}

void MagneBox::DrawBright()
{
	//磁力をまとっていないなら描画しない
	if ((m_maguneForce & NoForce) != 0)return;

	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld, m_color);
}

void MagneBox::DrawImGui()
{
	ImGui::PushID(m_randomId);
	if (ImGui::CollapsingHeader("MagneBox", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat3("m_pos", &m_pos.x, 0.01f);
		ImGui::DragFloat("m_size", &m_modelSize, 0.01f);
		ImGui::DragFloat("angle", &m_angle, 0.01f);
	}
	ImGui::PopID();
}

void MagneBox::ChangeForce()
{
	// クールタイム中の時
	if (m_coolTimeCnt > 0)
	{
		m_coolTimeCnt--;
	}
	else
	{
		// カウントの更新
		m_changeCnt++;

		// 磁極が切り替わるタイミングになったら磁極を切り替える
		if (m_changeCnt > k_changeMoment)
		{
			// 今まとっている磁極を反転させる
			switch (m_maguneForce)
			{
			case KdGameObject::MagunePowerN:
				m_maguneForce = MagunePowerS;
				break;
			case KdGameObject::MagunePowerS:
				m_maguneForce = MagunePowerN;
				break;
			default:
				break;
			}

			// カウントの初期化
			m_changeCnt = 0.f;

			// クールタイムの初期化
			m_coolTimeCnt = m_gimmickData["MagneBox"].value("CoolTime", 500);
		}
	}
}

void MagneBox::BrightColorUpdate()
{
	if ((m_maguneForce & MagunePowerN) != 0)
	{
		// S極の場合赤色に光らせる
		m_color = { m_gimmickData["MagneBox"].value("RgbMax", 1.f) - (m_changeCnt / k_changeMoment),0,m_changeCnt / k_changeMoment };
	}
	else if ((m_maguneForce & MagunePowerS) != 0)
	{
		// N極の場合青色に光らせる
		m_color = { m_changeCnt / k_changeMoment,0, m_gimmickData["MagneBox"].value("RgbMax", 1.f) - (m_changeCnt / k_changeMoment) };
	}
}
