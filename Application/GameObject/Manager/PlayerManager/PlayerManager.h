#pragma once

class Player;

class PlayerManager
{
public:
	static PlayerManager& Instance()
	{
		static PlayerManager instance;
		return instance;
	}

	// プレイヤーの実体取得関数
	const std::weak_ptr<Player>& GetPlayer()const;

	// プレイヤーの実体セット関数
	const void SetPlayer(std::shared_ptr<Player>& _player);

	// プレイヤーの座標取得関数
	const DirectX::SimpleMath::Vector3& GetPlayerPos()const { return m_playerPos; }

	// プレイヤーの座標を取得し続ける関数
	void Update();

private:
	PlayerManager() = default;
		~PlayerManager() = default;

	std::mutex m_mutex; // データの一貫性を保つ

	// プレイヤーの実体
	std::weak_ptr<Player> m_player;

	// プレイヤーの座標保存変数
	DirectX::SimpleMath::Vector3 m_playerPos;
};