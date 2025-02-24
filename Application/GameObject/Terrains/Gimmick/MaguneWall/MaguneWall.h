#pragma once
#include"../MaguneFloorBase/MaguneFloorBase.h"

class MaguneWall : public MagneFloorBase
{
public:
	MaguneWall(){}
	~MaguneWall()override {}

	void Init()override;

	void Update()override;

	void PostUpdate()override;

	void DrawImGui()override;

	static const float k_modelSize;	// モデルの大きさ

	static const float k_area;	// プレイヤーに影響を与えるエリア

private:

	// プレイヤーの受ける影響の処理
	void PlayerReaction()override;
};