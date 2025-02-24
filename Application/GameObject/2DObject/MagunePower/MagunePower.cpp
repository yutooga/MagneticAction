#include "MagunePower.h"

const float MagunePower::k_rcMax = 11.0f;
const float MagunePower::k_rotAngleMax = 180.f;
const float MagunePower::k_addEffectCnt = 0.5f;

void MagunePower::Init()
{
	// 画像のロード
	m_effectTex.Load("Asset/Textures/Scene/GameScene/2DObject/Effect/Effect.png");

	m_nPowerTex.Load("Asset/Textures/Scene/GameScene/2DObject/MagunePower/nPower.png");
	m_sPowerTex.Load("Asset/Textures/Scene/GameScene/2DObject/MagunePower/sPower.png");
	m_noPowerTex.Load("Asset/Textures/Scene/GameScene/2DObject/MagunePower/NoPower.png");

	// 画像の座標設定
	m_texturePos = { -512,-245,0 };

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

		Math::Matrix transMat = Math::Matrix::CreateTranslation(m_texturePos);
		Math::Matrix scaleMat = Math::Matrix::CreateScale(m_powerTexSize);
		Math::Matrix rotMat = Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(m_angle));
		m_powerMat = scaleMat * rotMat * transMat;
	}
	{

		Math::Matrix transMat = Math::Matrix::CreateTranslation(m_texturePos);
		Math::Matrix scaleMat = Math::Matrix::CreateScale(m_effectTexSize);
		m_effectMat = scaleMat * transMat;
	}
}

void MagunePower::PostUpdate()
{
	// もしプレイヤーのまとっている磁極が変わったならテキストの更新をする
	if (m_nowPower != m_wpPlayer.lock()->GetMaguneForce())
	{
		// 回転状態を初期状態に戻す
		m_textRotFlg = RotationDirection::Noturn;

		// プレイヤーの現在帯びている磁極を取得する
		m_nowPower = m_wpPlayer.lock()->GetMaguneForce();

		// テキスト更新をONにする
		m_textFlg = true;
	}
}

void MagunePower::DrawSprite()
{
	// プレイヤーの実体がないなら早期リターン
	if (m_wpPlayer.expired() != false)return;

	// エフェクト更新
	if(m_effectFlg)
	{
		KdShaderManager::Instance().m_spriteShader.SetMatrix(m_effectMat);
		KdShaderManager::Instance().m_spriteShader.DrawTex(&m_effectTex, 0, 0, &m_effcRc);
		KdShaderManager::Instance().m_spriteShader.SetMatrix(Math::Matrix::Identity);
	}

	// まとっている磁極の表示(回転を伴うので行列をセット)
	KdShaderManager::Instance().m_spriteShader.SetMatrix(m_powerMat);
	Math::Rectangle rc = { 0,0,1000,750 };
	if (m_wpPlayer.lock()->GetMaguneForce() == MagunePowerN && m_textRotFlg == RotationDirection::Finish)
	{
		KdShaderManager::Instance().m_spriteShader.DrawTex(&m_nPowerTex, 0, 0, &rc);
	}
	else if (m_wpPlayer.lock()->GetMaguneForce() == MagunePowerS && m_textRotFlg == RotationDirection::Finish)
	{
		KdShaderManager::Instance().m_spriteShader.DrawTex(&m_sPowerTex, 0, 0, &rc);
	}
	
	KdShaderManager::Instance().m_spriteShader.DrawTex(&m_noPowerTex, 0, 0, &rc);

	KdShaderManager::Instance().m_spriteShader.SetMatrix(Math::Matrix::Identity);
}

void MagunePower::DrawImGui()
{
	if (ImGui::CollapsingHeader("MagunePower", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat3("MagunePower Pos", &m_texturePos.x, 0.1f);
		ImGui::DragFloat("MagunePower texSize", &m_powerTexSize, 0.01f);
		ImGui::DragFloat("MagunePower effectSize", &m_effectTexSize, 0.01f);
		ImGui::DragInt("MagunePower speed", &RotSpeed, 1);
	}
}

void MagunePower::EffectRcUpdate()
{
	switch (m_nowPower)
	{
	case MagunePowerS:
		m_effcRc = { 64 * (int)m_rcCnt, 128, 64, 64 };
		break;
	case MagunePowerN:
		m_effcRc = { 64 * (int)m_rcCnt, 0, 64, 64 };
		break;
	default:
		break;
	}
}

void MagunePower::TextRotUpdate()
{
	// 回転方向が無し or 終了状態　の時は早期リターン
	if (m_textRotFlg == RotationDirection::Noturn || m_textRotFlg == RotationDirection::Finish)return;

	if (m_textRotFlg == RotationDirection::Rightturn)
	{
		m_angle -= RotSpeed;

		// 回転が終わったら終了状態にする
		if (m_angle <= 0)
		{
			m_angle = 0;
			m_textRotFlg = RotationDirection::Finish;
		}
	}
	else if (m_textRotFlg == RotationDirection::Leftturn)
	{
		m_angle += RotSpeed;

		// 回転が終わったら終了状態にする
		if (m_angle >= k_rotAngleMax)
		{
			m_angle = k_rotAngleMax;
			m_textRotFlg = RotationDirection::Finish;
		}
	}
}

void MagunePower::TextUpdate()
{
	// プレイヤーが存在しないときは早期リターン
	if (m_wpPlayer.expired() != false)return;

	if (m_wpPlayer.lock()->GetMaguneForce() == NoForce)
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
	else if (m_wpPlayer.lock()->GetMaguneForce() == MagunePowerS)
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
	else if (m_wpPlayer.lock()->GetMaguneForce() == MagunePowerN)
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
