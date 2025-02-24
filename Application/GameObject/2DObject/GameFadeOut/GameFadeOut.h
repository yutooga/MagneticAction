#pragma once

class GameFadeOut : public KdGameObject
{
public:
	GameFadeOut(){}
	~GameFadeOut(){}

	void Init()override;

	void Update()override;

	void DrawSprite()override;

	static const float k_reduceAlphaAmount;	// 画像の透明度の減算量

private:

	// 画像の実体
	KdTexture m_tex;

	// 透明度
	float m_alpha = 1.0f;

	// 画像の色
	Math::Color color = { 1,1,1,m_alpha };

	// 画像の切り取り範囲
	Math::Rectangle rc = { 0,0,1280,720 };
};