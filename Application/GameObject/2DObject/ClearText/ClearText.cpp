#include "ClearText.h"
#include"../../Manager/GoalManager/GoalManager.h"

const float ClearText::k_alphaMax = 1.0f;
const float ClearText::k_addAlphaAmount = 0.005f;

void ClearText::Init()
{
	// 画像のロード
	m_tex.Load(m_jsonData["ClearText"]["URL"]);

	// 画像のサイズの初期化
	m_size = m_jsonData["ClearText"].value("TextureSize", 5.f);

	// 座標の初期化
	m_pos = { m_jsonData["ClearText"]["Pos"].value("X", 5.f),
		m_jsonData["ClearText"]["Pos"].value("Y", 140.f),0};

	// 切り取り範囲の初期化
	m_rc = { 0,0, m_jsonData["ClearText"]["Rc"].value("X", 237),
		m_jsonData["ClearText"]["Rc"].value("Y", 88) };
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
		GoalManager::instance().InformClearDraw();	// 透明度が最大値を超えたことを通達する
	}

	{
		Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos);
		Math::Matrix scaleMat = Math::Matrix::CreateScale(m_size);
		m_mWorld = scaleMat * transMat;
	}
}

void ClearText::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.SetMatrix(m_mWorld);
	Math::Color color = { 1,1,1,m_alpha };
	KdShaderManager::Instance().m_spriteShader.DrawTex(&m_tex, 0, 0, &m_rc, &color);
	KdShaderManager::Instance().m_spriteShader.SetMatrix(Math::Matrix::Identity);
}

void ClearText::DrawImGui()
{
	if (ImGui::CollapsingHeader("ClearText", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat("ClearText m_pos", &m_pos.x, 0.1f);
	}
}
