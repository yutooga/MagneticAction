#pragma once
#include "../2DTitleObjBase.h"

class TitleFadeIn : public _2DTitleObjBase
{
public:
	TitleFadeIn() {}
	~TitleFadeIn()override{}

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

	// フェードイン開始判断フラグ
	bool m_FadeInFlg = false;
};