#pragma once

class GameOver : public KdGameObject
{
public:
	GameOver(){}
	~GameOver(){}

	void Init()override;

	void Update()override;

	void DrawSprite()override;

	// 描画終了状態取得関数
	const bool GetDrawEnd()const { return m_drawEndFlg; }


	// ==========定数関係============================================================

	static const float k_addAlphaAmount;	// 透明度の加算量
	static const float k_alphaMaxAmount;	// 透明度の限界値
	static const float k_blackAlphaMaxAmount;	// 黒の画像の透明度の限界値

	// ==============================================================================

private:

	// 画像の実体
	KdTexture m_textTex;
	KdTexture m_blackTex;
	KdTexture m_enterTex;

	// 画像の座標
	Math::Vector3 m_pos = Math::Vector3::Zero;
	Math::Vector3 m_enterPos = Math::Vector3::Zero;

	// 行列
	Math::Matrix m_brackMat = Math::Matrix::Identity;
	Math::Matrix m_enterMat = Math::Matrix::Identity;

	// 画像のサイズ
	float m_textSize = 2.0f;

	// 透明度
	float m_alpha = 0.0f;

	// 黒の画像の透明度
	float m_blackAlpha = 0.0f;

	//描画終了フラグ
	bool m_drawEndFlg = false;
};