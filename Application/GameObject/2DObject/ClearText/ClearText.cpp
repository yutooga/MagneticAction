#include "ClearText.h"

const float ClearText::k_alphaMax = 1.0f;
const float ClearText::k_addAlphaAmount = 0.05f;

void ClearText::Init()
{
	// 画像のロード
	m_tex.Load("Asset/Textures/Scene/GameScene/2DObject/ClearText/ClearText.png");

	// 画像のサイズの初期化
	m_textSize = 5.0f;

	// 座標の初期化
	m_pos = { 5,140,0 };
}

void ClearText::Update()
{
	// 透明度が最大になったら早期リターン
	if (m_alpha == k_alphaMax)return;

	// 透明度の更新処理
	m_alpha += k_addAlphaAmount;

	// 透明度が最大値を超えたら強制的に最大値にセットする
	if (m_alpha >= k_alphaMax)
	{
		m_alpha = k_alphaMax;
	}

	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix scaleMat = Math::Matrix::CreateScale(m_textSize);
	m_mWorld = scaleMat * transMat;
}

void ClearText::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.SetMatrix(m_mWorld);
	Math::Rectangle rc = { 0,0,237,88 };
	Math::Color color = { 1,1,1,m_alpha };
	KdShaderManager::Instance().m_spriteShader.DrawTex(&m_tex, 0, 0, &rc, &color);
	KdShaderManager::Instance().m_spriteShader.SetMatrix(Math::Matrix::Identity);
}

void ClearText::DrawImGui()
{
	if (ImGui::CollapsingHeader("clear", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat("clear m_posX", &m_pos.x, 0.1f);
		ImGui::DragFloat("clear m_posY", &m_pos.y, 0.1f);
		ImGui::DragFloat("clear size", &m_textSize, 0.1f);
	}
}
