#include "AnimationCamera.h"
#include"../../../Scene/GameScene/GameScene.h"
#include"../../../GameObject/Manager/GateManager/GateManager.h"

const float AnimationCamera::k_end = 1.f;

void AnimationCamera::Init()
{
}

void AnimationCamera::Update()
{
	// 二点間の補完処理を行う
	std::shared_ptr<KdGameObject> _spStart = m_wpStart.lock();
	std::shared_ptr<KdGameObject> _spEnd = m_wpEnd.lock();

	// 対象が消えていたらアニメーション強制終了
	if (!_spStart || !_spEnd)
	{
		// 補完するための存在なので破棄　＊カメラ対象が不在になるのでフォロー必須
		m_isExpired = true;
		return;
	}

	// 補完する行列の取得
	Math::Matrix _startMat = _spStart->GetCameraMatrix();
	Math::Matrix _endMat = _spEnd->GetCameraMatrix();

	//============================================================
	// 回転の補完
	//============================================================

	// クォータニオン用意
	Math::Quaternion _startQue;
	Math::Quaternion _endQue;
	Math::Quaternion _nowQue;

	// 最初の行列からクォータニオン作成
	_startQue = Math::Quaternion::CreateFromRotationMatrix(_startMat);
	_endQue = Math::Quaternion::CreateFromRotationMatrix(_endMat);

	// 進行具合を加味した回転を求める
	// 球面線形補完
	_nowQue = Math::Quaternion::Slerp(_startQue, _endQue, m_progress);

	// ワールド行列を更新（回転）
	m_mWorld = Math::Matrix::CreateFromQuaternion(_nowQue);

	//============================================================
	// 回転の補完	ここまで
	//============================================================


	//============================================================
	// 線形補完
	//============================================================

	Math::Vector3 _startVec;
	Math::Vector3 _endVec;

	_startVec = _startMat.Translation();
	_endVec = _endMat.Translation();

	// 進行具合を加味した座標を求める
	Math::Vector3 _nowVec;

	// 線形補完
	_nowVec = Math::Vector3::Lerp(_startVec, _endVec, m_progress);

	//ワールド行列を更新（座標）
	m_mWorld.Translation(_nowVec);

	//============================================================
	// 線形補完　ここまで
	//============================================================

	//============================================================
	// 進行具合の更新
	//============================================================

	m_progress += m_speed;

	if (m_progress > k_end)
	{
		m_progress = k_end;

		// アニメーションが終了したら自身は消滅
		m_isExpired = true;

		// カメラの追尾対象を終点オブジェクトに設定
		if (m_pNowScene)
		{
			m_pNowScene->SetCameraTarget(_spEnd);
		}

		//もし終点オブジェクトが扉なら扉の開放アニメーション開始状態にする
		if (_spEnd->GetObjType() == KdGameObject::ObjectType::Gate)
		{
			GateManager::Instance().SetState(GateManager::State::Open);
		}
		// もし終点オブジェクトがプレイヤーなら全ての更新が終わった状態に設定する
		else if (_spEnd->GetObjType() == KdGameObject::ObjectType::Player)
		{
			GateManager::Instance().SetState(GateManager::State::AllEnd);
		}
	}
}

void AnimationCamera::SetAnimationInfo(GameScene* scene, const std::shared_ptr<KdGameObject>& start, const std::shared_ptr<KdGameObject>& end, float spd)
{
	// 現在のシーン
	m_pNowScene = scene;

	// 始点と終点を記憶
	m_wpStart = start;
	m_wpEnd = end;

	// 進行度とスピードを初期化
	m_progress = 0.0f;
	m_speed = spd;
}
