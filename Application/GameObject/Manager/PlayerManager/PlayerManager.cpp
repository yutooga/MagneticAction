#include "PlayerManager.h"
#include"../../Character/Player/Player.h"

const std::weak_ptr<Player>& PlayerManager::GetPlayer() const
{
	return m_player;
}

const void PlayerManager::SetPlayer(std::shared_ptr<Player>& _player)
{
	m_player = _player;
}

void PlayerManager::Update()
{
	std::shared_ptr<Player> _spPlayer = m_player.lock();
	
	if (_spPlayer)
	{
		// 座標を更新する
		m_playerPos = _spPlayer->GetPos();
	}
}
