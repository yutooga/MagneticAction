#pragma once
#include"../../Floor/MagneFloorBase/MagneFloorBase.h"

class MagneWall : public MagneFloorBase
{
public:
	MagneWall(){}
	~MagneWall()override {}

	void Init()override;

	void Update()override;

	void PostUpdate()override;

	void DrawImGui()override;

private:

	// プレイヤーの受ける影響の処理
	void PlayerReaction()override;

	float m_modelSize = 0.f;
};