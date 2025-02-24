#pragma once
#include"../2DObjectBase.h"

class ResultText : public _2DObjectBase
{
public:
	ResultText(){}
	~ResultText()override{}

	void Init()override;

	void Update()override;

	void DrawImGui()override;

	static const float k_inPlace;	// 所定の位置

private:
};