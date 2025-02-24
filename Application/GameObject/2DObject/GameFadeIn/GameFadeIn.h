#pragma once

class GameFadeIn : public  KdGameObject
{
public:
	GameFadeIn(){}
	~GameFadeIn()override{}

	void Init()override;

	void DrawSprite()override;

	void Update()override;

	static const float k_alphaMaxAmount;	// 透明度の限界値
	static const float k_addAlphaAmount;	// 透明度の加算量

private:

	// 画像の実体
	KdTexture m_tex;

	// 透明度
	float m_alpha = 0.0f;

	// 画像の切り取り範囲
	Math::Rectangle m_rc = { 0,0,1280,720 };
	Math::Color m_color = { 1,1,1,m_alpha };
};