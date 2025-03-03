#include "GameScene.h"
#include"../SceneManager.h"
#include"../../../Framework/Effekseer/KdEffekseerManager.h"
#include"../../main.h"
#include"../../GameObject/Camera/TPSCamera/TPSCamera.h"
#include"../../GameObject/Camera/FPSCamera/FPSCamera.h"
#include"../../GameObject/Terrains/Ground/Ground.h"
#include"../../GameObject/Terrains/BackGround/BackGround.h"
#include"../../GameObject/Character/Player/Player.h"
#include"../../GameObject/2DObject/Reticle/Reticle.h"
#include"../../GameObject/2DObject/Hp/Hp.h"
#include"../../GameObject/2DObject/GameFadeOut/GameFadeOut.h"
#include"../../GameObject/2DObject/CountDown/CountDown.h"
#include"../../GameObject/2DObject/Pause/Pause.h"
#include"../../GameObject/2DObject/MagunePower/MagunePower.h"
#include"../../GameObject/Weapon/MagneGun/MagneGun.h"
#include"../../GameObject/Terrains/Gimmick/Wall/MovingWall/MovingWall.h"
#include"../../GameObject/Terrains/Gimmick/JumpingMagnet/JumpingMagnet.h"
#include"../../GameObject/Terrains/Gimmick/MagicRing/MagicRing.h"
#include"../../GameObject/Terrains/Gimmick/Floor/ChangeFloor/ChangeFloor.h"
#include"../../GameObject/Terrains/Scaffold/ShortScaffold/ShortScaffold.h"
#include"../../GameObject/Terrains/Scaffold/LongScaffold/LongScaffold.h"
#include"../../GameObject/Terrains/Gimmick/SavePoint/SavePoint.h"
#include"../../GameObject/Terrains/Gimmick/Goal/Goal.h"
#include"../../GameObject/Terrains/Gimmick/Gate/Gate.h"
#include"../../GameObject/Terrains/Gimmick/Wall/GateWall/GateWall.h"
#include"../../GameObject/Terrains/Gimmick/Floor/MoveMagneFloor/MoveMagneFloor.h"
#include"../../GameObject/Terrains/Gimmick/Floor/MagneticFloor/MagneticFloor.h"
#include"../../GameObject/Terrains/Gimmick/Wall/MagneWall/MagneWall.h"
#include"../../GameObject/Terrains/Gimmick/Rail/Rail.h"
#include"../../GameObject/Terrains/Gimmick/Coil/Coil.h"
#include"../../GameObject/Terrains/Gimmick/Conductor/Conductor.h"
#include"../../GameObject/Terrains/Gimmick/Cage/Cage.h"
#include"../../GameObject/Terrains/Gimmick/Pillar/CopperPillar/CopperPillar.h"
#include"../../GameObject/Terrains/Gimmick/Pillar/GoldPillar/GoldPillar.h"
#include"../../GameObject/Terrains/Gimmick/Lift/WoodenLift/WoodenLift.h"
#include"../../GameObject/Terrains/Gimmick/CopperDoor/CopperDoor.h"
#include"../../GameObject/Terrains/Gimmick/Lift/JointLift/JointLift.h"
#include"../../GameObject/Terrains/Gimmick/Lift/TargetLift/TargetLift.h"
#include"../../GameObject/Terrains/Gimmick/Wall/ObstructiveWall/ObstructiveWall.h"
#include"../../GameObject/Terrains/Gimmick/Line/Line.h"
#include"../../GameObject/Terrains/Gimmick/Wall/StoneWall/StoneWall.h"
#include"../../GameObject/Terrains/Gimmick/MagneBox/MagneBox.h"
#include"../../GameObject/Terrains/Gimmick/MagneticArea/MagneticArea.h"
#include"../../GameObject/Terrains//Gimmick/SignBoard/SignBoard.h"
#include"../../GameObject/Terrains//Gimmick/Barb/Barb.h"
#include"../../GameObject/Terrains/ElectricCurrent/ElectricCurrent.h"
#include"../../GameObject/Weapon/FallingMagneGun/FallingMagneGun.h"
#include"../../GameObject/CameraWork/AnimationCamera/AnimationCamera.h"
#include"../../GameObject/Manager/GoalManager/GoalManager.h"
#include"../../GameObject/Manager/StartManager/StartManager.h"
#include"../../Manager/ScoreManager/ScoreManager.h"
#include"../../GameObject/Manager/GateManager/GateManager.h"
#include"../../Manager/ModelManager/ModelManager.h"
#include"../../GameObject/Manager/UpdateObjManager/UpdateObjManager.h"
#include"../../Manager/FileManager/FileManager.h"

const Math::Vector2 GameScene::k_cursorPos = { 640,360 };

bool GameScene::SetCameraTarget(std::weak_ptr<KdGameObject> _target)
{
	// 対象が有効ならカメラのターゲットに設定
	std::shared_ptr<CameraBase> _spCamera = m_wpCamera.lock();
	std::shared_ptr<KdGameObject> _spTarget = _target.lock();
	if (_spCamera && _spTarget)
	{
		_spCamera->SetTarget(_spTarget);
		return true;
	}

	return false;
}

void GameScene::Update()
{
	if (m_pauseFlg)
	{
		// ゲームを再開する処理
		if (m_pause.expired() != false)return;
	
		if (GetAsyncKeyState('P') & 0x8000)
		{
			if(!m_pushFlg)
			{
				m_pauseFlg = false;
				m_pushFlg = true;
			}
		}
		else
		{
			m_pushFlg = false;
		}

		// マウスを画面中央に固定する処理
		SetCursorPos(static_cast<int>(k_cursorPos.x), static_cast<int>(k_cursorPos.y));
		return;
	}
	// ゴールしてないときのみポーズできる
	else if(!GoalManager::instance().GetClearState())
	{
		//ポーズ状態（にする/を解除する）処理
		if (GetAsyncKeyState('P') & 0x8000)
		{
			if (!m_pushFlg)
			{
				m_pushFlg = true;
				if (!m_pauseFlg)
				{
					KdAudioManager::Instance().Play(m_jsonData["Se"]["Pause"]["URL"], false);

					m_pauseFlg = true;
					std::shared_ptr<Pause> pause = std::make_shared<Pause>();
					pause->Init();
					m_objList.push_back(pause);
					m_pause = pause;
				}
			}
		}
		else
		{
			m_pushFlg = false;
		}
	}

	BaseScene::Update();
}

