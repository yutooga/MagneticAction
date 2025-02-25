#pragma once
#include"../2DGameObjBase.h"

class GameFadeIn : public  _2DGameObjBase
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

};