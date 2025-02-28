#include "GoalManager.h"
#include"../../2DObject/ClearText/ClearText.h"
#include"../../2DObject/GameFadeIn/GameFadeIn.h"
#include"../../../Scene/SceneManager.h"

void GoalManager::CreateClearText()
{
	//BGMの初期化
	KdAudioManager::Instance().StopAllSound();
	KdAudioManager::Instance().Play("Asset/Sounds/GameScene/SE/Clear/clear.wav",true);

	//===================================
	// フェードイン初期化
	//===================================
	{
		std::shared_ptr<GameFadeIn> fadein = std::make_shared<GameFadeIn>();
		fadein->Init();
		SceneManager::Instance().AddObject(fadein);
	}
	
	//===================================
	// ClearText初期化
	//===================================
	{
		std::shared_ptr<ClearText> clear = std::make_shared<ClearText>();
		clear->Init();
		SceneManager::Instance().AddObject(clear);
	}
}