void GameScene::DrawImGui()
{
	//return;

	for (auto& obj : m_objList)
	{
		obj->DrawImGui();
	}
}

void GameScene::Event()
{
	// エフェクトの更新
	KdEffekseerManager::GetInstance().Update();

	// 次のシーンへの遷移処理
	ChangeNextScene();

	// 門のギミックの開閉アニメーション関数
	OpenGateAnimation();

	// 電磁誘導によるオブジェクトの更新判断
	ElectroMagneticInductionUpdateObject();
}
void GameScene::Init()
{
	// jsonファイルからデータを読み込んでくる
	if (m_jsonData.empty()) {
		std::ifstream ifs("Asset/Data/GameScene/GameSceneData.json");
		if (ifs) {
			ifs >> m_jsonData;
		}
	}

	// エフェクトの初期化
	{
		int width = m_jsonData["Effect"].value("Width", 1280);
		int height = m_jsonData["Effect"].value("Height", 720);
		KdEffekseerManager::GetInstance().Create(width, height);
	}

	// BGM SEの初期化
	KdAudioManager::Instance().StopAllSound();
	KdAudioManager::Instance().Play(m_jsonData["Bgm"]["URL"], true);


	ModelManager::Instance().LoadModelFromCSVAsync(m_jsonData["ModelData"]["URL"]);

	//=====================================
	//　フェードアウト初期化
	//=====================================
	{
		std::shared_ptr<GameFadeOut> out = std::make_shared<GameFadeOut>();
		out->Init();
		m_objList.push_back(out);
	}

	//=====================================
	//　カウントダウン初期化
	//=====================================
	{
		std::shared_ptr<CountDown> count = std::make_shared<CountDown>();
		count->Init();
		m_objList.push_back(count);
	}

	//=====================================
	// 背景初期化
	//=====================================
	{
		std::shared_ptr<BackGround> background = std::make_shared<BackGround>();
		background->Init();
		background->SetOption(BackGround::Option::NormalMode);
		m_objList.push_back(background);
	}

	//=====================================
	// HP初期化
	//=====================================

	std::shared_ptr<HP> spHp;

	{
		std::shared_ptr<HP> hp = std::make_shared<HP>();
		hp->Init();
		m_hp = hp;
		m_objList.push_back(hp);
		spHp = hp;
	}
	
	//=====================================
	// 地面初期化
	//=====================================
	{
		std::shared_ptr<Ground> ground = std::make_shared<Ground>();
		ground->Init();
		m_objList.push_back(ground);
	}

	//=====================================
	// プレイヤー初期化
	//=====================================

	std::shared_ptr<Player> spPlayer;

	{
		std::shared_ptr<Player> player = std::make_shared<Player>();
		player->Init();
		player->SetHp(spHp);
		m_player = player;
		spPlayer = player;
		m_objList.push_back(player);
	}

	//=====================================
	// プレイヤーのまとっている磁極初期化
	//=====================================
	{
		std::shared_ptr<MagunePower> power = std::make_shared<MagunePower>();
		power->SetPlayer(spPlayer);
		power->Init();
		m_objList.push_back(power);
	}

	//=====================================
	// 落ちているマグネット銃初期化
	//=====================================

	std::shared_ptr<FallingMagneGun> spFallingMagneGun;

	{
		std::shared_ptr<FallingMagneGun> fallingGun = std::make_shared<FallingMagneGun>();
		fallingGun->Init();
		fallingGun->SetPlayer(spPlayer);
		m_objList.push_back(fallingGun);
		spFallingMagneGun = fallingGun;
	}

	//=====================================
	// ギミック初期化
	//=====================================

	LoadMoveFloorGimmickParam(m_jsonData["MoveFloorData"]["URL"]);

	LoadGimmickDefaultParam(m_jsonData["DefaultGimmickData"]["URL"]);

	LoadGimmickSpecialParam(m_jsonData["SpecialGimmickData"]["URL"]);

	LoadElectricityEffectObjectParam(m_jsonData["EffectData"]["URL"]);

	//=====================================
	// マグネット銃初期化
	//=====================================
	{
		std::shared_ptr<MagneGun> gun = std::make_shared<MagneGun>();
		gun->Init();
		gun->SetParent(spPlayer);
		spFallingMagneGun->SetMagneGun(gun);
		m_objList.push_back(gun);
	}

	//=====================================
	// カメラ初期化
	//=====================================
	{
		std::shared_ptr<FPSCamera> camera = std::make_shared<FPSCamera>();
		camera->Init();
		camera->SetTarget(spPlayer);
		m_wpCamera = camera;
		m_objList.push_back(camera);
	}
}

