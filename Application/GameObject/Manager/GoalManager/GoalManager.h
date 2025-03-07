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
	// クリアテキストができったことを登録する関数
	void InformClearDraw() { m_clearTextFlg = true; }

	//クリア状態取得関数
	const bool GetClearState()const { return m_clearFlg; }
	//次のシーンに進んでよいかを知るための関数
	const bool GetNextSceneFlg()const { return m_nextSceneFlg; }
	// 現在のクリアテキストの状態を取得する関数
	const bool GetClearTextFlg()const { return m_clearTextFlg; }

	//状態リセット関数
	void ResetAllState() 
	{ 
		m_clearFlg = false; 
		m_nextSceneFlg = false;
		m_clearTextFlg = false;
	}

	//ゴールの位置設定関数
	void SetGoalPos(const Math::Vector3& _pos) { m_goalPos = _pos; }
	//ゴールの位置取得関数
	const Math::Vector3 GetGoalPos()const { return m_goalPos; }
	

private:
	GoalManager() = default;
	~GoalManager() = default;

	// クリアのテクスチャが完全に表示されたかどうか判断するフラグ
	bool m_clearTextFlg = false;

	//クリア状態
	bool m_clearFlg = false;
	//次のシーンに進んでいいかどうか
	bool m_nextSceneFlg = false;
	//ゴールの位置
	Math::Vector3 m_goalPos = Math::Vector3::Zero;
};