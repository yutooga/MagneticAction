#include "GameFadeIn.h"

const float GameFadeIn::k_alphaMaxAmount = 0.7f;
const float GameFadeIn::k_addAlphaAmount = 0.01f;

void GameFadeIn::Init()
{
	// 画像のロード
	m_tex.Load("Asset/Textures/Scene/GameScene/2DObject/GameFadeOut/Brack.png");
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
	m_color = { 1,1,1,m_alpha };
}
