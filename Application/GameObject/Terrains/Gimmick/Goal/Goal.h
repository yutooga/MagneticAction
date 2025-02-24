#pragma once
#include"../../TerrainsBase.h"

class Goal : public TerrainsBase
{
public:
	Goal(){}
	~Goal(){}

	void Init()override;

	void Update()override;

	void DrawBright()override{}

	void DrawImGui()override;

	ObjectType GetObjType() { return ObjectType::Goal; }

	static const float k_modelSize;	// モデルのサイズ

private:

	// 座標をセットしたかどうか判断するフラグ
	bool m_setFlg = false;

};