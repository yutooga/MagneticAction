#include "ResultScene.h"
#include"../../GameObject/Terrains/BackGround/BackGround.h"
#include"../../GameObject/Camera/CameraBase.h"
#include"../../ResultObject/2DObject/ResultText/ResultText.h"
#include"../../ResultObject/2DObject/ClearLife/ClearLife.h"
#include"../../ResultObject/2DObject/RankLogo/RankLogo.h"
#include"../../Scene/SceneManager.h"
#include"../../GameObject/Manager/GoalManager/GoalManager.h"



void ResultScene::DrawImGui()
{
	for (auto& obj : m_objList)
	{
		obj->DrawImGui();
	}
}

void ResultScene::Init()
{

	// 再生中のBGM等初期化
	KdAudioManager::Instance().StopAllSound();

	// リザルトシーンのBGM再生
	KdAudioManager::Instance().Play("Asset/Sounds/ResultScene/BGM/Result.wav");

	// エフェクトをリセット
	KdEffekseerManager::GetInstance().Reset();


	//=====================================
	// 背景初期化
	//=====================================
	{
		std::shared_ptr<BackGround> background = std::make_shared<BackGround>();
		background->Init();
		background->SetOption(BackGround::Option::RotationMode);
		m_objList.push_back(background);
	}

	//=====================================
	// リザルトテキスト初期化
	//=====================================
	{
		std::shared_ptr<ResultText> result = std::make_shared<ResultText>();
		result->Init();
		m_objList.push_back(result);
	}

	//=====================================
	// クリア時のライフ初期化
	//=====================================
	{
		std::shared_ptr<ClearLife> life = std::make_shared<ClearLife>();
		life->Init();
		m_objList.push_back(life);
	}

	//=====================================
	// ランクのロゴ初期化
	//=====================================
	{
		std::shared_ptr<RankLogo> rank = std::make_shared<RankLogo>();
		rank->Init();
		m_objList.push_back(rank);
	}

	//=====================================
	// カメラ初期化
	//=====================================
	{
		std::shared_ptr<CameraBase> camera = std::make_shared<CameraBase>();
		camera->Init();
		m_objList.push_back(camera);
	}
}

void ResultScene::Event()
{
	// スコアが完全に表示されていないなら早期リターン
	if (!(GoalManager::instance().GetNextSceneFlg()))return;

	// クリック押したらタイトルシーンに戻る
	if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) || GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		// 諸々の初期化
		GoalManager::instance().ResetAllState();

		// シーンの切り替え
		SceneManager::Instance().SetNextScene
		(
			SceneManager::SceneType::Title
		);
	}
}
