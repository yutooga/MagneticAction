#include "FallingMagneGun.h"
#include"../../../Scene/SceneManager.h"
#include"../../Character/Player/Player.h"
#include"../../Weapon/MagneGun/MagneGun.h"
#include"../../2DObject/Reticle/Reticle.h"

const float FallingMagneGun::k_upDownSpeed = 0.03f;
const float FallingMagneGun::k_rotSpeed = 0.5f;
const float FallingMagneGun::k_addAnimeCnt = 0.1f;
const float FallingMagneGun::k_animationMax = 1.4f;
const float FallingMagneGun::k_adjustValue = 5.f;

void FallingMagneGun::Init()
{
	// jsonファイルから情報を読み込んでくる

	std::ifstream ifs("Asset/Data/GameScene/3DObject/Weapon/FallingMagneGun/FallingMagneGunData.json");
	if (ifs) {
		ifs >> m_magneGunData;
	}

	// モデルの読み込み
	if (!m_model)
	{
		m_model = std::make_shared<KdModelWork>();
		m_model->SetModelData(m_magneGunData["URL"]);
	}

	//取得可能表示テキスト
	if(!m_polygon)
	{
		m_polygon = std::make_shared<KdSquarePolygon>();
		m_polygon->SetMaterial("Asset/Textures/Scene/GameScene/2DObject/ChangeInstructions/ChangeInstructions.png");
		m_polygon->SetSplit(m_magneGunData["Text"].value("SplitX",2), m_magneGunData["Text"].value("SplitY", 1));
		m_polygon->SetScale(m_magneGunData["Text"].value("Scale", 3.f));
	}

	// モデルの表示座標
	m_pos = {m_magneGunData["FirstPos"].value("X", -112.f),
		m_magneGunData["FirstPos"].value("Y", -52.f),m_magneGunData["FirstPos"].value("Z", 1825.f) };
	m_pos.y += m_magneGunData["FirstPos"].value("AdjustPosY", 5.0f);

	//デバックライン処理
	m_pDebugWire = std::make_unique<KdDebugWireFrame>();

	// エフェクトの初期化

	// エフェクトの表示位置初期化
	Math::Vector3 effectPos = m_pos;
	effectPos.y -= m_magneGunData["Effect"].value("AdjustPosY", 10.0f);

	// 一番最初の座標を保存しておく
	m_firstPos = effectPos;

	float effectSize = m_magneGunData["Effect"].value("Size", 5.0f);
	float effectSpeed = m_magneGunData["Effect"].value("Speed", 1.0f);

	// エフェクトの名前を保存
	m_effectName = m_magneGunData["Effect"]["Name"];

	KdEffekseerManager::GetInstance().Play(m_effectName, effectPos, effectSize, effectSpeed, true);
}

void FallingMagneGun::Update()
{
	//オブジェクトの回転更新
	m_angle += k_rotSpeed;

	// 限界値を超えないようにする
	if (m_angle > k_rotAngleMax)
	{
		m_angle -= k_rotAngleMax;
	}

	{
		const float firstPosY = m_pos.y;
		m_pos.y = firstPosY + sin(DirectX::XMConvertToRadians(m_angle)) * k_upDownSpeed;
	}

	//銃の行列更新
	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix scaleMat = Math::Matrix::CreateScale(m_modelSize);
	Math::Matrix rotMat = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angle));
	m_mWorld = scaleMat * rotMat * transMat;

	// 銃の獲得処理
	GetMagneGun();

	//テキストの更新
	TextureUpdate();
}

void FallingMagneGun::PostUpdate()
{
	//プレイヤーが銃を取得できる位置にいるか判定する
	JudgmentAcquisition();
}

void FallingMagneGun::DrawLit()
{
	//モデルが読み込まれていないなら早期リターン
	if (!m_model)return;
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);

	// 銃の近くまで来たらテキストを表示する
	if (!m_acquisitionFlg || !m_polygon)return;
	KdShaderManager::Instance().m_StandardShader.DrawPolygon(*m_polygon, m_textureMat);
}

void FallingMagneGun::DrawImGui()
{
	if (ImGui::CollapsingHeader("FallingMagneGun", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat3("FallingMagneGun m_pos", &m_pos.x, 0.1f);
		ImGui::DragFloat("FallingMagneGun size", &m_modelSize, 0.001f);
	}
}

void FallingMagneGun::JudgmentAcquisition()
{
	KdCollider::SphereInfo sphere;
	sphere.m_sphere.Center = m_pos;
	sphere.m_sphere.Radius = m_magneGunData["Colision"].value("Radius", 6.0f);
	sphere.m_type = KdCollider::TypeSight;

	//ヒット判定フラグ
	bool hitFlg = false;

	for (auto& obj : SceneManager::Instance().GetObjList())
	{
		if (obj->GetObjType() != ObjectType::Player)continue;
		hitFlg = obj->Intersects(sphere, nullptr);
		if (hitFlg)break;
	}

	//当たっているなら取得可能状態にする
	if (hitFlg)
	{
		m_acquisitionFlg = true;
	}
	else
	{
		m_acquisitionFlg = false;
	}
}

void FallingMagneGun::TextureUpdate()
{
	// 取得可能状態でなければ早期リターン
	if (!m_acquisitionFlg)return;
	
	//アニメーション更新
		m_animeCnt += k_addAnimeCnt;

	// 限界値を超えないようにする
	if (m_animeCnt > k_animationMax)
	{
		m_animeCnt = 0.0f;
	}

	// ポリゴンにセット
	m_polygon->SetUVRect((int)m_animeCnt);

	//ビルボード処理
	std::shared_ptr<Player> spPlayer = m_player.lock();
	Math::Vector3 angle;

	// プレイヤーの回転角度を取得する
	if (spPlayer)
	{
		angle = spPlayer->GetRotateAngle();
	}

	//テキストの行列更新

	Math::Matrix rotMat = Math::Matrix::CreateFromYawPitchRoll(
			DirectX::XMConvertToRadians(angle.y),
			DirectX::XMConvertToRadians(angle.x),
			0.0f);

	 Math::Matrix transMat = Math::Matrix::CreateTranslation({ m_pos.x,m_pos.y + k_adjustValue, m_pos.z });

	 m_textureMat = rotMat * transMat;
}

void FallingMagneGun::GetMagneGun()
{
	//取得可能状態でなければ早期リターン
	if (!m_acquisitionFlg)return;

	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		{
			std::shared_ptr<MagneGun> Gun = m_gun.lock();
			if (Gun)
			{
				Gun->SetUpdate(true);
			}
		}

		// レティクル作成
		{
			std::shared_ptr<Reticle> reticle = std::make_shared<Reticle>();
			reticle->Init();
			SceneManager::Instance().AddObject(reticle);
		}

		// 存在を消滅させる
		m_isExpired = true;

		// エフェクトを停止させる
		KdEffekseerManager::GetInstance().StopEffect(m_effectName, m_firstPos);

		//銃取得のSE再生
		KdAudioManager::Instance().Play("Asset/Sounds/GameScene/Weapon/FallingMaguneGun/Acquisition.wav", false);
	}
}
