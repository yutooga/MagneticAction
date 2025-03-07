#pragma once
#include"../2DGameObjBase.h"

class GameOver : public _2DGameObjBase
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
	static const float k_alphaMax;	// 透明度の限界値
	static const float k_blackAlphaMax;	// 黒の画像の透明度の限界値

	// ==============================================================================

private:

	// 色の更新処理
	void ColorUpdate();

	// 画像の実体
	KdTexture m_textTex;
	KdTexture m_blackTex;
	KdTexture m_clickTex;

	// 画像の切り取り範囲
	Math::Rectangle m_blackRc;
	Math::Rectangle enterRc;

	// 表示色
	Math::Color m_blackColor;
	Math::Color m_clickColor;

	// 画像の座標
	Math::Vector3 m_enterPos = Math::Vector3::Zero;

	// 行列
	Math::Matrix m_brackMat = Math::Matrix::Identity;
	Math::Matrix m_clickMat = Math::Matrix::Identity;

	// 黒の画像の透明度
	float m_blackAlpha = 0.0f;

	//描画終了フラグ
	bool m_drawEndFlg = false;
};