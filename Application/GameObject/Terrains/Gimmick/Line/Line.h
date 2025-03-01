#pragma once
#include"../../TerrainsBase.h"

class Line :public TerrainsBase 
{
public:
	Line(){}
	~Line()override{}

	void Init()override;

	void Update()override;

	void DrawBright()override{}

	void DrawImGui()override;

private:

	// モデルの大きさ
	float m_modelSize = 0.f;

	// 回転角度
	float m_rotAngle = 0.f;
};
