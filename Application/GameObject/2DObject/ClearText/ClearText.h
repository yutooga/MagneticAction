#pragma once
#include"../2DGameObjBase.h"

class ClearText : public _2DGameObjBase
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

};