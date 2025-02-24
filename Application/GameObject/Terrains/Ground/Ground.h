#pragma once
#include"../TerrainsBase.h"

class Ground : public TerrainsBase
{
public:
	Ground(){}
	~Ground(){}

	void Init()override;

	void OnHit([[maybe_unused]] ObjectType _obj)override {};

	void DrawBright()override {}

	// オブジェクトタイプ取得関数
	ObjectType GetObjType()override { return ObjectType::Ground; }

private:
};