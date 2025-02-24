#pragma once
#include "../2DTitleObjBase.h"

class StartBar : public _2DTitleObjBase
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

	// スタートボタンの点滅処理
	void Flashing();

	// スタート押される前か判断するフラグ
	bool m_startFlg = false;

	// 透明度の加算量
	float m_alphaAdd = 0.f;

	// おしっぱ制御フラグ
	bool m_pushFlg = true;
};