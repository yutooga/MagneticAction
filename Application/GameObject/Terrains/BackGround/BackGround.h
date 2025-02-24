#pragma once
#include"../TerrainsBase.h"

class BackGround : public TerrainsBase
{
public:
	BackGround(){}
	~BackGround(){}

	void Init()override;

	void Update()override;

	void DrawUnLit()override;

	enum class Option
	{
		NormalMode,
		RotationMode,
		None
	};

	void SetOption(const Option& _option) { m_nowMode = _option; }

	// 回転角度の加算量
	static const float k_addAngleValue;

	static const float k_modelSize;

private:

	// 現在の状態の保存関数
	Option m_nowMode = Option::None;

	float m_angle = 0.0f;
};