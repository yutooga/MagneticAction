#pragma once

class _3DObjectBase : public KdGameObject
{
public:
	_3DObjectBase();
	~_3DObjectBase()override {}

	void DrawLit()override;

protected:

	// モデルの実体
	std::shared_ptr<KdModelWork> m_model;

	// jsonファイルの内容を保存する変数
	static nlohmann::json m_jsonData;
};