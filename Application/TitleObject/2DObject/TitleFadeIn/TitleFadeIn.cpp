#include "TitleFadeIn.h"

const float TitleFadeIn::k_addAmount = 0.01f;
const float TitleFadeIn::k_alphaMax = 1.0f;

void TitleFadeIn::Init()
{
	// 画像のロード
	m_tex.Load(m_jsonData["TitleFadeIn"]["URL"]);

	//画像の表示範囲
	m_rectAngle = { 0,0,m_jsonData["TitleFadeIn"].value("RcX",1280),m_jsonData["TitleFadeIn"].value("RcY",720)};
}

void TitleFadeIn::Update()
{
	// フェードインが開始されていないなら更新をしない
	if (!m_FadeInFlg)return;

	// フェードインし終わったら更新をやめる
	else if (m_alpha >= k_alphaMax)return;

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
	KdShaderManager::Instance().m_spriteShader.DrawTex(&m_tex, 0, 0, & m_rectAngle, &color);
}