void GameScene::LoadGimmickDefaultParam(const std::string& _filePath)
{
	m_gimmickNormalData = FileManager::Instance().GetData(_filePath);

	if (m_gimmickNormalData.has_value())
	{
		for (const auto& row : m_gimmickNormalData.value())
		{
			switch ((int)(row[static_cast<unsigned int>(GimmickData::type)]))
			{
			case 0:
			{
				//=====================================
				// 動く壁初期化
				//=====================================
				std::shared_ptr<MovingWall> wall = std::make_shared<MovingWall>();
				wall->SetPos({ row[static_cast<unsigned int>(GimmickData::posX)],row[static_cast<unsigned int>(GimmickData::posY)],row[static_cast<unsigned int>(GimmickData::posZ)] });
				wall->Init();
				wall->SetSize(MovingWall::Size::Small);
				wall->SetObjType(KdGameObject::ObjectType::MovingWall);
				m_objList.push_back(wall);
				break;
			}
			case 1:
			{
				//=====================================
				// ジャンプ床初期化
				//=====================================
				std::shared_ptr<JumpingMagnet> floor = std::make_shared<JumpingMagnet>();
				floor->Init();
				floor->SetPos({ row[static_cast<unsigned int>(GimmickData::posX)],row[static_cast<unsigned int>(GimmickData::posY)],row[static_cast<unsigned int>(GimmickData::posZ)] });
				floor->SetMaguneForce(static_cast<UINT> (2));
				m_objList.push_back(floor);
				break;
			}
			case 2:
			{
				//=====================================
				// コイル初期化
				//=====================================
				std::shared_ptr<Coil> coil = std::make_shared<Coil>();
				coil->SetPos({ row[static_cast<unsigned int>(GimmickData::posX)],row[static_cast<unsigned int>(GimmickData::posY)],row[static_cast<unsigned int>(GimmickData::posZ)] });
				coil->Init();
				m_objList.push_back(coil);
				break;
			}
			case 3:
			{
				//=====================================
				// 魔法陣初期化 (S極の磁力付与)
				//=====================================
				CreateMagicRing(KdGameObject::ObjectType::MagicRingS, "Simple_Distortion2.efkproj", row[static_cast<unsigned int>(GimmickData::posX)], row[static_cast<unsigned int>(GimmickData::posY)], row[static_cast<unsigned int>(GimmickData::posZ)]);
				break;
			}
			case 4:
			{
				//=====================================
				// 磁力が変わる床初期化
				//=====================================
				CreateChangeFloor(0.0f, row[static_cast<unsigned int>(GimmickData::posX)], row[static_cast<unsigned int>(GimmickData::posY)], row[static_cast<unsigned int>(GimmickData::posZ)], 100.0f);
				break;
			}
			case 5:
			{
				//=====================================
				// 磁力が変わる床初期化
				//=====================================
				CreateChangeFloor(191.8f, row[static_cast<unsigned int>(GimmickData::posX)], row[static_cast<unsigned int>(GimmickData::posY)], row[static_cast<unsigned int>(GimmickData::posZ)], 100.0f);
				break;
			}
			case 6:
			{
				//=====================================
				// 阻害壁初期化(S極の磁力)
				//=====================================
				std::shared_ptr<ObstructiveWall> wall = std::make_shared<ObstructiveWall>();
				wall->SetPos({ row[static_cast<unsigned int>(GimmickData::posX)],row[static_cast<unsigned int>(GimmickData::posY)],row[static_cast<unsigned int>(GimmickData::posZ)] });
				wall->SetMaguneForce(KdGameObject::MagunePowerS);
				wall->Init();
				m_objList.push_back(wall);
				break;
			}
			case 7:
			{
				//=====================================
				// 魔法陣初期化 (N極の磁力付与)
				//=====================================
				CreateMagicRing(KdGameObject::ObjectType::MagicRingN, "Simple_Distortion.efkproj", row[static_cast<unsigned int>(GimmickData::posX)], row[static_cast<unsigned int>(GimmickData::posY)], row[static_cast<unsigned int>(GimmickData::posZ)]);
				break;
			}
			case 8:
			{
				//=====================================
				// 短い足場初期化
				//=====================================
				std::shared_ptr<ShortScaffold> scaffold = std::make_shared<ShortScaffold>();
				scaffold->Init();
				scaffold->SetPos({ row[static_cast<unsigned int>(GimmickData::posX)],row[static_cast<unsigned int>(GimmickData::posY)],row[static_cast<unsigned int>(GimmickData::posZ)] });
				m_objList.push_back(scaffold);
				break;
			}
			case 9:
			{
				//=====================================
				// 返し初期化
				//=====================================
				std::shared_ptr<Barb> barb = std::make_shared<Barb>();
				barb->Init();
				barb->SetPos({ row[static_cast<unsigned int>(GimmickData::posX)],row[static_cast<unsigned int>(GimmickData::posY)],row[static_cast<unsigned int>(GimmickData::posZ)] });
				m_objList.push_back(barb);
				break;
			}
			case 10:
			{
				//=====================================
				// セーブポイント初期化
				//=====================================
				std::shared_ptr<SavePoint> savepoint = std::make_shared<SavePoint>();
				savepoint->Init();
				savepoint->SetPos({ row[static_cast<unsigned int>(GimmickData::posX)],row[static_cast<unsigned int>(GimmickData::posY)],row[static_cast<unsigned int>(GimmickData::posZ)] });
				m_objList.push_back(savepoint);
				break;
			}
			case 11:
			{
				//=====================================
				// ゴール初期化
				//=====================================
				std::shared_ptr<Goal> goal = std::make_shared<Goal>();
				goal->SetPos({ row[static_cast<unsigned int>(GimmickData::posX)],row[static_cast<unsigned int>(GimmickData::posY)],row[static_cast<unsigned int>(GimmickData::posZ)] });
				goal->Init();
				m_objList.push_back(goal);
				break;
			}
			case 12:
			{
				//=====================================
				// 阻害壁初期化(N極の磁力付与)
				//=====================================
				std::shared_ptr<ObstructiveWall> wall = std::make_shared<ObstructiveWall>();
				wall->SetPos({ row[static_cast<unsigned int>(GimmickData::posX)],row[static_cast<unsigned int>(GimmickData::posY)],row[static_cast<unsigned int>(GimmickData::posZ)] });
				wall->SetMaguneForce(KdGameObject::MagunePowerN);
				wall->Init();
				m_objList.push_back(wall);
				break;
			}
			case 13:
			{
				//=====================================
				// 扉初期化
				//=====================================
				std::shared_ptr<Gate> gate = std::make_shared<Gate>();
				gate->Init();
				gate->SetPos({ row[static_cast<unsigned int>(GimmickData::posX)],row[static_cast<unsigned int>(GimmickData::posY)],row[static_cast<unsigned int>(GimmickData::posZ)] });
				m_objList.push_back(gate);
				break;
			}
			case 14:
			{
				//=====================================
				// 扉の壁初期化
				//=====================================
				std::shared_ptr<GateWall> wall = std::make_shared<GateWall>();
				wall->Init();
				wall->SetPos({ row[static_cast<unsigned int>(GimmickData::posX)],row[static_cast<unsigned int>(GimmickData::posY)],row[static_cast<unsigned int>(GimmickData::posZ)] });
				m_objList.push_back(wall);
				break;
			}
			case 15:
			{
				//=====================================
				// 死の床初期化
				//=====================================
				CreateDeathFloor({ row[static_cast<unsigned int>(GimmickData::posX)],
					row[static_cast<unsigned int>(GimmickData::posY)],row[static_cast<unsigned int>(GimmickData::posZ)] },
					DeathFloor::MoveState::nomal, KdGameObject::ObjectType::DeathFloor);
				break;
			}
			case 16:
			{
				//=====================================
				// 線路初期化
				//=====================================
				std::shared_ptr<Line> line = std::make_shared<Line>();
				line->Init();
				line->SetPos({ row[static_cast<unsigned int>(GimmickData::posX)],row[static_cast<unsigned int>(GimmickData::posY)],row[static_cast<unsigned int>(GimmickData::posZ)] });
				m_objList.push_back(line);
				break;
			}
			case 19:
			{
				//=====================================
				// 死の床初期化(磁力床ver)
				//=====================================
				CreateDeathFloor({ row[static_cast<unsigned int>(GimmickData::posX)],
					row[static_cast<unsigned int>(GimmickData::posY)],row[static_cast<unsigned int>(GimmickData::posZ)] },
					DeathFloor::MoveState::change, KdGameObject::ObjectType::MovingDeathFloor,29.f);
				break;
			}
			case 20:
			{
				//=====================================
				// 魔法陣初期化(磁力付与なし)
				//=====================================
				CreateMagicRing(KdGameObject::ObjectType::NoForceRing, "", row[static_cast<unsigned int>(GimmickData::posX)], row[static_cast<unsigned int>(GimmickData::posY)], row[static_cast<unsigned int>(GimmickData::posZ)]);
				break;
			}
			case 22:
			{
				//=====================================
				// 磁力壁初期化
				//=====================================
				std::shared_ptr<MagneWall> wall = std::make_shared<MagneWall>();
				wall->Init();
				wall->SetPos({ row[static_cast<unsigned int>(GimmickData::posX)],row[static_cast<unsigned int>(GimmickData::posY)],row[static_cast<unsigned int>(GimmickData::posZ)] });
				wall->SetMaguneForce(KdGameObject::MagunePowerN);
				m_objList.push_back(wall);
				break;
			}

			case 27:
			{
				//=====================================
				// 短い足場初期化
				//=====================================
				std::shared_ptr<ShortScaffold> scaffold = std::make_shared<ShortScaffold>();
				scaffold->SetModelSize(15.0f);
				scaffold->Init();
				scaffold->SetPos({ row[static_cast<unsigned int>(GimmickData::posX)],row[static_cast<unsigned int>(GimmickData::posY)],row[static_cast<unsigned int>(GimmickData::posZ)] });
				m_objList.push_back(scaffold);
				break;
			}
			case 28:
			{
				//=====================================
				// 動く壁初期化
				//=====================================
				std::shared_ptr<MovingWall> wall = std::make_shared<MovingWall>();
				wall->SetPos({ row[static_cast<unsigned int>(GimmickData::posX)],row[static_cast<unsigned int>(GimmickData::posY)],row[static_cast<unsigned int>(GimmickData::posZ)] });
				wall->Init();
				wall->SetModelSize(1.0f);
				wall->SetSize(MovingWall::Size::Big);
				wall->SetObjType(KdGameObject::ObjectType::BigMovingWall);
				m_objList.push_back(wall);
				break;
			}
			case 40:
			{
				//=====================================
				// 木のリフト初期化
				//=====================================
				std::shared_ptr<WoodenLift> lift = std::make_shared<WoodenLift>();
				lift->SetPos({ row[static_cast<unsigned int>(GimmickData::posX)],row[static_cast<unsigned int>(GimmickData::posY)],row[static_cast<unsigned int>(GimmickData::posZ)] });
				lift->Init();
				m_objList.push_back(lift);
				break;
			}
			case 41:
			{
				//=====================================
				// 銅の扉初期化
				//=====================================
				std::shared_ptr<CopperDoor> door = std::make_shared<CopperDoor>();
				door->SetPos({ row[static_cast<unsigned int>(GimmickData::posX)],row[static_cast<unsigned int>(GimmickData::posY)],row[static_cast<unsigned int>(GimmickData::posZ)] });
				door->Init();
				m_objList.push_back(door);
				break;
			}
			case 42:
			{
				//=====================================
				// リフトのつなぎ目初期化
				//=====================================
				std::shared_ptr<JointLift> lift = std::make_shared<JointLift>();
				lift->Init();
				lift->SetPos({ row[static_cast<unsigned int>(GimmickData::posX)],row[static_cast<unsigned int>(GimmickData::posY)],row[static_cast<unsigned int>(GimmickData::posZ)] });
				lift->SetMoveDirOption(JointLift::MoveDirOption::X);
				m_objList.push_back(lift);
				break;
			}
			case 43:
			{
				//=====================================
				// 磁力の箱初期化
				//=====================================
				std::shared_ptr<MagneBox> box = std::make_shared<MagneBox>();
				box->Init();
				box->SetPos({ row[static_cast<unsigned int>(GimmickData::posX)],row[static_cast<unsigned int>(GimmickData::posY)],row[static_cast<unsigned int>(GimmickData::posZ)] });
				m_objList.push_back(box);
				break;
			}
			case 44:
			{
				//=====================================
				// リフトのつなぎ目初期化
				//=====================================
				std::shared_ptr<JointLift> lift = std::make_shared<JointLift>();
				lift->Init();
				lift->SetPos({ row[static_cast<unsigned int>(GimmickData::posX)],row[static_cast<unsigned int>(GimmickData::posY)],row[static_cast<unsigned int>(GimmickData::posZ)] });
				lift->SetMoveDirOption(JointLift::MoveDirOption::Z);
				m_objList.push_back(lift);
				break;
			}
			case 46:
			{
				//=====================================
				// 死の床初期化
				//=====================================
				CreateDeathFloor({ row[static_cast<unsigned int>(GimmickData::posX)],
					row[static_cast<unsigned int>(GimmickData::posY)],row[static_cast<unsigned int>(GimmickData::posZ)] },
					DeathFloor::MoveState::nomal, KdGameObject::ObjectType::DeathFloor, 4.0f);
				break;
			}
			case 47:
			{
				//=====================================
				// 看板初期化
				//=====================================
				std::shared_ptr<SignBoard> board = std::make_shared<SignBoard>();
				board->SetPos({ row[static_cast<unsigned int>(GimmickData::posX)],row[static_cast<unsigned int>(GimmickData::posY)],row[static_cast<unsigned int>(GimmickData::posZ)] });
				board->Init();
				board->SetTexture(SignBoard::TextOption::ElectroMagneticInduction, 20.f, 4, 1);
				if(m_player.expired()==false)
				{
					board->SetPlayer(m_player.lock());
				}
				m_objList.push_back(board);
				break;
			}
			case 48:
			{
				//=====================================
				// 看板初期化
				//=====================================
				std::shared_ptr<SignBoard> board = std::make_shared<SignBoard>();
				board->SetPos({ row[static_cast<unsigned int>(GimmickData::posX)],row[static_cast<unsigned int>(GimmickData::posY)],row[static_cast<unsigned int>(GimmickData::posZ)] });
				board->Init();
				board->SetTexture(SignBoard::TextOption::GetGun, 20.f, 5, 1);
				if (m_player.expired() == false)
				{
					board->SetPlayer(m_player.lock());
				}
				m_objList.push_back(board);
				break;
			}
			case 49:
			{
				//=====================================
				// 看板初期化
				//=====================================
				std::shared_ptr<SignBoard> board = std::make_shared<SignBoard>();
				board->SetPos({ row[static_cast<unsigned int>(GimmickData::posX)],row[static_cast<unsigned int>(GimmickData::posY)],row[static_cast<unsigned int>(GimmickData::posZ)] });
				board->Init();
				board->SetTexture(SignBoard::TextOption::MagneGun, 20.f, 4, 1);
				if (m_player.expired() == false)
				{
					board->SetPlayer(m_player.lock());
				}
				m_objList.push_back(board);
				break;
			}
			default:
				break;
			}
		}
	}
}

