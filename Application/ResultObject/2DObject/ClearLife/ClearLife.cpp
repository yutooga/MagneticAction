#include "ClearLife.h"
#include"../../../Manager/ScoreManager/ScoreManager.h"

void ClearLife::Init()
{
	// 画像のロード
	m_tex.Load(m_jsonData["ClearLife"]["LifeLogo"]["URL"]);

	m_lifeTex.Load(m_jsonData["ClearLife"]["Life"]["URL"]);

	// 表示座標の初期化
	m_pos = {
		m_jsonData["ClearLife"]["LifeLogo"].value("PosX",254.f),
		m_jsonData["ClearLife"]["LifeLogo"].value("PosY",64.1f),
		0
	};

	m_lifePos = {
		m_jsonData["ClearLife"]["Life"].value("PosX", 271.9f),
		m_jsonData["ClearLife"]["Life"].value("PosY", -149.f),
		0
	};

	// 切り取り範囲の初期化

	m_rc = {0,0,m_jsonData["ClearLife"]["LifeLogo"].value("RcX",194),
	m_jsonData["ClearLife"]["LifeLogo"].value("RcY", 117),};

	m_lifeRc = { 0,(11 * (6 - ScoreManager::instance().GetClearHp())),
		m_jsonData["ClearLife"]["Life"].value("RcX", 66),m_jsonData["ClearLife"]["Life"].value("RcY", 11) };

	// 透明度
	m_alpha = m_jsonData["ClearLife"].value("Alpha", 1.0f);

	// 表示色
	m_color = { m_jsonData["ClearLife"].value("Color", 1.0f),
		m_jsonData["ClearLife"].value("Color", 1.0f),m_jsonData["ClearLife"].value("Color", 1.0f) };

	// 表示サイズ
	m_textureSize = m_jsonData["ClearLife"]["LifeLogo"].value("TextureSize", 0.9f);

	m_lifeSize = { m_jsonData["ClearLife"]["Life"].value("TextureSizeX", 9.2f),
		m_jsonData["ClearLife"]["Life"].value("TextureSizeY", 1.7f),0 };

	
}

void ClearLife::Update()
{
	{
		Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos);
		Math::Matrix scaleMat = Math::Matrix::CreateScale(m_textureSize);
		m_mWorld = scaleMat * transMat;
	}

	{
		Math::Matrix transMat = Math::Matrix::CreateTranslation(m_lifePos);
		Math::Matrix scaleMat = Math::Matrix::CreateScale(m_lifeSize);
		m_lifeMat = scaleMat * transMat;
	}
}

void ClearLife::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.SetMatrix(m_mWorld);
	KdShaderManager::Instance().m_spriteShader.DrawTex(&m_tex, 0, 0, &m_rc, &m_color);
	KdShaderManager::Instance().m_spriteShader.SetMatrix(Math::Matrix::Identity);

	KdShaderManager::Instance().m_spriteShader.SetMatrix(m_lifeMat);
	KdShaderManager::Instance().m_spriteShader.DrawTex(&m_lifeTex,0,0, &m_lifeRc, &m_color);
	KdShaderManager::Instance().m_spriteShader.SetMatrix(Math::Matrix::Identity);
}

void ClearLife::DrawImGui()
{
	if (ImGui::CollapsingHeader("ClearLife", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat3("pos", &m_pos.x, 0.1f);
		ImGui::DragFloat("size", &m_textureSize, 0.1f);
		ImGui::DragFloat3("sized", &m_lifeSize.x, 0.1f);
		ImGui::DragFloat2("pos2", &m_lifePos.x, 0.1f);
	}
	ImGui::PopID();
}
