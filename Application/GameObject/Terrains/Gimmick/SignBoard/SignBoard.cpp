#include "SignBoard.h"
#include"../../../Character/Player/Player.h"

void SignBoard::Init()
{
	// jsonファイルから情報を読み込んでくる

	std::ifstream ifs("Asset/Data/GameScene/3DObject/Terrains/Gimmick/SignBoardData/SignBoard.json");
	if (ifs) {
		ifs >> m_signBoardData;
	}

	// 画像のロード
	if (!m_exclamationMark)
	{
		m_exclamationMark = std::make_shared<KdSquarePolygon>();
		m_exclamationMark->SetMaterial(m_signBoardData["ExclamationMark"]["URL"]);
		m_exclamationMark->SetScale(m_signBoardData["ExclamationMark"].value("Scale",10.f));
	}

	m_pDebugWire = std::make_unique<KdDebugWireFrame>();

	// エフェクトの表示位置初期化
	Math::Vector3 effectPos = m_pos;
	effectPos.y -= m_signBoardData["Effect"].value("AdjustPosY", 10.f);

	// エフェクトの初期化
	KdEffekseerManager::GetInstance().Play(
		m_signBoardData["Effect"]["URL"], effectPos, m_signBoardData["Effect"].value("Size", 5.f), 
		m_signBoardData["Effect"].value("Speed",1.f), true);
}

void SignBoard::Update()
{
	// 表示画像切り替えフラグの初期化
	m_changeFlg = false;

	// 画像の更新
	TextureUpdate();

	// 画像のアニメーション
	Animation();

	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos);
	Math::Matrix rotMat = Math::Matrix::CreateFromYawPitchRoll(
		DirectX::XMConvertToRadians(m_angle.y),
		DirectX::XMConvertToRadians(m_angle.x),
		0.0f);
	m_mWorld = rotMat * transMat;
}

void SignBoard::PostUpdate()
{
	// プレイヤー感知関数
	JudgementPlayer();
}

void SignBoard::DrawUnLit()
{
	if(!m_changeFlg)
	{
		KdShaderManager::Instance().m_StandardShader.DrawPolygon(*m_exclamationMark, m_mWorld);
	}
	else
	{
		KdShaderManager::Instance().m_StandardShader.DrawPolygon(*m_text, m_mWorld);
	}
}

void SignBoard::DrawImGui()
{
	if (ImGui::CollapsingHeader("SignBoard", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat3("board m_pos", &m_pos.x, 0.1f);
	}
}

void SignBoard::SetTexture(const TextOption& _option, const float _scale, const int _splitX, const int _splitY)
{
	// 既に画像が読み込んでいるなら処理しない
	if (m_text)return;

	m_text = std::make_shared<KdSquarePolygon>();

	switch (_option)
	{
	case TextOption::GetGun:
		m_text->SetMaterial(m_signBoardData["Texture"]["GetGun"]["URL"]);
		break;
	case TextOption::ElectroMagneticInduction:
		m_text->SetMaterial(m_signBoardData["Texture"]["Electro"]["URL"]);
		break;
	case TextOption::MagneGun:
		m_text->SetMaterial(m_signBoardData["Texture"]["MagneGun"]["URL"]);
		break;
	default:
		break;
	}

	m_text->SetScale(_scale);
	m_text->SetSplit(_splitX,_splitY);
	m_animationCntMax = static_cast<float>(_splitX);
}

void SignBoard::TextureUpdate()
{
	std::shared_ptr<Player> _spPlayer = m_wpPlayer.lock();

	if (_spPlayer)
	{
		m_angle = _spPlayer->GetRotateAngle();
	}
}

void SignBoard::Animation()
{
	m_animationCnt += m_signBoardData["Animation"].value("AddValue", 0.1f);

	// アニメーションが最後のコマまでいったら最初のコマに戻す
	if (m_animationCnt > m_animationCntMax)
	{
		m_animationCnt = 0;
	}
	m_text->SetUVRect(static_cast<int>(m_animationCnt));
}

void SignBoard::JudgementPlayer()
{
	// プレイヤーの実体が存在しないなら早期リターン
	std::shared_ptr<Player> _spPlayer = m_wpPlayer.lock();

	if (!_spPlayer)return;

	//===============================
	//              球判定
	//===============================

	//球判定用の変数を作成
	KdCollider::SphereInfo sphere;

	//球の中心位置を設定
	sphere.m_sphere.Center = m_pos;
	sphere.m_sphere.Center.y += m_signBoardData["Colision"].value("AdjustValueY", 0.1f);
	//球の半径を設定
	sphere.m_sphere.Radius = m_signBoardData["Colision"].value("Radius", 20.f);
	//当たり判定をしたいタイプを設定
	sphere.m_type = KdCollider::TypeSight;

	//球に当たったオブジェクト情報を格納
	//当たり判定
	bool hitFlg = false;
	hitFlg = _spPlayer->Intersects(sphere, nullptr);

	if (hitFlg)
	{
		m_changeFlg = true;
	}
}
