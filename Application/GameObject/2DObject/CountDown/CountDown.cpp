#include "CountDown.h"
#include"../../Manager/StartManager/StartManager.h"

const float CountDown::k_reduceAmount = 0.1f;
const float CountDown::k_textNormalSize = 1.0f;

void CountDown::Init()
{
	// 各画像の読み込み&切り取り範囲の初期化
	std::shared_ptr<KdTexture> three = std::make_shared<KdTexture>();
	three->Load("Asset/Textures/Scene/GameScene/2DObject/CountDown/Three.png");
	m_number.push_back(three);

	Math::Rectangle rc3 = { 0,0,150,224 };
	m_rect.push_back(rc3);

	std::shared_ptr<KdTexture> two = std::make_shared<KdTexture>();
	two->Load("Asset/Textures/Scene/GameScene/2DObject/CountDown/Two.png");
	m_number.push_back(two);

	Math::Rectangle rc2 = { 0,0,174,224 };
	m_rect.push_back(rc2);

	std::shared_ptr<KdTexture> one = std::make_shared<KdTexture>();
	one->Load("Asset/Textures/Scene/GameScene/2DObject/CountDown/One.png");
	m_number.push_back(one);

	Math::Rectangle rc1 = { 0,0,108,224 };
	m_rect.push_back(rc1);

	std::shared_ptr<KdTexture> start = std::make_shared<KdTexture>();
	start->Load("Asset/Textures/Scene/GameScene/2DObject/CountDown/START.png");
	m_number.push_back(start);

	Math::Rectangle rcStart = { 0,0,734,226 };
	m_rect.push_back(rcStart);
}

void CountDown::Update()
{
	//サイズが０になったら次の画像にする
	if (m_size <= 0)
	{
		// 各パラメータの初期化
		m_changeCnt = 0.0f;
		m_size = k_textNormalSize;
		m_angle = 0.0f;
		m_nowTextureCnt++;
	}

	// SE再生フラグがONなら鳴らす
	if (!m_seFlg)
	{
		KdAudioManager::Instance().Play("Asset/Sounds/GameScene/2DObject/CountDown/CountDown.wav",false);
		m_seFlg = true;
	}

	//最後の画像が終わったら消去
	if (m_nowTextureCnt > k_textMaxAmount)
	{
		m_isExpired = false;
		m_endFlg = true;
		StartManager::instance().SetStartState(true);
		return;
	}

	// 次の画像に切り替わるまでのクールタイム
	if (m_changeCnt < k_changeCoolTime)
	{
		m_changeCnt++;
	}

	else
	{
		// 画像の回転の更新
		m_angle++;
		// 回転角度が限界値を超えたら、限界値分引く
		if (m_angle >= k_rotAngleMax)
		{
			m_angle -= k_rotAngleMax;
		}

		// 画像の縮小処理
		m_size -= k_reduceAmount;
	}

	Math::Matrix scaleMat = Math::Matrix::CreateScale(m_size);
	Math::Matrix rotMat = Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(m_angle));
	m_mWorld = scaleMat * rotMat;
}

void CountDown::DrawSprite()
{
	// 画像の更新終了しているなら描画しない
	if (m_endFlg)return;

	KdShaderManager::Instance().m_spriteShader.SetMatrix(m_mWorld);
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_number[m_nowTextureCnt], 0, 0, &m_rect[m_nowTextureCnt]);
	KdShaderManager::Instance().m_spriteShader.SetMatrix(Math::Matrix::Identity);
}
