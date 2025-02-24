#pragma once

class TerrainsBase : public KdGameObject
{
public:
	TerrainsBase();
	virtual ~TerrainsBase()override{}

	virtual void DrawLit()override;

	virtual void GenerateDepthMapFromLight() {}

	virtual void DrawBright()override;

	void SetPos(const Math::Vector3& _pos) { m_pos = _pos; }

	virtual void OnHit([[maybe_unused]] ObjectType _obj)override{}

	static const float k_magnetRange; // マグネットの影響範囲
	static const float k_magnetPower; // マグネットの強さ(全てのマグネットは統一するべき)

protected:

	void SetData(nlohmann::json _data) {m_gimmickData = _data;}

	std::shared_ptr<KdModelWork> m_model;

	//違う極同士のとき引き寄せあう範囲内判断フラグ
	bool m_attractFlg = false;

	//imgui用のランダムなID
	int m_randomId;

	static nlohmann::json m_gimmickData;
};