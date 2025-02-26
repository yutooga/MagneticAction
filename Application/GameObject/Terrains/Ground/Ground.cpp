#include "Ground.h"
#include"../../../Manager/ModelManager/ModelManager.h"

void Ground::Init()
{
	// モデルの読み込み
	if (!m_model)
	{
		m_model = ModelManager::Instance().GetModel("Ground");

		m_pCollider = std::make_unique<KdCollider>();
		m_pCollider->RegisterCollisionShape("Ground", m_model, KdCollider::TypeGround | KdCollider::TypeDamage);
	}
}
