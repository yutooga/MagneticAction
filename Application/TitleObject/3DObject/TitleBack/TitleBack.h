#pragma once

class TitleBack : public KdGameObject
{
public:
	TitleBack(){}
	~TitleBack()override{}

	void Init()override;

	void DrawUnLit()override;

	static const float k_modelSize;	// モデルのサイズ

private:

	// モデルの実体
	std::shared_ptr<KdModelWork> m_model;
};