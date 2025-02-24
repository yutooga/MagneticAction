#pragma once
#include"../../TerrainsBase.h"

class TargetLift : public TerrainsBase
{
public:
	TargetLift(){}
	~TargetLift(){}

	void Init()override;

	void Update()override;

	void DrawImGui()override;

private:

	// オブジェクトの纏っている磁極を伝える
	void TellToForce();

	// サインカーブ用の角度変数
	float m_angle = 0.0f;
};