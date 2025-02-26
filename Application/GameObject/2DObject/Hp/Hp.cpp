#include "Hp.h"

const float HP::k_addTimeAmount = 0.016f;
const float HP::k_tremorSpeed = 5.f;
const float HP::k_tremorRange = 40.f;
const float HP::k_tremorLimit = 0.5f;

void HP::Init()
{
	// 画像のロード
	m_tex.Load(m_jsonData["Hp"]["URL"]);

	// 座標の初期化
	m_pos = {
		m_jsonData["Hp"]["Pos"].value("X",-400.f),
		m_jsonData["Hp"]["Pos"].value("Y",300.f),
		0
	};

	// 最初の座標の保存
	m_previosPos = m_pos;

	// 画像の大きさの初期化
	m_scale = {
		m_jsonData["Hp"]["Scale"].value("X",6.0f),
		m_jsonData["Hp"]["Scale"].value("Y",1.0f),
		m_jsonData["Hp"]["Scale"].value("Z",1.0f)
	};

	// 現在のHP数の初期化
	m_nowHp = m_jsonData["Hp"].value("MaxHp", 6);
}

void HP::Update()
{
	// ハートが揺れる処理
	ShakeHeart();

	m_rc = { 0,
		m_jsonData["Hp"]["Rc"].value("StarttingX",11)* (m_jsonData["Hp"].value("MaxHp",6) - m_nowHp),
		m_jsonData["Hp"]["Rc"].value("X",66) ,
		m_jsonData["Hp"]["Rc"].value("Y",11) };

	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix scaleMat = Math::Matrix::CreateScale(m_scale);
	m_mWorld = scaleMat * transMat;
}

void HP::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.SetMatrix(m_mWorld);
	KdShaderManager::Instance().m_spriteShader.DrawTex(&m_tex, 0, 0, &m_rc);
	KdShaderManager::Instance().m_spriteShader.SetMatrix(Math::Matrix::Identity);
}

void HP::DecreaseHp()
{
	// HPの数を減らす
	m_nowHp--;

	// ダメージを受けた状態にする
	m_damageFlg = true;

	// SE再生
	KdAudioManager::Instance().Play(m_jsonData["Hp"]["Se"]["URL"], false);
}

void HP::ShakeHeart()
{
	if (!m_damageFlg)return;

	// ダメージを受けた時ハートが揺れる
	
	m_time += k_addTimeAmount; // おおよそ60FPSで更新

	// 正弦波を使って揺れを計算
	float offsetX = std::sin(m_time * k_tremorRange) * k_tremorSpeed; // 揺れの振幅と速度を調整
	float offsetY = std::cos(m_time * k_tremorRange) * k_tremorSpeed; // 揺れの振幅と速度を調整

	m_pos = { m_previosPos.x + offsetX, m_previosPos.y + offsetY, 0 };

	// 揺れの継続時間を過ぎたら揺れを止める
	if (m_time >= k_tremorLimit)
	{
		// 揺れが収まったら元の座標に戻る
		m_time = 0.0f;
		m_pos = m_previosPos;
		m_damageFlg = false;
	}
}
