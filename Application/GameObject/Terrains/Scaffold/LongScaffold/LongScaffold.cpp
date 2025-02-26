#include "LongScaffold.h"
#include"../../../../Manager/ModelManager/ModelManager.h"

const float LongScaffold::k_modelSize = 15.0f;
const float LongScaffold::k_defaultRotAngle = 90.f;

void LongScaffold::Init()
{
	// モデルの読み込み
	if(!m_model)
	{
		m_model = ModelManager::Instance().GetModel("LongScaffold");
	}

	// 補正値の初期化
	m_adjustWidth = m_gimmickData["LongScaffold"].value("AdjustWidth", 90.0f);

	// 回転角度の初期化
	m_rotAngle = m_gimmickData["LongScaffold"].value("Angle", 90.0f);

	// ImGui用のランダムなIdの生成
	m_randomId = rand();

	// 当たり判定の形状登録
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("LongScaffold", m_model, KdCollider::TypeGround | KdCollider::TypeDamage);
}

void LongScaffold::Update()
{
	float modelSize = m_gimmickData["LongScaffold"].value("ModelSize", k_modelSize);

	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix scaleMat = Math::Matrix::CreateScale({ modelSize,modelSize,modelSize + m_adjustWidth });
	Math::Matrix rotMat = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_rotAngle));
	m_mWorld = scaleMat * rotMat * transMat;
}

void LongScaffold::DrawImGui()
{
	ImGui::PushID(m_randomId);
	if (ImGui::CollapsingHeader("LongScaffold", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat3("LongScaffold m_pos", &m_pos.x, 0.01f);
		ImGui::DragFloat("LongScaffold m_adjust", &m_adjustWidth, 0.01f);
		ImGui::DragFloat("LongScaffold Angle", &m_rotAngle, 0.01f);
	}
	ImGui::PopID();
}

void LongScaffold::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}
