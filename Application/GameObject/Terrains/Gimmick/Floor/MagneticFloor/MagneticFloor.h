#pragma once
#include"../../../TerrainsBase.h"

class MaguneticFloor : public TerrainsBase
{
public:
	MaguneticFloor(){}
	~MaguneticFloor()override{}

	void Init()override;

	void Update()override;

	void PostUpdate()override;

	void DrawImGui()override;

	// オブジェクトタイプの取得
	ObjectType GetObjType()override { return ObjectType::MaguneticFloor; }

	// モデルの大きさセット
	void SetModelSize(const float _size = 3.5f) { m_modelSize = _size; }

	// 磁力の及ぶ範囲調整関数
	void SetScope(const float _scope = 20.0f) { m_maguneScope = _scope; }

	// 斥力の補正値調整関数
	void SetAdjustAdValue(const float _value) { m_adjustAdValue = _value; }

	//=================================================================
		
	static const float k_oppositionPower;	// 反発力の大きさ
	
	static const float k_adsorptionPower;	// 斥力の大きさ
		
	static const float k_adjustValue;	// 補正値

	static const float k_oppositionLimit;	// 反発の継続時間の限界

	//=================================================================

private:

	//プレイヤーとの当たり判定
	void MaguneScope();

	void PlayerReaction();

	void OnHit(std::weak_ptr<KdGameObject> _obj);

	// モデルのサイズ
	float m_modelSize = 0.f;

	// 現在の状態
	enum class State
	{
		Opposition,
		Adsorption,
		NoState
	};

	// 現在の状態を保持する変数
	State m_nowState = State::NoState;

	// オブジェクトの実態
	std::weak_ptr<KdGameObject> m_obj;

	// 反発力
	float m_oppoPow = 0.f;

	// 斥力
	float m_adPow = 0.f;

	// 斥力の補正値
	float m_adjustAdValue = 0.0f;

	// 磁力の及ぶ範囲
	float m_maguneScope = 0.0f;
};