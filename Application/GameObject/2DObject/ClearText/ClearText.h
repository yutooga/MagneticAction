#pragma once

class ClearText : public KdGameObject
{
public:
	ClearText(){}
	~ClearText()override{}

	void Init()override;

	void Update()override;

	void DrawSprite()override;

	void DrawImGui()override;

	//=====================定数関係============================================
	
	static const float k_alphaMax;	// 透明度の最大値
	static const float k_addAlphaAmount;	// 透明度の加算量

	//=========================================================================

private:

	// 画像の実体
	KdTexture m_tex;

	// 表示座標
	Math::Vector3 m_pos;

	// 画像のサイズ
	float m_textSize = 1.0f;

	// 透明度
	float m_alpha = 0.0f;
};