void GameScene::LoadGimmickSpecialParam(const std::string& _filePath)
{
	m_gimmickSpecialData = FileManager::Instance().GetData(_filePath);

	if (m_gimmickSpecialData.has_value())
	{
		for (auto& row : m_gimmickSpecialData.value())
		{
			switch (static_cast<int>(row[static_cast<int>(GimmickData::type)]))
			{
			case 7:
			{
				//=====================================
				// 返し初期化
				//=====================================
				std::shared_ptr<Barb> barb = std::make_shared<Barb>();
				barb->Init();
				barb->SetRotationAngle(row[static_cast<unsigned int>(GimmickDataForSpecial::angle)]);
				barb->SetModelSize(row[static_cast<unsigned int>(GimmickDataForSpecial::modelSize)]);
				barb->SetPos({ row[static_cast<unsigned int>(GimmickData::posX)],row[static_cast<unsigned int>(GimmickData::posY)],row[static_cast<unsigned int>(GimmickData::posZ)] });
				m_objList.push_back(barb);
				break;
			}
			case 8:
			{
				//=====================================
				// コイル初期化
				//=====================================
				std::shared_ptr<Coil> coil = std::make_shared<Coil>();
				coil->SetPos({ row[static_cast<unsigned int>(GimmickData::posX)],row[static_cast<unsigned int>(GimmickData::posY)],row[static_cast<unsigned int>(GimmickData::posZ)] });
				coil->Init();
				coil->SetRotAngle(row[static_cast<unsigned int>(GimmickDataForSpecial::angle)]);
				m_objList.push_back(coil);
				break;
			}
			case 9:
			{
				//=====================================
				// 長い足場初期化
				//=====================================
				std::shared_ptr<LongScaffold> scaffold = std::make_shared<LongScaffold>();
				scaffold->SetPos({ row[static_cast<unsigned int>(GimmickData::posX)],row[static_cast<unsigned int>(GimmickData::posY)],row[static_cast<unsigned int>(GimmickData::posZ)] });
				scaffold->Init();
				scaffold->SetWidth(row[static_cast<unsigned int>(GimmickDataForSpecial::width)]);
				m_objList.push_back(scaffold);
				break;
			}
			case 12:
			{
				//=====================================
				// 導線初期化
				//=====================================
				std::shared_ptr<Conductor> conductor = std::make_shared<Conductor>();
				conductor->SetPos({ row[static_cast<unsigned int>(GimmickData::posX)],row[static_cast<unsigned int>(GimmickData::posY)],row[static_cast<unsigned int>(GimmickData::posZ)] });
				conductor->SetModelSize(row[static_cast<unsigned int>(GimmickDataForSpecial::modelSize)]);
				conductor->SetAngle(row[static_cast<unsigned int>(GimmickDataForSpecial::angle)]);
				conductor->Init();
				m_objList.push_back(conductor);
				break;
			}
			case 17:
			{
				//=====================================
				// 足場床初期化(S極)
				//=====================================
				std::shared_ptr<MaguneticFloor> floor = std::make_shared<MaguneticFloor>();
				floor->Init();
				floor->SetModelSize(row[static_cast<unsigned int>(GimmickDataForSpecial::modelSize)]);
				floor->SetAdjustAdValue(row[static_cast<unsigned int>(GimmickDataForSpecial::adjustValue)]);
				if (row[static_cast<unsigned int>(GimmickDataForSpecial::scope)] != 0)
				{
					floor->SetScope(row[static_cast<unsigned int>(GimmickDataForSpecial::scope)]);
				}
				floor->SetPos({ row[static_cast<unsigned int>(GimmickData::posX)],row[static_cast<unsigned int>(GimmickData::posY)],row[static_cast<unsigned int>(GimmickData::posZ)] });
				floor->SetMaguneForce(KdGameObject::MagunePowerS);
				m_objList.push_back(floor);
				break;
			}
			case 18:
			{
				//=====================================
				// 足場床初期化(磁力無し)
				//=====================================
				std::shared_ptr<MaguneticFloor> floor = std::make_shared<MaguneticFloor>();
				floor->Init();
				floor->SetModelSize(row[static_cast<unsigned int>(GimmickDataForSpecial::modelSize)]);
				floor->SetPos({ row[static_cast<unsigned int>(GimmickData::posX)],row[static_cast<unsigned int>(GimmickData::posY)],row[static_cast<unsigned int>(GimmickData::posZ)] });
				floor->SetMaguneForce(KdGameObject::NoForce);
				m_objList.push_back(floor);
				break;
			}
			case 24:
			{
				//=====================================
				// レール初期化
				//=====================================
				std::shared_ptr<Rail> rail = std::make_shared<Rail>();
				rail->Init();
				rail->SetRotAngle(row[static_cast<unsigned int>(GimmickDataForSpecial::angle)]);
				rail->SetPos({ row[static_cast<unsigned int>(GimmickData::posX)],row[static_cast<unsigned int>(GimmickData::posY)],row[static_cast<unsigned int>(GimmickData::posZ)] });
				m_objList.push_back(rail);
				break;
			}
			case 29:
			{
				//=====================================
				// 檻初期化
				//=====================================
				std::shared_ptr<Cage> cage = std::make_shared<Cage>();
				cage->SetPos({ row[static_cast<unsigned int>(GimmickData::posX)],row[static_cast<unsigned int>(GimmickData::posY)],row[static_cast<unsigned int>(GimmickData::posZ)] });
				cage->Init();
				// 回転角度をセット
				cage->SetAngle(row[static_cast<unsigned int>(GimmickDataForSpecial::angle)]);
				m_objList.push_back(cage);
				break;
			}
			case 30:
			{
				//=====================================
				// 銅の支柱初期化
				//=====================================
				std::shared_ptr<CopperPillar> pillar = std::make_shared<CopperPillar>();
				pillar->Init();
				pillar->SetPos({ row[static_cast<unsigned int>(GimmickData::posX)],row[static_cast<unsigned int>(GimmickData::posY)],row[static_cast<unsigned int>(GimmickData::posZ)] });
				pillar->SetModelSize(row[static_cast<unsigned int>(GimmickDataForSpecial::modelSize)]);
				m_objList.push_back(pillar);
				break;
			}
			case 32:
			{
				//=====================================
				// 長い足場初期化
				//=====================================
				std::shared_ptr<LongScaffold> scaffold = std::make_shared<LongScaffold>();
				scaffold->SetPos({ row[static_cast<unsigned int>(GimmickData::posX)],row[static_cast<unsigned int>(GimmickData::posY)],row[static_cast<unsigned int>(GimmickData::posZ)] });
				scaffold->Init();
				scaffold->SetWidth(row[static_cast<unsigned int>(GimmickDataForSpecial::width)]);
				scaffold->SetRotAngle(row[static_cast<unsigned int>(GimmickDataForSpecial::angle)]);
				m_objList.push_back(scaffold);
				break;
			}
			case 33:
			{
				//=====================================
				// 足場床初期化(N極)
				//=====================================
				std::shared_ptr<MaguneticFloor> floor = std::make_shared<MaguneticFloor>();
				floor->Init();
				floor->SetUpdate(true);
				floor->SetModelSize(row[static_cast<unsigned int>(GimmickDataForSpecial::modelSize)]);
				floor->SetScope(row[static_cast<unsigned int>(GimmickDataForSpecial::scope)]);
				floor->SetAdjustAdValue(row[static_cast<unsigned int>(GimmickDataForSpecial::adjustValue)]);
				floor->SetPos({ row[static_cast<unsigned int>(GimmickData::posX)],row[static_cast<unsigned int>(GimmickData::posY)],row[static_cast<unsigned int>(GimmickData::posZ)] });
				floor->SetMaguneForce(KdGameObject::MagunePowerN);
				m_objList.push_back(floor);
				break;
			}
			case 35:
			{
				//=====================================
				// 石の壁初期化(回転あり)
				//=====================================
				std::shared_ptr<StoneWall> wall = std::make_shared<StoneWall>();
				wall->Init();
				wall->SetPos({ row[static_cast<unsigned int>(GimmickData::posX)],row[static_cast<unsigned int>(GimmickData::posY)],row[static_cast<unsigned int>(GimmickData::posZ)] });
				wall->SetModelSize(row[static_cast<unsigned int>(GimmickDataForSpecial::modelSize)]);
				wall->SetAngle(row[static_cast<unsigned int>(GimmickDataForSpecial::angle)]);
				m_objList.push_back(wall);
				break;
			}
			case 36:
			{
				//=====================================
				// 動く壁初期化(回転あり)
				//=====================================
				std::shared_ptr<MovingWall> wall = std::make_shared<MovingWall>();
				wall->SetPos({ row[static_cast<unsigned int>(GimmickData::posX)],row[static_cast<unsigned int>(GimmickData::posY)],row[static_cast<unsigned int>(GimmickData::posZ)] });
				wall->Init();
				wall->SetSize(MovingWall::Size::Small);
				wall->SetObjType(KdGameObject::ObjectType::MovingWall);
				wall->SetRotAngle(row[static_cast<unsigned int>(GimmickDataForSpecial::angle)]);
				m_objList.push_back(wall);
				break;
			}
			case 37:
			{
				//=====================================
				// 金の支柱初期化
				//=====================================
				std::shared_ptr<GoldPillar> pillar = std::make_shared<GoldPillar>();
				pillar->Init();
				pillar->SetPos({ row[static_cast<unsigned int>(GimmickData::posX)],row[static_cast<unsigned int>(GimmickData::posY)],row[static_cast<unsigned int>(GimmickData::posZ)] });
				pillar->SetModelSize(row[static_cast<unsigned int>(GimmickDataForSpecial::modelSize)]);
				m_objList.push_back(pillar);
				break;
			}
			case 38:
			{
				//=====================================
				// 磁力帯初期化
				//=====================================
				std::shared_ptr<MagnaticArea> Area = std::make_shared<MagnaticArea>();
				Area->Init();
				Area->SetPos({ row[static_cast<unsigned int>(GimmickData::posX)],row[static_cast<unsigned int>(GimmickData::posY)],row[static_cast<unsigned int>(GimmickData::posZ)] });
				Area->SetModelSize(row[static_cast<unsigned int>(GimmickDataForSpecial::modelSize)]);
				Area->SetAngle(row[static_cast<unsigned int>(GimmickDataForSpecial::angle)]);
				m_objList.push_back(Area);
				m_gimmickList.push_back(Area);
				break;
			}
			case 39:
			{
				//=====================================
				// 短い足場初期化
				//=====================================
				std::shared_ptr<ShortScaffold> scaffold = std::make_shared<ShortScaffold>();
				scaffold->Init();
				scaffold->SetPos({ row[static_cast<unsigned int>(GimmickData::posX)],row[static_cast<unsigned int>(GimmickData::posY)],row[static_cast<unsigned int>(GimmickData::posZ)] });
				scaffold->SetModelSize(row[static_cast<unsigned int>(GimmickDataForSpecial::modelSize)]);
				m_objList.push_back(scaffold);
				break;
			}
			default:
			break;
			}
		}
	}
}

