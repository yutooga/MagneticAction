#pragma once

class StartBar : public KdGameObject
{
public:
	StartBar() {}
	~StartBar()override{}

	void Init()override;

	void DrawSprite()override;

	void Update()override;

	static const float k_alphaMax;	// 透明度の最大値

	static const float k_alphaMin;	// 透明度の最低値

private:

	// 画像の実体
	KdTexture m_tex;

	// スタート押される前か判断するフラグ
	bool m_startFlg = false;

	// 透明度
	float m_alpha = 0.0f;

	// 透明度の加算量
	float m_alphaAdd = 0.01f;

	// おしっぱ制御フラグ
	bool m_pushFlg = true;

	Math::Rectangle m_rectAngle = { 0,0,1027,114 };
};