#pragma once

class GoalManager
{
public:
	static GoalManager& instance()
	{
		static GoalManager instance;
		return instance;
	}

	//クリアテキスト作成関数
	void CreateClearText();

	//ゲームクリア通知関数
	void InformClear() { m_clearFlg = true; }
	//次のシーンに進んでいいことを登録する関数
	void InformNextScene() { m_nextSceneFlg = true; }

	//クリア状態取得関数
	bool GetClearState() { return m_clearFlg; }
	//次のシーンに進んでよいかを知るための関数
	bool GetNextSceneFlg() { return m_nextSceneFlg; }

	//状態リセット関数
	void ResetAllState() 
	{ 
		m_clearFlg = false; 
		m_nextSceneFlg = false;
	}

	//ゴールの位置設定関数
	void SetGoalPos(Math::Vector3& _pos) { m_goalPos = _pos; }
	//ゴールの位置取得関数
	Math::Vector3 GetGoalPos() { return m_goalPos; }
	

private:
	GoalManager() = default;
	~GoalManager() = default;
	std::mutex m_mutex; // データの一貫性を保つ

	//クリア状態
	bool m_clearFlg = false;
	//次のシーンに進んでいいかどうか
	bool m_nextSceneFlg = false;
	//ゴールの位置
	Math::Vector3 m_goalPos = Math::Vector3::Zero;
};