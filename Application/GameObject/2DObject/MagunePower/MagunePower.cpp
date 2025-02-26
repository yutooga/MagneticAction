#include "MagunePower.h"

const float MagunePower::k_rcMax = 11.0f;
const float MagunePower::k_rotAngleMax = 180.f;
const float MagunePower::k_addEffectCnt = 0.5f;

void MagunePower::Init()
{
	// 画像のロード
	m_effectTex.Load(m_jsonData["MagunePower"]["Effect"]["URL"]);

	m_nPowerTex.Load(m_jsonData["MagunePower"]["NPower"]["URL"]);
	m_sPowerTex.Load(m_jsonData["MagunePower"]["SPower"]["URL"]);
	m_noPowerTex.Load(m_jsonData["MagunePower"]["NoPower"]["URL"]);

	// 画像の座標設定
	m_pos = { m_jsonData["MagunePower"]["Pos"].value("X",-512.f),
		m_jsonData["MagunePower"]["Pos"].value("Y",245.f),0};

	// 画像の切り取り範囲の初期化
	m_rc = { 0,0,
		m_jsonData["MagunePower"]["Power"]["Rc"].value("X", 1000),
		m_jsonData["MagunePower"]["Power"]["Rc"].value("Y", 750) };

	// 画像の表示サイズの初期化
	m_effectSize = m_jsonData["MagunePower"]["Effect"].value("TextureSize", 0.6f);
	m_powerSize = m_jsonData["MagunePower"]["Power"].value("TextureSize", -0.25f);

	// 回転速度の初期化
	m_rotSpeed = m_jsonData["MagunePower"]["Power"].value("RotSpeed", 9);

	// 現在まとっている磁極の把握
	if (m_wpPlayer.expired() == false)
	{
		m_nowPower = m_wpPlayer.lock()->GetMaguneForce();
	}
}

void MagunePower::Update()
{
	// プレイヤーの実体がなかったら早期リターン
	if (m_wpPlayer.expired() != false)return;

	//// エフェクトの更新
	if (m_effectFlg)
	{
		m_rcCnt += k_addEffectCnt;
		// エフェクトの更新回数上限まできたらエフェクトを消す
		if (m_rcCnt > k_rcMax)
		{
			m_rcCnt = 0;
			m_effectFlg = false;
		}

		// 画像の切り取り範囲更新
		EffectRcUpdate();
	}

	// テキストの更新

	if (m_textFlg)
	{
		TextUpdate();

		TextRotUpdate();
	}

	{

		Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos);
		Math::Matrix scaleMat = Math::Matrix::CreateScale(m_powerSize);
		Math::Matrix rotMat = Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(m_angle));
		m_powerMat = scaleMat * rotMat * transMat;
	}
	{

		Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos);
		Math::Matrix scaleMat = Math::Matrix::CreateScale(m_effectSize);
		m_effectMat = scaleMat * transMat;
	}
}

void MagunePower::PostUpdate()
{
	// もしプレイヤーのまとっている磁極が変わったならテキストの更新をする
	// 変わっていないなら早期リターン
	if (m_nowPower == m_wpPlayer.lock()->GetMaguneForce())return;

	// 回転状態を初期状態に戻す
	m_textRotFlg = RotationDirection::Noturn;

	// プレイヤーの現在帯びている磁極を取得する
	m_nowPower = m_wpPlayer.lock()->GetMaguneForce();

	// テキスト更新をONにする
	m_textFlg = true;
}

void MagunePower::DrawSprite()
{
	// プレイヤーの実体がないなら早期リターン
	if (m_wpPlayer.expired() != false)return;

	// エフェクト
	if(m_effectFlg)
	{
		KdShaderManager::Instance().m_spriteShader.SetMatrix(m_effectMat);
		KdShaderManager::Instance().m_spriteShader.DrawTex(&m_effectTex, 0, 0, &m_effcRc);
		KdShaderManager::Instance().m_spriteShader.SetMatrix(Math::Matrix::Identity);
	}

	// まとっている磁極の表示(回転を伴うので行列をセット)
	KdShaderManager::Instance().m_spriteShader.SetMatrix(m_powerMat);
	switch (m_wpPlayer.lock()->GetMaguneForce())
	{
	case MagunePowerS:
		if ((static_cast<std::underlying_type_t<RotationDirection>>(m_textRotFlg) &
			static_cast<std::underlying_type_t<RotationDirection>>(RotationDirection::Finish)) != 0)
		{
			KdShaderManager::Instance().m_spriteShader.DrawTex(&m_sPowerTex, 0, 0, &m_rc);
		}
		break;
	case MagunePowerN:
	{
		if ((static_cast<std::underlying_type_t<RotationDirection>>(m_textRotFlg) &
			static_cast<std::underlying_type_t<RotationDirection>>(RotationDirection::Finish)) != 0)
		{
			KdShaderManager::Instance().m_spriteShader.DrawTex(&m_nPowerTex, 0, 0, &m_rc);
		}
	}
	break;
	default:
		break;
	}
	
	KdShaderManager::Instance().m_spriteShader.DrawTex(&m_noPowerTex, 0, 0, &m_rc);

	KdShaderManager::Instance().m_spriteShader.SetMatrix(Math::Matrix::Identity);
}

