#include "GameFadeOut.h"

const float GameFadeOut::k_reduceAlphaAmount = 0.01f;

void GameFadeOut::Init()
{
	// 画像の読み込み
	m_tex.Load(m_jsonData["GameFadeOut"]["URL"]);

	// 透明度の初期化
	m_alpha = m_jsonData["GameFadeOut"].value("Alpha", 1.0f);

	// 画像の切り取り範囲の初期化
	m_rc = { 0,0,
		m_jsonData["GameFadeOut"]["Rc"].value("X",1280),
		m_jsonData["GameFadeOut"]["Rc"].value("Y",720) };

	// 色の初期化
	m_color = { m_jsonData["GameFadeOut"].value("RgbMax", 1.0f),
		m_jsonData["GameFadeOut"].value("RgbMax", 1.0f),
		m_jsonData["GameFadeOut"].value("RgbMax", 1.0f),m_alpha };
}

void GameFadeOut::Update()
{
	// 画像の減算処理
	m_alpha -= k_reduceAlphaAmount;

	// 透明度が０になったら存在を消す
	if (m_alpha)
	{
		m_isExpired = true;
	}

	// 画像の色の更新処理
	m_color = { m_jsonData["GameFadeOut"].value("RgbMax", 1.0f),
		m_jsonData["GameFadeOut"].value("RgbMax", 1.0f),
		m_jsonData["GameFadeOut"].value("RgbMax", 1.0f),m_alpha };
}

void GameFadeOut::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.DrawTex(&m_tex, 0, 0,&m_rc,&m_color);
}
