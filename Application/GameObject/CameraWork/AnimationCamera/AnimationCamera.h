#pragma once

class GameScene;

class AnimationCamera : public KdGameObject
{
public:
	AnimationCamera(){}
	~AnimationCamera()override{}

	void Init()override;

	void Update()override;

	// 情報のセット
	void SetAnimationInfo(GameScene* scene,
		const std::shared_ptr<KdGameObject>& start,
		const std::shared_ptr<KdGameObject>& end, float spd = 0.01f);

	static const float k_end;	// 進行の終わり

private:

	// 現在のシーン
	GameScene* m_pNowScene = nullptr;

	//スタート地点とゴール地点のオブジェクト
	std::weak_ptr<KdGameObject> m_wpStart;
	std::weak_ptr<KdGameObject> m_wpEnd;

	//進行具合（0~1）
	float m_progress = 0.0f;

	// 進行スピード
	float m_speed = 0.01f;
};