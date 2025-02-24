#pragma once
#include"../../TerrainsBase.h"

class ObstructiveWall : public TerrainsBase
{
public:
	ObstructiveWall(){}
	~ObstructiveWall()override{}

	void Init() override;

	void Update()override;

	void PostUpdate()override;

	void DrawBright()override;

	void DrawImGui()override;

	enum class ColisionOption
	{
		DifferentForce,
		SameForce,
		WallDistance
	};

private:

	// オブジェクトの纏っている磁極を変化させる処理
	void ChangeMagneForce();

	void ColisionCheck(const float _radius, ColisionOption _option);

	void OnHit(const UINT& _magneForce,const Math::Vector3& _targetPos);

	// モデルの回転角度
	float m_rotAngle = 0.f;

	// 前フレームに纏っていた磁極
	UINT m_beforeMagneForce = NoForce;

	// 磁極が切り替わる間隔
	float m_changeForceCnt = 0;

	// 最初の座標
	Math::Vector3 m_firstPos;

	// オブジェクトの動くスピード
	float m_moveSpeed = 0.0f;

	float m_changeMoment = 0.0f;

};