#pragma once

// ゲーム上に存在するすべてのオブジェクトの基底となるクラス
class KdGameObject : public std::enable_shared_from_this<KdGameObject>
{
public:

	// どのような描画を行うのかを設定するTypeID：Bitフラグで複数指定可能
	enum
	{
		eDrawTypeLit = 1 << 0,
		eDrawTypeUnLit = 1 << 1,
		eDrawTypeBright = 1 << 2,
		eDrawTypeUI = 1 << 3,
		eDrawTypeDepthOfShadow = 1 << 4,
	};

	enum
	{
		NoForce = 1 << 0,
		MagunePowerS = 1 << 1,
		MagunePowerN = 1 << 2
	};

	enum class KeyObject
	{
		Zero = 0,
		One = 1,
		Two = 2,
		Three = 3,
		Four = 4,
		Five = 5,
		Six = 6
	};

	enum class ObjectType
	{
		JumpingFloor,
		MovingWall,
		MovingFloor,
		MovingDeathFloor,
		MoveMagunet,
		MagicRingS,
		MagicRingN,
		NoForceRing,
		ChangeFloor,
		MaguneBulletS,
		MaguneBulletN,
		SavePoint,
		DeathFloor,
		Goal,
		MaguneticFloor,
		BigMovingWall,
		Player,
		Gate,
		Cage,
		MagneticArea,
		WoodenLift,
		Ground,
		ShortScaffold,
		LongScaffold,
		MagneBox,
		ObstructiveWall,
		MoveMagneFloorNomalVer,
		None
	};

	KdGameObject() {}
	virtual ~KdGameObject() { Release(); }

	// 生成される全てに共通するパラメータに対する初期化のみ
	virtual void Init() {}

	virtual void PreUpdate() {}
	virtual void Update() {}
	virtual void PostUpdate() {}

	// それぞれの状況で描画する関数
	virtual void GenerateDepthMapFromLight() {}
	virtual void PreDraw() {}
	virtual void DrawLit() {}
	virtual void DrawUnLit() {}
	virtual void DrawBright() {}
	virtual void DrawSprite() {}

	virtual void DrawImGui(){}

	virtual void DrawDebug();

	//当たった時の処理
	virtual void OnHit(ObjectType _obj){}
	virtual void OnHit(ObjectType _type, const std::weak_ptr<KdGameObject>& _obj){}

	virtual void SetAsset(const std::string&) {}

	virtual void SetPos(const Math::Vector3& pos) { m_mWorld.Translation(pos); }
	virtual Math::Vector3 GetPos() const { return m_mWorld.Translation(); }

	// 拡大率を変更する関数
	void SetScale(float scalar);
	virtual void SetScale(const Math::Vector3& scale);
	virtual Math::Vector3 GetScale() const;

	const Math::Matrix& GetMatrix() const { return m_mWorld; }

	// カメラのローカル行列取得
	const Math::Matrix& GetCameraMatrix() const { return m_localCamera * m_mWorld; }

	virtual bool IsExpired() const { return m_isExpired; }
	virtual void EraseIsExpired(){ m_isExpired = true; }

	virtual bool IsVisible()	const { return false; }
	virtual bool IsRideable()	const { return false; }

	// 視錐台範囲内に入っているかどうか
	virtual bool CheckInScreen(const DirectX::BoundingFrustum&) const { return false; }

	// カメラからの距離を計算
	virtual void CalcDistSqrFromCamera(const Math::Vector3& camPos);

	float GetDistSqrFromCamera() const { return m_distSqrFromCamera; }

	UINT GetDrawType() const { return m_drawType; }

	virtual ObjectType GetObjType() { return m_ObjType; }
	void SetObjType(ObjectType _type) { m_ObjType = _type; }

	bool Intersects(const KdCollider::SphereInfo& targetShape, std::list<KdCollider::CollisionResult>* pResults);
	bool Intersects(const KdCollider::BoxInfo& targetBox, std::list<KdCollider::CollisionResult>* pResults);
	bool Intersects(const KdCollider::RayInfo& targetShape, std::list<KdCollider::CollisionResult>* pResults);

	UINT GetMaguneForce() { return m_maguneForce; }

	const Math::Vector3 GetDistancePos(){ return m_pos - m_previosPos; }
	const Math::Vector3 GetDistancePos2(){ return m_previosPos - m_pos; }

	void SetMaguneForce(const UINT& _maguneForce) { m_maguneForce = _maguneForce; }

	Math::Vector3 GetGimmickPos() { return m_pos; }

	const KeyObject GetKeyObj()const { return m_keyObject; }

	void SetKeyObj(const int _key);

	bool GetReaction() { return m_reactionFlg; }

	void SettingPos(const Math::Vector3& _pos) { m_pos = _pos; }

	void AddPos(Math::Vector3& _pos) { m_pos += _pos; }

	void SetUpdate(const bool _flg) { m_updateFlg = _flg; }

	void SetMoveFlg(const bool _move) { m_moveFlg = _move; }

	const bool GetUpdate()const { return m_updateFlg; }

	void SetReaction(const bool _reaction) { m_reactionFlg = _reaction; }

	static const float k_rotAngleMax;	// 回転角度の限界値
	static const float k_alphaMax;	// 透明度の限界値

protected:

	void Release() {}

	// 描画タイプ・何の描画を行うのかを決める / 最適な描画リスト作成用
	UINT m_drawType = 0;

	// カメラからの距離
	float m_distSqrFromCamera = 0;

	// 存在消滅フラグ
	bool m_isExpired = false;

	// 3D空間に存在する機能
	Math::Matrix	m_mWorld;

	// 当たり判定クラス
	std::unique_ptr<KdCollider> m_pCollider = nullptr;

	// デバッグ情報クラス
	std::unique_ptr<KdDebugWireFrame> m_pDebugWire = nullptr;

	//オブジェクトタイプ
	ObjectType m_ObjType = ObjectType::None;

	// キーオブジェクト判断
	KeyObject m_keyObject = KeyObject::Zero;

	//磁力判断フラグ
	UINT m_maguneForce = 0;
	
	// 座標
	Math::Vector3 m_pos;

	// 前の座標
	Math::Vector3 m_previosPos;

	Math::Vector3 m_size;

	//更新判断フラグ
	bool m_updateFlg = false;

	//特定の更新処理判断フラグ
	bool m_reactionFlg = false;

	// プレイヤーの移動制限フラグ
	bool m_moveFlg = false;

	// カメラのローカル行列
	Math::Matrix m_localCamera;
};
