#pragma once
#include"../../TerrainsBase.h"

class MagneFloorBase :public TerrainsBase
{
public:
	MagneFloorBase(){}
	~MagneFloorBase(){}

	void PostUpdate()override;

	static const float k_adsorptionPower;	// 引き寄せあう力の強さ

	static const float k_oppositionPower;	// 反発の強さ

	static const float k_initialOppoPowerValue;	// 反発力の初期値

	static const float k_OppoStateLimit;	// 反発力の継続時間

protected:

	//プレイヤーとの当たり判定
	virtual void MaguneScope();

	// プレイヤー側の処理が書かれた関数
	virtual void PlayerReaction();

	// 当たられたときの処理
	void OnHit(std::weak_ptr<KdGameObject> _obj);

	//状態
	enum class State
	{
		Opposition,
		Adsorption,
		NormalState
	};

	// 今の状態を保持する変数
	State m_nowState = State::NormalState;
	
	
	std::weak_ptr<KdGameObject> m_obj;

	// 反発する力
	float m_oppoPow = 0.5f;

	// 引き寄せあう力
	float m_adPow = 0.4f;

	// 反発カウント
	int m_oppCnt = 0;
};