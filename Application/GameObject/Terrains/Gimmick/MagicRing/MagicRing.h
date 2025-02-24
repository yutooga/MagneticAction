#pragma once
#include"../../TerrainsBase.h"

class MagicRing : public TerrainsBase
{
public:

	MagicRing(){}
	~MagicRing(){}

	void Init()override;

	void Update()override;

	void OnHit([[maybe_unused]] ObjectType _obj)override {};

	ObjectType GetObjType() override { return m_objType; }

	void SetObjType(ObjectType _type);

	//エフェクトセット関数
	void SetEffectName(const std::string& _name) { m_effectName = _name; }

	void DrawBright()override;

	void DrawImGui()override;

	//============================定数関係=============================
	
	static const float k_changeColorTime;	// 魔法人の色が復活するタイミング

	static const float k_colorCoolTime;	// 魔法陣のクールタイム

	static const float k_subtractionAmount;	// 色の減算量

	static const float k_addAmount;	// 色の加算量

	//=================================================================

private:

	//モデルのサイズ
	const float m_modelSize = 5.0f;

	//オブジェクトの種類
	ObjectType m_objType = ObjectType::None;

	//effectの名前格納用
	std::string m_effectName;

	// エフェクトの再出現カウント
	int m_effectReappearanceCnt = 0;

	// 色の透明度
	float m_colorAlpha = 1.0f;
};