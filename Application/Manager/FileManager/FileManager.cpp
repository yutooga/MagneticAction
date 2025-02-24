#include "FileManager.h"
#include"../../main.h"

std::optional<std::vector<std::vector<float>>> FileManager::GetData(const std::string& _filename)
{
	// 最終的に情報を集約するリスト
	std::vector<std::vector<float>> dataList;

	bool onceFlg = false;

	std::ifstream ifs(_filename);

	// ファイルが開けなかった場合の早期リターンとエラーメッセージ
	if (!ifs.is_open()) {
		std::cerr << "Failed to open file: " << _filename<< std::endl;
		return std::nullopt;
	}

	// CSVから敵情報を読み込む
	std::string lineString;
	while (std::getline(ifs, lineString)) {
		std::istringstream iss(lineString);
		std::string commaString;

		if (!onceFlg)
		{
			onceFlg = true;
			continue;
		}


		// 行の最初のセルを読み込み
		if (!std::getline(iss, commaString, ',')) {
			continue; // 空行の場合はスキップ
		}

		// 最初のセルが空白の場合、読み込みを終了
		if (commaString.empty()) {
			break;
		}

		// 最初のセルの値を変換し、行のデータに追加
		std::vector<float> row;
		try {
			row.push_back(std::stof(commaString));
		}
		catch (const std::invalid_argument& e) {
			Application::Instance().m_log.AddLog("Invalid number in CSV file: %s\n", commaString);
		}
		catch (const std::out_of_range& e) {
			Application::Instance().m_log.AddLog("Number out of range in CSV file: %s\n", commaString);
		}

		// 2番目以降のセルを処理
		while (std::getline(iss, commaString, ',')) {
			if (commaString.empty()) {
				break; // 空のセルはスキップ
			}
			try {
				row.push_back(std::stof(commaString));
			}
			catch (const std::invalid_argument& e) {
				Application::Instance().m_log.AddLog("Invalid number in CSV file: %s\n", commaString);
			}
			catch (const std::out_of_range& e) {
				Application::Instance().m_log.AddLog("Number out of range in CSV file: %s\n", commaString);
			}
		}

		// 各行の内容を保存する
		dataList .push_back(row);
	}

	ifs.close();

	// まとめあがったリストを結果として返す
	return dataList;
}
