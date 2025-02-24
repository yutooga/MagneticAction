#pragma once

class TitleFadeIn : public KdGameObject
{
public:
	TitleFadeIn() {}
	~TitleFadeIn() {}

	void Init()override;

	void Update()override;

	void DrawSprite()override;

	// フェードイン処理を開始する関数
	void StartFadeIn() { m_FadeInFlg = true; }

	// 透明度を取得する関数
	const float GetAlpha() const{ return m_alpha; }

	static const float k_addAmount;	// 透明度の加算量

	static const float k_alphaMax;	// 透明度の限界値

private:

	// 画像の実体
	KdTexture m_tex;

	// 透明度
	float m_alpha = 0.0f;

	// フェードインするかどうか判断するフラグ
	bool m_FadeInFlg = false;

	Math::Rectangle m_rectAngle = { 0,0,1280,720 };
};