void GameScene::LoadMoveFloorGimmickParam(const std::string& _filePath)
{
	m_moveFloorGimmickData = FileManager::Instance().GetData(_filePath);

	if (m_moveFloorGimmickData.has_value())
	{
		for (const auto& row : m_moveFloorGimmickData.value())
		{
			switch ((int)(row[static_cast<unsigned int>(GimmickData::type)]))
			{
			case 1:
			{
				//=====================================
				// 電磁誘導のエリアの的となるリフトの初期化
				//=====================================
				std::shared_ptr<TargetLift> lift = std::make_shared<TargetLift>();
				lift->SetPos({ row[static_cast<unsigned int>(GimmickData::posX)],row[static_cast<unsigned int>(GimmickData::posY)],row[static_cast<unsigned int>(GimmickData::posZ)] });
				lift->Init();
				lift->SetKeyObj(static_cast<int>(row[static_cast<unsigned int>(MoveFloor::keyObjNum)]));
				lift->SetMaguneForce(static_cast<int>(row[static_cast<unsigned int>(MoveFloor::force)]));
				m_objList.push_back(lift);
				break;
			}
			case 16:
			{
				//=====================================
				// 動く足場床初期化(S極)
				//=====================================
				std::shared_ptr<MoveMagneFloor> floor = std::make_shared<MoveMagneFloor>();
				floor->Init();
				floor->SetPos({ row[static_cast<unsigned int>(GimmickData::posX)],row[static_cast<unsigned int>(GimmickData::posY)],row[static_cast<unsigned int>(GimmickData::posZ)] });
				floor->SetMoveMax(static_cast<int>(row[static_cast<unsigned int>(MoveFloor::moveMax)]));
				floor->SetMaguneState(MoveMagneFloor::MoveState::nomal);
				floor->SetObjType(KdGameObject::ObjectType::MoveMagneFloorNomalVer);
				m_objList.push_back(floor);
				break;
			}
			case 26:
			{
				//=====================================
				// 動く足場床初期化
				//=====================================
				std::shared_ptr<MoveMagneFloor> floor = std::make_shared<MoveMagneFloor>();
				floor->Init();
				floor->SetPos({ row[static_cast<unsigned int>(GimmickData::posX)],row[static_cast<unsigned int>(GimmickData::posY)],row[static_cast<unsigned int>(GimmickData::posZ)] });
				floor->SetMoveMax(static_cast<int>(row[static_cast<unsigned int>(MoveFloor::moveMax)]));
				floor->SetKeyObj(static_cast<int>(row[static_cast<unsigned int>(MoveFloor::keyObjNum)]));
				floor->SetMaguneForce(static_cast<int>(row[static_cast<unsigned int>(MoveFloor::force)]));
				floor->SetMaguneState(MoveMagneFloor::MoveState::special);
				floor->SetObjType(KdGameObject::ObjectType::MoveMagunet);
				m_objList.push_back(floor);
				break;
			}
			default:
				break;
			}
		}
	}
}

