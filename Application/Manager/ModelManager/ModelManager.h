#pragma once

class ModelManager
{
public:
	// インスタンス取得関数
	static ModelManager& Instance()
	{
		static ModelManager instance;
		return instance;
	}

	// コピーコンストラクタと代入演算子を削除
	ModelManager(const ModelManager&) = delete;
	ModelManager& operator=(const ModelManager&) = delete;

	// モデルをCSVファイルから非同期にロードする関数
	void LoadModelFromCSVAsync(const std::string& _fileName);

	// 名前でモデルを取得する関数
	const std::shared_ptr<KdModelWork>& GetModel(const std::string& _name);

	// すべてのモデルを解放する関数
	void Release();

private:
	// プライベートコンストラクタ
	ModelManager() = default;
	~ModelManager() = default;

	std::unordered_map<std::string, std::shared_ptr<KdModelWork>> m_models; // モデルデータのコンテナ
	std::mutex m_mutex; // スレッドセーフのためのミューテックス
};