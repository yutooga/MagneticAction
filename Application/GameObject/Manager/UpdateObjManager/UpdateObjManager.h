#pragma once

typedef struct KeyObj
{
	UINT maguneforce;
	KdGameObject::KeyObject keynumber;
};

class UpdateObjManager : public KdGameObject
{
public:
	static UpdateObjManager& Instance()
	{
		static UpdateObjManager instance;
		return instance;
	}

	enum
	{
		one = 1,
		two,
		three,
		four,
		five,
		six
	};

	void Init()override;

	const bool GetLiftUpdate()const { return m_woodenLiftUpdateFlg; }

	const bool GetDoorUpdate()const { return m_copperDoorFlg; }

	const KeyObj GetKeyObjects(int _number)const { return m_keyObjects[_number]; }

	void SetMaguneForce(const UINT _force, const int _number) { m_keyObjects[_number].maguneforce = _force; }

	void SetLiftUpdate(const bool _lift) { m_woodenLiftUpdateFlg = _lift; }

	void SetDoorUpdate(const bool _door) { m_copperDoorFlg = _door; }

private:
	UpdateObjManager() = default;
	~UpdateObjManager() = default;
	std::mutex m_mutex; // データの一貫性を保つ

	// リフト更新判断フラグ
	bool m_woodenLiftUpdateFlg = false;

	// 銅の扉更新判断フラグ
	bool m_copperDoorFlg = false;

	KeyObj m_keyObjects[six] = {};
};