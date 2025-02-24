#pragma once
#include"../../TerrainsBase.h"

class Gate : public TerrainsBase
{
public:
	Gate(){}
	~Gate(){}

	void Init()override;

	void Update()override;

	void DrawImGui()override;

	void DrawBright()override{}

	static const int k_moveMax = 80;	// 門の開く動作の限界値

private:

	// 門を開ける動作
	void GateOpen();

	// モデルのサイズ
	float m_modelSize = 16.5f;

	// 門が開く動作のカウント
	int m_count = 0;

	// SE再生フラグ
	bool m_seFlg = false;
};