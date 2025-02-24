#include "Hp.h"

const float HP::k_addTimeAmount = 0.016f;
const float HP::k_tremorSpeed = 5.f;
const float HP::k_tremorRange = 40.f;
const float HP::k_tremorLimit = 0.5f;

void HP::Init()
{
	// 画像のロード
	m_tex.Load("Asset/Textures/Scene/GameScene/2DObject/Hp/Hp.png");

	// 座標の初期化
	m_pos = k_firstPos;
}

void HP::Update()
{
	// ダメージを受けた時ハートが揺れる
	if (m_damageFlg)
	{
		m_time += k_addTimeAmount; // おおよそ60FPSで更新

		// 正弦波を使って揺れを計算
		float offsetX = std::sin(m_time * k_tremorRange) * k_tremorSpeed; // 揺れの振幅と速度を調整
		float offsetY = std::cos(m_time * k_tremorRange) * k_tremorSpeed; // 揺れの振幅と速度を調整

		m_pos = { k_firstPos.x + offsetX, k_firstPos.y + offsetY, 0 };

		// 揺れの継続時間を過ぎたら揺れを止める
		if (m_time >= k_tremorLimit)
		{
			m_time = 0.0f;
			m_damageFlg = false;
		}
	}
	else
	{
		// 揺れが収まったら元の座標に戻る
		m_pos = k_firstPos;
		m_time = 0.0f;
	}

	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix scaleMat = Math::Matrix::CreateScale(k_scale);
	m_mWorld = scaleMat * transMat;
}

void HP::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.SetMatrix(m_mWorld);
	Math::Rectangle rec = { 0,11 * (6 - m_nowHp),66,11 };
	KdShaderManager::Instance().m_spriteShader.DrawTex(&m_tex, 0, 0, &rec);
	KdShaderManager::Instance().m_spriteShader.SetMatrix(Math::Matrix::Identity);
}

void HP::DecreaseHp()
{
	// HPの数を減らす
	m_nowHp--;

	// ダメージを受けた状態にする
	m_damageFlg = true;

	// SE再生
	KdAudioManager::Instance().Play("Asset/Sounds/GameScene/2DObject/Hp/Damage.wav", false);
}