void GameScene::LoadElectricityEffectObjectParam(const std::string& _filePath)
{
	m_moveFloorGimmickData = FileManager::Instance().GetData(_filePath);

	if (m_moveFloorGimmickData.has_value())
	{
		for (const auto& row : m_moveFloorGimmickData.value())
		{
			switch ((int)(row[static_cast<unsigned int>(GimmickData::type)]))
			{
			case 44:
			{
				//=====================================
				// 電流の初期化
				//=====================================
				std::shared_ptr<ElectricCurrent> elec = std::make_shared<ElectricCurrent>();
				elec->SetStartPos({ row[static_cast<unsigned int>(GimmickData::posX)],row[static_cast<unsigned int>(GimmickData::posY)],row[static_cast<unsigned int>(GimmickData::posZ)] });
				elec->SetEndPos({ row[static_cast<unsigned int>(Effect::endPosX)],row[static_cast<unsigned int>(Effect::endPosY)],row[static_cast<unsigned int>(Effect::endPosZ)] });
				elec->Init();
				elec->SetDestination(ElectricCurrent::Destination::End);
				elec->SetTargetObject((ElectricCurrent::TargetObject)row[static_cast<unsigned int>(Effect::target)]);
				m_objList.push_back(elec);
				break;
			}
			case 45:
			{
				//=====================================
				// 電流の初期化(中継地点あり)
				//=====================================
				std::shared_ptr<ElectricCurrent> elec = std::make_shared<ElectricCurrent>();
				elec->SetStartPos({ row[static_cast<unsigned int>(GimmickData::posX)],row[static_cast<unsigned int>(GimmickData::posY)],row[static_cast<unsigned int>(GimmickData::posZ)] });
				elec->SetEndPos({ row[static_cast<unsigned int>(Effect::endPosX)],row[static_cast<unsigned int>(Effect::endPosY)],row[static_cast<unsigned int>(Effect::endPosZ)] });
				elec->SetStopoverPos({ row[static_cast<unsigned int>(Effect::middleX)],row[static_cast<unsigned int>(Effect::middleY)],row[static_cast<unsigned int>(Effect::middleZ)] });
				elec->Init();
				elec->SetDestination(ElectricCurrent::Destination::StopOver);
				elec->SetTargetObject((ElectricCurrent::TargetObject)row[static_cast<unsigned int>(Effect::endTarget)]);
				m_objList.push_back(elec);
				break;
			}
			}
		}
	}

}

