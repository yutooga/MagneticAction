#include"ModelManager.h"

// 非同期にモデルをロードする関数
void ModelManager::LoadModelFromCSVAsync(const std::string& _fileName)
{
	std::ifstream file(_fileName);
	if (!file)
	{
		std::cerr << "Failed to open file: " << _fileName << std::endl;
		return;
	}

	std::string line;
	// ヘッダーを読み飛ばす
	std::getline(file, line);

	std::vector<std::pair<std::string, std::string>> modelData;
	while (std::getline(file, line))
	{
		if (line.empty())
		{
			continue;
		}
		std::istringstream ss(line);
		std::string name, modelPath;
		std::getline(ss, name, ',');
		std::getline(ss, modelPath, ',');
		modelData.emplace_back(name, modelPath);
	}

	// 非同期タスクの制限を設ける
	const size_t maxConcurrentTasks = std::thread::hardware_concurrency();
	std::vector<std::future<void>> futures;

	for (const auto& data : modelData)
	{
		if (futures.size() >= maxConcurrentTasks)
		{
			futures.front().get();
			futures.erase(futures.begin());
		}

		futures.push_back(std::async(std::launch::async, [this, data]() {
			try {
				const std::string& name = data.first;
				const std::string& modelPath = data.second;

				// モデルオブジェクトを作成してロード
				auto model = std::make_shared<KdModelWork>();
				if (model->Load(modelPath)) {
					model->SetModelData(modelPath);
					std::lock_guard<std::mutex> lock(m_mutex);
					m_models[name] = model;
				}
				else {
					// ロード失敗時の処理（ログ出力など）
					std::cerr << "Failed to load model: " << modelPath << std::endl;
				}
			}
			catch (const std::bad_alloc& e) {
				std::cerr << "Memory allocation failed while loading model: " << e.what() << std::endl;
			}
			catch (const std::exception& e) {
				std::cerr << "Exception occurred while loading model: " << e.what() << std::endl;
			}
			}));

	}

	// 残った非同期タスクの完了を待つ
	for (auto& future : futures)
	{
		future.get();
	}
}

// 名前でモデルを取得する関数
const std::shared_ptr<KdModelWork>& ModelManager::GetModel(const std::string& _name)
{
	auto it = m_models.find(_name);
	if (it != m_models.end())
	{
		return it->second;
	}
	return nullptr; // 見つからなかった場合はnullptrを返す
}

// すべてのモデルを解放する関数
void ModelManager::Release()
{
	m_models.clear(); // unordered_map内のshared_ptrが全て解放される
}