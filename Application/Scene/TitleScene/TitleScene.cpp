#include "TitleScene.h"
#include "../SceneManager.h"
#include"../../TitleObject/2DObject/Title/Title.h"
#include"../../TitleObject/2DObject/StartBar/StartBar.h"
#include"../../TitleObject/2DObject/TitleFadeIn/TitleFadeIn.h"
#include"../../TitleObject/3DObject/Magnet/Magnet.h"
#include"../../TitleObject/3DObject/Floor/Floor.h"
#include"../../TitleObject/3DObject/TitleBack/TitleBack.h"
#include"../../GameObject/Camera/TPSCamera/TPSCamera.h"
#include"../../Manager/ModelManager/ModelManager.h"

const float TitleScene::k_alphaMax = 1.0f;

void TitleScene::Event()
{

	// クリックで次のシーンへいく
	if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) || GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		if ((!m_pushEnterFlg) && m_fadeIn.expired() == false)
		{
			//フェードイン処理を開始する
			m_fadeIn.lock()->StartFadeIn();
			if(!m_sePlayFlg)
			{
				KdAudioManager::Instance().Play("Asset/Sounds/TitleScene/SE/Click.wav", true);
				m_sePlayFlg = true;
			}

			// おしっぱ制御
			m_pushEnterFlg = true;
		}
	}
	else
	{
		m_pushEnterFlg = false;
	}

	// フェードイン処理が始まっていないならこれより下は処理しない
	if (m_fadeIn.expired())return;
	
	//フェードイン処理が終わったら次のシーンに進む
	if (m_fadeIn.lock()->GetAlpha() >= k_alphaMax)
	{
		SceneManager::Instance().SetNextScene
		(
			SceneManager::SceneType::Game
		);
	}
}

void TitleScene::Init()
{
	//BGM SEの初期化
	KdAudioManager::Instance().StopAllSound();
	KdAudioManager::Instance().Play("Asset/Sounds/TitleScene/BGM/TitleBGM.wav", true);

	//エフェクトの初期化
	KdEffekseerManager::GetInstance().Release();

	//================================
	// マグネット初期化
	//================================
	{
		std::shared_ptr<Magnet> magnet = std::make_shared<Magnet>();
		magnet->Init();
		m_objList.push_back(magnet);
	}

	//================================
	// 床初期化
	//================================
	{
		std::shared_ptr<Floor> floor = std::make_shared<Floor>();
		floor->Init();
		m_objList.push_back(floor);
	}

	//================================
	// 背景初期化
	//================================
	{
		std::shared_ptr<TitleBack> back = std::make_shared<TitleBack>();
		back->Init();
		m_objList.push_back(back);
	}

	//================================
	// タイトル初期化
	//================================
	{
		std::shared_ptr<Title> title = std::make_shared<Title>();
		title->Init();
		m_objList.push_back(title);
	}

	//================================
	// スタートバー初期化
	//================================
	{
		std::shared_ptr<StartBar> start = std::make_shared<StartBar>();
		start->Init();
		m_objList.push_back(start);
	}

	//================================
	// フェードイン初期化
	//================================
	{
		std::shared_ptr<TitleFadeIn> fadein = std::make_shared<TitleFadeIn>();
		fadein->Init();
		m_fadeIn = fadein;
		m_objList.push_back(fadein);
	}

	//=====================================
	// カメラ初期化
	//=====================================
	{
		std::shared_ptr<TPSCamera> camera = std::make_shared<TPSCamera>();
		camera->Init();
		m_objList.push_back(camera);
	}
}
