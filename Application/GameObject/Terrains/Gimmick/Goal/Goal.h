#pragma once
#include"../../TerrainsBase.h"

class Goal : public TerrainsBase
{
public:
	Goal(){}
	~Goal()override{}

	void Init()override;

	void Update()override;

	void DrawBright()override{}

	void DrawImGui()override;

	ObjectType GetObjType()override{ return ObjectType::Goal; }

private:

	// モデルの大きさ
	float m_modelSize = 0.f;

};