#pragma once
#include"../../../../Terrains/TerrainsBase.h"

class JointLift : public TerrainsBase
{
public:
	JointLift(){}
	~JointLift()override{}

	enum class MoveDirOption
	{
		X,
		Z,
		None
	};

	void Init()override;

	void Update()override;

	void DrawImGui()override;

	void DrawBright()override {}

	void SetMoveDirOption(const MoveDirOption& _option) { m_dir = _option; }

private:

	// モデルのサイズ
	float m_modelSize = 0.f;

	// 回転角度
	float m_angle = 0.0f;

	// 移動スピード
	float m_moveSpeed = 0.f;

	// 進行方向のオプション
	MoveDirOption m_dir = MoveDirOption::None;
};