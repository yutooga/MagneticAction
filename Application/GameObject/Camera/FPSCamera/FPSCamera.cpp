#include "FPSCamera.h"

void FPSCamera::Init()
{
	// 親クラスの初期化呼び出し
	CameraBase::Init();
}

void FPSCamera::Update()
{
	// ターゲットの行列(有効な場合利用する)
	const std::shared_ptr<const KdGameObject>	_spTarget = m_wpTarget.lock();
	Math::Matrix _targetMat = Math::Matrix::Identity;

	//仮行列消す
	Math::Matrix transMat = Math::Matrix::CreateTranslation({ -2.0f,0.0f,-4.0f });
	if(_spTarget)
	{
		_targetMat = _spTarget->GetCameraMatrix();
	}
	//m_mWorld = transMat * _targetMat;
	m_mWorld = _targetMat;

	CameraBase::Update();
}
