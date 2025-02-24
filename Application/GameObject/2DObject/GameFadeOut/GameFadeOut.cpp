#include "GameFadeOut.h"

const float GameFadeOut::k_reduceAlphaAmount = 0.01f;

void GameFadeOut::Init()
{
	// 画像の読み込み
	m_tex.Load("Asset/Textures/Scene/GameScene/2DObject/GameFadeOut/Brack.png");
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
	color = { 1,1,1,m_alpha };
}

void GameFadeOut::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.DrawTex(&m_tex, 0, 0,&rc,&color);
}
