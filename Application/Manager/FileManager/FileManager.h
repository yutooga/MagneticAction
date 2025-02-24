#pragma once

class FileManager
{
public:
	// インスタンス取得関数
	static FileManager& Instance()
	{
		static FileManager instance;
		return instance;
	}

	// コピーコンストラクタと代入演算子を削除
	FileManager(const FileManager&) = delete;
	FileManager& operator=(const FileManager&) = delete;

	//　ファイルの中の値を数値として変換して返す関数
	std::optional<std::vector<std::vector<float>>> GetData(const std::string& _filename);

private:
	// プライベートコンストラクタ
	FileManager(){}
	~FileManager(){}

	std::mutex m_mutex; // スレッドセーフのためのミューテックス
};