// 磁力が変わる床を生成する関数
void GameScene::CreateChangeFloor(float _angle, float _posX, float _posY, float _posZ, float _interval)
{
	//=====================================
	// 磁力が変わる床初期化
	//=====================================
	std::shared_ptr<ChangeFloor> floor = std::make_shared<ChangeFloor>();
	floor->Init();
	floor->SetPos({_posX,_posY,_posZ});
	floor->SetAngle(_angle);
	floor->SetIntervalMax(_interval);
	m_objList.push_back(floor);
	m_gimmickList.push_back(floor);
}

// 魔法陣生成関数
void GameScene::CreateMagicRing(KdGameObject::ObjectType _magune, const std::string& _effectName, float _posX, float _posY, float _posZ)
{
	std::shared_ptr<MagicRing> ring = std::make_shared<MagicRing>();
	ring->SetEffectName(_effectName);
	ring->SetPos({ _posX,_posY,_posZ });
	ring->Init();
	ring->SetObjType(_magune);
	m_objList.push_back(ring);
}

void GameScene::CreateDeathFloor(const Math::Vector3& _pos, DeathFloor::MoveState _state, KdGameObject::ObjectType _type, const float _scale)
{
	//=====================================
	// 死の床初期化
	//=====================================
	std::shared_ptr<DeathFloor> death = std::make_shared<DeathFloor>();
	death->SetPos(_pos);
	death->Init();
	death->ChangeMoveState(_state);
	death->SetObjType(_type);
	death->SetModelSize(_scale);
	m_objList.push_back(death);
	m_gimmickList.push_back(death);
}

