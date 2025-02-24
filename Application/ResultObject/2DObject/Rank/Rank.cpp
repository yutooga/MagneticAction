#include "Rank.h"
#include"../../../Manager/ScoreManager/ScoreManager.h"
#include"../../../GameObject/Manager/GoalManager/GoalManager.h"

const float Rank::k_rotAngleMax = 3600.f;
const float Rank::k_alphaMax = 1.f;
const float Rank::k_addAngleAmount = 30.f;
const float Rank::k_addAlphaAmount = 0.01f;
const float Rank::k_rgbMax = 1.f;

void Rank::Init()
{
	//ランク情報を取得して読み込む画像を決定する
	switch (ScoreManager::instance().GetScore())
	{
	case static_cast<int>(RANK::S)://Sランク
		m_tex.Load(m_jsonData["Rank"]["S"]["URL"]);
		m_rc = { 0,0,m_jsonData["Rank"]["S"].value("RcX",1118),m_jsonData["Rank"]["S"].value("RcY",1280)};
		break;
	case static_cast<int>(RANK::A)://Aランク
		m_tex.Load(m_jsonData["Rank"]["A"]["URL"]);
		m_rc = { 0,0,m_jsonData["Rank"]["A"].value("RcX",1216),m_jsonData["Rank"]["A"].value("RcY",1280) };
		break;
	case static_cast<int>(RANK::B)://Bランク
		m_tex.Load(m_jsonData["Rank"]["B"]["URL"]);
		m_rc = { 0,0,m_jsonData["Rank"]["B"].value("RcX",1121),m_jsonData["Rank"]["B"].value("RcY",1281) };
		break;
	case static_cast<int>(RANK::C)://Cランク
		m_tex.Load(m_jsonData["Rank"]["C"]["URL"]);
		m_rc = { 0,0,m_jsonData["Rank"]["C"].value("RcX",1087),m_jsonData["Rank"]["C"].value("RcY",1280) };
		break;
	case static_cast<int>(RANK::D)://Dランク
		m_tex.Load(m_jsonData["Rank"]["D"]["URL"]);
		m_rc = { 0,0,m_jsonData["Rank"]["D"].value("RcX",1121),m_jsonData["Rank"]["D"].value("RcY",1281) };
		break;
	}
	
	//透明度
	m_alpha = m_jsonData["Rank"].value("Alpha", 0.0f);

	//画像のサイズ
	m_textureSize = m_jsonData["Rank"].value("TextureSize", 0.5f);

	//画像の表示座標
	m_pos = { m_jsonData["Rank"].value("PosX",-361.f),
		m_jsonData["Rank"].value("PosY",-147.f),0};

	//ImGui用のランダムなIdの生成
	m_randomId = rand();
}

void Rank::Update()
{
	//所定の角度まで来たら次のシーンに切り替えるようにする
	if (m_angle == k_rotAngleMax)
	{
		GoalManager::instance().InformNextScene();
		return;
	}

	// 透明度の加算処理
	AddAlpha();

	// 画像の回転処理
	RotationTexture();

	// 行列の確定
	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix scaleMat = Math::Matrix::CreateScale(m_textureSize);
	Math::Matrix rotMat = Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(m_angle));
	m_mWorld = scaleMat * rotMat * transMat;
}

void Rank::DrawImGui()
{
	ImGui::PushID(m_randomId);
	if (ImGui::CollapsingHeader("Rank", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat("m_posX", &m_pos.x, 1.0f);
		ImGui::DragFloat("m_posY", &m_pos.y, 1.0f);
		ImGui::DragFloat("size", &m_textureSize, 0.1f);
		ImGui::DragFloat("angle", &m_angle, 1.0f);
	}
	ImGui::PopID();
}

void Rank::AddAlpha()
{
	// 透明度が最大値まで達したらそれ以降処理しない
	if (m_alpha >= k_alphaMax)return;

	//透明度の更新処理
	m_alpha += k_addAlphaAmount;
	m_color = { k_rgbMax,k_rgbMax,k_rgbMax,m_alpha };

	//一定の値まで来たらその値を超えないようにする
	if (m_alpha >= k_alphaMax)
	{
		m_alpha = k_alphaMax;
	}
}

void Rank::RotationTexture()
{
	// 最大まで回転しきったらそれ以降処理しない
	if (m_angle >= k_rotAngleMax)return;

	// 回転角度の更新処理
	m_angle += k_addAngleAmount;

	// 一定の値まで来たらその値を超えないようにする
	if (m_angle >= k_rotAngleMax)
	{
		m_angle = k_rotAngleMax;

		// 一定ランクの場合追加でSEを鳴らす
		if (ScoreManager::instance().GetScore() == static_cast<int>(RANK::S))
		{
			KdAudioManager::Instance().Play("Asset/Sounds/ResultScene/SE/BigCall.wav");
		}
		else if (ScoreManager::instance().GetScore() == static_cast<int>(RANK::A))
		{
			KdAudioManager::Instance().Play("Asset/Sounds/ResultScene/SE/SmallCall.wav");
		}
	}
}
