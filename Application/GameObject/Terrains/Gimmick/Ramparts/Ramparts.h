#pragma once
#include"../../TerrainsBase.h"

class Ramparts : public TerrainsBase
{
public:
	Ramparts(){}
	~Ramparts()override{}

	void Init()override;

	void Update()override;

	void DrawBright()override{}

	void DrawImGui()override;

	static const float k_rotAngle;	// モデルの回転角度

private:

	// モデルの大きさ
	float m_modelSize = 16.5f;

};