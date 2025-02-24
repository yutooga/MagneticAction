#include "RankLogo.h"

void RankLogo::Init()
{
	// 画像のロード
	m_tex.Load(m_jsonData["RankLogo"]["URL"]);

	// 表示座標の初期化
	m_pos = { m_jsonData["RankLogo"].value("PosX",-454.f),m_jsonData["RankLogo"].value("PosY",108.f) };

	// 表示サイズ
	m_textureSize = m_jsonData["RankLogo"].value("TextureSize", 0.9f);

	// 切り取り範囲
	m_rc = { 0,0,m_jsonData["RankLogo"].value("RcX", 356),m_jsonData["RankLogo"].value("RcY", 119) };

	// 表示色
	m_color = { m_jsonData["RankLogo"].value("Color", 1.0f),
		m_jsonData["RankLogo"].value("Color", 1.0f),m_jsonData["RankLogo"].value("Color", 1.0f) };
}

void RankLogo::Update()
{
	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix scaleMat = Math::Matrix::CreateScale(m_textureSize);
	m_mWorld = scaleMat * transMat;
}

void RankLogo::DrawImGui()
{
	ImGui::PushID(m_randomId);
	if (ImGui::CollapsingHeader("RankLogo", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat2("Pos", &m_pos.x, 1.f);
		ImGui::DragFloat("Size", &m_textureSize, 0.1f);
	}
	ImGui::PopID();
}
