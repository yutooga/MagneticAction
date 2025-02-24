#include "TitleFadeIn.h"

const float TitleFadeIn::k_addAmount = 0.01f;
const float TitleFadeIn::k_alphaMax = 1.0f;

void TitleFadeIn::Init()
{
	// 画像のロード
	m_tex.Load("Asset/Textures/Scene/TitleScene/TitleFadeIn/Brack.png");
}

void TitleFadeIn::Update()
{
	// フェードイン開始フラグがfalseなら早期リターン
	if (!m_FadeInFlg)return;

	// 透明度の加算処理
	m_alpha += k_addAmount;

	// 透明度が限界値を超えないようにする
	if (m_alpha >= k_alphaMax)
	{
		m_alpha = k_alphaMax;
	}
}

void TitleFadeIn::DrawSprite()
{
	Math::Color color = { k_alphaMax,k_alphaMax,k_alphaMax,m_alpha };
	KdShaderManager::Instance().m_spriteShader.DrawTex(&m_tex, 0, 0, & m_rectAngle, & color);
}
