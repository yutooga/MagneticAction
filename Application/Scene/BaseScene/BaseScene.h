#pragma once

class BaseScene
{
public :

	BaseScene() { Init(); }
	virtual ~BaseScene() {}

	void PreUpdate();
	virtual void Update();
	void PostUpdate();

	void PreDraw();
	void Draw();
	void DrawSprite();
	void DrawDebug();

	virtual void DrawImGui() {};

	// オブジェクトリストを取得
	const std::list<std::shared_ptr<KdGameObject>>& GetObjList()
	{
		return m_objList;
	}

	const std::list<std::shared_ptr<KdGameObject>>& GetGimmickList()
	{
		return m_gimmickList;
	}
	
	// オブジェクトリストに追加
	void AddObject(const std::shared_ptr<KdGameObject>& obj)
	{
		m_objList.push_back(obj);
	}

	//kesu
	void AddGimmickObject(const std::shared_ptr<KdGameObject>& obj)
	{
		m_gimmickList.push_back(obj);
	}

protected :

	// 継承先シーンで必要ならオーバーライドする
	virtual void Event();
	virtual void Init();

	// 全オブジェクトのアドレスをリストで管理
	std::list<std::shared_ptr<KdGameObject>> m_objList;

	//消す
	std::list<std::shared_ptr<KdGameObject>> m_gimmickList;

	bool m_pushEnterFlg = true;
};