// 扉の開閉アニメーション関数
void GameScene::OpenGateAnimation()
{
	// アニメーション開始通達中以外は早期リターン
	if (GateManager::Instance().GetNowState() != GateManager::State::NoticeAnimation)return;

	// 今のターゲットを探す
	std::shared_ptr<KdGameObject> _now = FindObject(m_nowCamTarget);

	// 新しいターゲットを設定する
	m_nowCamTarget = FindNextTarget(_now);

	std::shared_ptr<KdGameObject> _target = nullptr;

	if (m_nowCamTarget != KdGameObject::ObjectType::None)
	{
		// 新しいターゲットを探す
		 _target = FindObject(m_nowCamTarget);
	}

	// 補完用のオブジェクトを作成
	std::shared_ptr<AnimationCamera> _spNewAnimCam = std::make_shared<AnimationCamera>();
	_spNewAnimCam->Init();
	_spNewAnimCam->SetAnimationInfo(this, _now, _target);
	_spNewAnimCam->Update();
	m_objList.push_back(_spNewAnimCam);

	// 補完用のオブジェクトをカメラのターゲットに切り替える
	std::shared_ptr<CameraBase> _spCamera = m_wpCamera.lock();
	if (_spCamera)
	{
		_spCamera->SetTarget(_spNewAnimCam);
	}

	// ゲートマネージャーにアニメーション開始を伝える
	switch (m_nowCamTarget)
	{
	case KdGameObject::ObjectType::Gate:
		GateManager::Instance().SetState(GateManager::State::OpenAnimation);
		break;
	case KdGameObject::ObjectType::Player:
		GateManager::Instance().SetState(GateManager::State::BackPlayerAnimation);
		break;
	default:
		break;
	}
}

void GameScene::ChangeNextScene()
{
	// プレイヤーの実態がなければ処理をしない
	std::shared_ptr<Player> spPlayer = m_player.lock();
	if (!spPlayer)return;

	// ゴール状態でもゲームオーバー状態でもなければ処理をしない
	if (!GoalManager::instance().GetClearState() && !spPlayer->GetGameOver())return;

	//プレイヤーがゴールした時リザルト画面へ移行する
	if (GoalManager::instance().GetClearState() == true)
	{
		if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		{
			if (!m_pushEnterFlg)
			{
				if (m_hp.expired() == false)
				{
					ScoreManager::instance().RegisterClearHp(m_hp.lock()->GetHpNum());
					ScoreManager::instance().ConversionScore();
				}

				SceneManager::Instance().SetNextScene
				(
					SceneManager::SceneType::Result
				);
				StartManager::instance().SetStartState(false);
				m_pushEnterFlg = true;
			}
		}
		else
		{
			m_pushEnterFlg = false;
		}
	}
	// HPが０になった場合
	else if (m_player.lock()->GetGameOver() == true)
	{
		// タイトル画面に戻る処理
		if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		{
			if (!m_pushEnterFlg)
			{
				SceneManager::Instance().SetNextScene
				(
					SceneManager::SceneType::Title
				);
				StartManager::instance().SetStartState(false);
				m_pushEnterFlg = true;
			}
		}
		else
		{
			m_pushEnterFlg = false;
		}
	}
}

// 特定のオブジェクト検索関数
std::shared_ptr<KdGameObject> GameScene::FindObject(const KdGameObject::ObjectType _obj)
{
	for (auto& obj : m_objList)
	{
		//比較
		if (obj->GetObjType() == _obj)
		{
			return obj;
		}
	}

	// 見つからなかったらNULLを返す
	return nullptr;
}

// カメラアニメーションの次のターゲットを検索する関数
KdGameObject::ObjectType GameScene::FindNextTarget(const std::weak_ptr<KdGameObject>& _nowTarget)
{
	std::shared_ptr<KdGameObject> spNowTarget = _nowTarget.lock();

	// ターゲットの実態がないなら早期リターン
	if (!spNowTarget)
	{
		return KdGameObject::ObjectType::None;
	}

	switch (spNowTarget->GetObjType())
	{
	case KdGameObject::ObjectType::Player:
		return KdGameObject::ObjectType::Gate;
		break;
	case KdGameObject::ObjectType::Gate:
		return KdGameObject::ObjectType::Player;
		break;
	default:
		break;
	}

	//見つからなかった場合
	return KdGameObject::ObjectType::None;
}

void GameScene::ElectroMagneticInductionUpdateObject()
{
	if (UpdateObjManager::Instance().GetKeyObjects(UpdateObjManager::one).maguneforce != UpdateObjManager::Instance().GetKeyObjects(UpdateObjManager::two).maguneforce)
	{
		UpdateObjManager::Instance().SetLiftUpdate(true);
	}
	else
	{
		UpdateObjManager::Instance().SetLiftUpdate(false);
	}

	if (UpdateObjManager::Instance().GetKeyObjects(UpdateObjManager::three).maguneforce != UpdateObjManager::Instance().GetKeyObjects(UpdateObjManager::four).maguneforce)
	{
		UpdateObjManager::Instance().SetDoorUpdate(true);
	}
	else
	{
		UpdateObjManager::Instance().SetDoorUpdate(false);
	}

	UINT forceForFive = UpdateObjManager::Instance().GetKeyObjects(UpdateObjManager::five).maguneforce;
	UINT forceForSix = UpdateObjManager::Instance().GetKeyObjects(UpdateObjManager::six).maguneforce;

	if (forceForFive != forceForSix)
	{
		if (((forceForFive & KdGameObject::NoForce) == 0) && ((forceForSix & KdGameObject::NoForce) == 0))
		{
			// 電磁誘導していることを知らせる
			GateManager::Instance().SetElectromagneticInduction(true);

			if (GateManager::Instance().GetNowState() == GateManager::State::NotStart)
			{
				GateManager::Instance().SetState(GateManager::State::NoticeAnimation);
			}
		}
	}
	else
	{
		// 電磁誘導していないことを知らせる
		GateManager::Instance().SetElectromagneticInduction(false);
	}
}
