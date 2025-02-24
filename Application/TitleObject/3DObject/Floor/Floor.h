#pragma once

class Floor : public KdGameObject
{
public:
	Floor(){}
	~Floor()override{}

	void Init()override;

	void DrawLit()override;

private:

	// モデルの実体
	std::shared_ptr<KdModelWork> m_model;
};