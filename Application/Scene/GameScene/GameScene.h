#pragma once

#include"../BaseScene/BaseScene.h"
#include"../../GameObject/Terrains/Gimmick/Floor/DeathFloor/DeathFloor.h"

class KdGameObject;
class HP;
class Player;
class Pause;
class CameraBase;

class GameScene : public BaseScene
{
public :

	GameScene() { Init(); }
	~GameScene() {}

	// カメラのターゲットの設定
	bool SetCameraTarget(std::weak_ptr<KdGameObject> _target);

	void Update()override;

	void DrawImGui()override;

	enum class MagunetPower
	{
		GivePowerS,
		GivePowerN,
		GiveNoPower
	};

	static const Math::Vector2 k_cursorPos;

private:

	void Event() override;
	void Init() override;

	// 通常のギミックを読み取る関数
	void LoadGimmickDefaultParam(const std::string& _filePath);

	// 独自のパラメータを持つギミックを読み取る関数
	void LoadGimmickSpecialParam(const std::string& _filePath);

	// 動く床のギミックを読み取る関数
	void LoadMoveFloorGimmickParam(const std::string& _filePath);

	// 電流のエフェクトのパラメータを読み取る関数
	void LoadElectricityEffectObjectParam(const std::string& _filePath);

	// 磁力が切り替わる床の実体を生成する関数
	void CreateChangeFloor(float _angle,float _posX, float _posY, float _posZ,float _interval);

	// 魔法陣の実体を生成する関数
	void CreateMagicRing(KdGameObject::ObjectType _magune, const std::string& _effectName, float _posX, float _posY, float _posZ);

	void CreateDeathFloor(const Math::Vector3& _pos, DeathFloor::MoveState _state, KdGameObject::ObjectType _type, const float _scale = 8.f);

	// 門の開閉アニメーション関数
	void OpenGateAnimation();

	//特定のオブジェクト検索関数
	std::shared_ptr<KdGameObject> FindObject(const KdGameObject::ObjectType _obj);
	// 次のカメラのターゲットを検索
	KdGameObject::ObjectType FindNextTarget(const std::weak_ptr<KdGameObject>& _nowTarget);

	// 電磁誘導によるオブジェクト更新の判断
	void ElectroMagneticInductionUpdateObject();

	// ギミックのリスト
	std::optional<std::vector<std::vector<float>>> m_gimmickNormalData;
	std::optional<std::vector<std::vector<float>>> m_gimmickSpecialData;
	std::optional<std::vector<std::vector<float>>> m_moveFloorGimmickData;
	std::optional<std::vector<std::vector<float>>> m_electricityEffectData;

	// CSVからデータを読み取る際の各オブジェクトの共通項目
	enum class GimmickData
	{
		type,
		posX,
		posY,
		posZ
	};

	// 各オブジェクトの実体
	std::weak_ptr<HP> m_hp;
	std::weak_ptr<Player> m_player;
	std::weak_ptr<Pause> m_pause;
	std::weak_ptr<CameraBase> m_wpCamera;

	//ポーズ状態判定フラグ
	bool m_pauseFlg = false;

	//押しっぱ防止フラグ
	bool m_pushFlg = false;

	// 現在のカメラのターゲット
	KdGameObject::ObjectType m_nowCamTarget = KdGameObject::ObjectType::Player;
};