void MagunePower::DrawImGui()
{
	if (ImGui::CollapsingHeader("MagunePower", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat3("MagunePower Pos", &m_pos.x, 0.1f);
		ImGui::DragFloat("MagunePower texSize", &m_powerSize, 0.01f);
		ImGui::DragFloat("MagunePower effectSize", &m_effectSize, 0.01f);
		ImGui::DragInt("MagunePower speed", &m_rotSpeed, 1);
	}
}

void MagunePower::EffectRcUpdate()
{
	switch (m_nowPower)
	{
	case MagunePowerS:
		m_effcRc = { m_jsonData["MagunePower"]["SPower"]["Rc"].value("StarttingX", 64) * (int)m_rcCnt,
			m_jsonData["MagunePower"]["SPower"]["Rc"].value("StarttingY", 128),
			m_jsonData["MagunePower"]["SPower"]["Rc"].value("X", 64),
			m_jsonData["MagunePower"]["SPower"]["Rc"].value("Y", 64) };
		break;
	case MagunePowerN:
		m_effcRc = { m_jsonData["MagunePower"]["NPower"]["Rc"].value("StarttingX", 64) * (int)m_rcCnt,
			m_jsonData["MagunePower"]["NPower"]["Rc"].value("StarttingY", 0),
			m_jsonData["MagunePower"]["NPower"]["Rc"].value("X", 64),
			m_jsonData["MagunePower"]["NPower"]["Rc"].value("Y", 64) };
		break;
	default:
		break;
	}
}

void MagunePower::TextRotUpdate()
{
	// 回転方向が無し or 終了状態　の時は早期リターン

	switch (m_textRotFlg)
	{
	case RotationDirection::Noturn:
		return;	// 何もしない
		break;
	case RotationDirection::Finish:
		return;	// 何もしない
		break;
	case RotationDirection::Rightturn:
		m_angle -= m_rotSpeed;

		// 回転が終わったら終了状態にする
		if (m_angle <= 0)
		{
			m_angle = 0;
			m_textRotFlg = RotationDirection::Finish;
		}
		break;
	case RotationDirection::Leftturn:
		m_angle += m_rotSpeed;

		// 回転が終わったら終了状態にする
		if (m_angle >= k_rotAngleMax)
		{
			m_angle = k_rotAngleMax;
			m_textRotFlg = RotationDirection::Finish;
		}
		break;
	default:
		break;
	}
}

void MagunePower::TextUpdate()
{
	// プレイヤーが存在しないときは早期リターン
	if (m_wpPlayer.expired() != false)return;

	if ((m_wpPlayer.lock()->GetMaguneForce() & NoForce) != 0)
	{
		if (m_angle != 0)
		{
			m_textRotFlg = RotationDirection::Rightturn;
		}

		else if(m_angle == 0)
		{
			m_textRotFlg = RotationDirection::Noturn;
			m_textFlg = false;
		}
	}
	else if ((m_wpPlayer.lock()->GetMaguneForce() & MagunePowerS) != 0)
	{
		if (m_angle == k_rotAngleMax)
		{
			m_textRotFlg = RotationDirection::Finish;
			m_textFlg = false;

			// エフェクト画像の切り取り範囲初期化
			EffectRcUpdate();

			// エフェクト画像の切り取り範囲カウント初期化
			m_rcCnt = 0;

			m_effectFlg = true;
		}

		else if (m_angle != k_rotAngleMax)
		{
			m_textRotFlg = RotationDirection::Leftturn;
		}
	}
	else if ((m_wpPlayer.lock()->GetMaguneForce() & MagunePowerN) != 0)
	{
		if (m_angle == 0)
		{
			m_textRotFlg = RotationDirection::Finish;
			m_textFlg = false;

			// エフェクト画像の切り取り範囲初期化
			EffectRcUpdate();

			// エフェクト画像の切り取り範囲カウント初期化
			m_rcCnt = 0;

			m_effectFlg = true;
		}

		else if (m_angle != 0)
		{
			m_textRotFlg = RotationDirection::Rightturn;
		}

	}
}
