#include "ResultText.h"
#include"../Rank/Rank.h"
#include"../../../Scene/SceneManager.h"

const float ResultText::k_inPlace = 230.f;

void ResultText::Init()
{
	// 画像のロード
	m_tex.Load(m_jsonData["ResultText"]["URL"]);
	m_clickTex.Load(m_jsonData["ResultText"]["Click"]["URL"]);

	// 画像のサイズ
	m_clickTextSize = m_jsonData["ResultText"]["Click"].value("TextureSize", 1.06f);
	
	// 透明度
	m_alpha = k_alphaMax;

	// 画像の色
	m_color = { m_jsonData["ResultText"].value("Color",1.0f),
		m_jsonData["ResultText"].value("Color",1.0f),
		m_jsonData["ResultText"].value("Color",1.0f),m_alpha};

	// 画像の切り取り範囲
	m_rc = { 0,0,m_jsonData["ResultText"].value("RcX",422),m_jsonData["ResultText"].value("RcY",117) };

	m_clickRc = { 0,0, m_jsonData["ResultText"]["Click"]["Rc"].value("X", 403),
		m_jsonData["ResultText"]["Click"]["Rc"].value("Y", 64) };

	// 画像の座標
	m_pos = { m_jsonData["ResultText"].value("PosX",18.f),m_jsonData["ResultText"].value("PosY",436.f),0 };

	m_clickPos = { m_jsonData["ResultText"]["Click"]["Pos"].value("X", 374.7f),
	m_jsonData["ResultText"]["Click"]["Pos"].value("Y", -264.f),0 };

	// ImGui用のランダムなIdの生成
	m_randomId = rand();
}

void ResultText::Update()
{
	//所定の位置に来たなら早期リターン
	if (m_pos.y == k_inPlace)return;

	// ロゴの移動
	m_pos.y--;

	// 所定の位置に来たらランクの画像を生成する
	if (m_pos.y <= k_inPlace)
	{
		m_pos.y = k_inPlace;

		//=====================================
		// ランク初期化
		//=====================================
		std::shared_ptr<Rank> rank = std::make_shared<Rank>();
		rank->Init();
		SceneManager::Instance().AddObject(rank);
	}

	// 行列の確定
	{
		Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos);
		m_mWorld = transMat;
	}

	{
		Math::Matrix transMat = Math::Matrix::CreateTranslation(m_clickPos);
		Math::Matrix scaleMat = Math::Matrix::CreateScale(m_clickTextSize);
		m_clickMat = scaleMat * transMat;
	}
}

void ResultText::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.SetMatrix(m_mWorld);
	KdShaderManager::Instance().m_spriteShader.DrawTex(&m_tex, 0, 0, &m_rc, &m_color);
	KdShaderManager::Instance().m_spriteShader.SetMatrix(Math::Matrix::Identity);

	//所定の位置に来ていない早期リターン
	if (m_pos.y != k_inPlace)return;
	KdShaderManager::Instance().m_spriteShader.SetMatrix(m_clickMat);
	KdShaderManager::Instance().m_spriteShader.DrawTex(&m_clickTex, 0, 0, &m_clickRc);
	KdShaderManager::Instance().m_spriteShader.SetMatrix(Math::Matrix::Identity);
}

void ResultText::DrawImGui()
{
	ImGui::PushID(m_randomId);
	if (ImGui::CollapsingHeader("ResultText", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat("ResultText m_posX", &m_pos.x, 1.0f);
		ImGui::DragFloat("ResultText m_posY", &m_pos.y, 1.0f);
		ImGui::DragFloat3("m_pos", &m_clickPos.x, 0.1f);
		ImGui::DragFloat("m_size", &m_clickTextSize, 0.01f);
	}
	ImGui::PopID();
}
