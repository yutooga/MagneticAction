#include "Ground.h"

void Ground::Init()
{
	// モデルの読み込み
	if (!m_model)
	{
		m_model = std::make_shared<KdModelWork>();
		m_model->SetModelData("Asset/Models/Terrains/Ground/Ground.gltf");

		m_pCollider = std::make_unique<KdCollider>();
		m_pCollider->RegisterCollisionShape("Ground", m_model, KdCollider::TypeGround | KdCollider::TypeDamage);
	}
}
