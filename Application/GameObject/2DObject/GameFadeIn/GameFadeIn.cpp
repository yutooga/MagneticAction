#include "GameFadeIn.h"

const float GameFadeIn::k_alphaMaxAmount = 0.7f;
const float GameFadeIn::k_addAlphaAmount = 0.01f;

void GameFadeIn::Init()
{
	// 画像のロード
	m_tex.Load(m_jsonData["GameFadeIn"]["URL"]);

	// 画像の切り取り範囲の初期化
	m_rc = { 0,0,
		m_jsonData["GameFadeIn"]["Rc"].value("X",1280),
		m_jsonData["GameFadeIn"]["Rc"].value("Y",720) };

	// 透明度の初期化&色の初期化
	m_color = { m_jsonData["GameFadeIn"].value("RgbMax",1.f),
		m_jsonData["GameFadeIn"].value("RgbMax",1.f),
		m_jsonData["GameFadeIn"].value("RgbMax",1.f),m_alpha };
}

void GameFadeIn::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.DrawTex(&m_tex, 0, 0, &m_rc, &m_color);
}

void GameFadeIn::Update()
{
	// 画像の透明度が限界値に達しているなら早期リターン
	if (m_alpha == k_alphaMaxAmount)return;

	// 画像の透明度の更新
	m_alpha += k_addAlphaAmount;

	// 画像の透明度が限界値を超えたら限界値をセットする
	if (m_alpha >= k_alphaMaxAmount)
	{
		m_alpha = k_alphaMaxAmount;
	}

	// 色の更新
	m_color = { m_jsonData["GameFadeIn"].value("RgbMax",1.f),
		m_jsonData["GameFadeIn"].value("RgbMax",1.f),
		m_jsonData["GameFadeIn"].value("RgbMax",1.f),m_alpha };
}
