#pragma once

#include <cassert>
#include <iostream>
#include <map>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

#include <fstream>
#include <sstream>

class File {
public:
	File(const std::string& filename) : filename_(filename) {}

	operator std::string() const {
		std::ifstream file(filename_);

		// istreambuf_iteratorの先頭と末端を指定する方法。vectorの場合であるが、引数が同じなので可能。
		// https://qiita.com/kerokerop0607/items/111b2a323e03f07d30b5
		std::istreambuf_iterator<char> it(file); // 先頭イテレータ
		std::istreambuf_iterator<char> last;     // endイテレータ

		std::string data{it, last};
		return data;
	}

	std::string ToString() const { return std::string(*this); }
	std::stringstream ToSStream() const {
		std::ifstream file(filename_);
		std::stringstream data;
		data << file.rdbuf();
		return data;
	}

	operator std::stringstream() const { return this->ToSStream(); }

	File& operator=(const std::string& data) {
		std::ofstream file(filename_);
		file << data;
		return *this;
	}

	File& operator=(const std::stringstream& data) {
		std::ofstream file(filename_);
		file << data.rdbuf();
		return *this;
	}

	File& operator+=(const std::string& data) {
		std::ofstream file(filename_, std::ios::app);
		file << data;
		return *this;
	}

	File& operator+=(const std::stringstream& data) {
		std::ofstream file(filename_, std::ios::app);
		file << data.rdbuf();
		return *this;
	}

private:
	const std::string filename_;
};

class CSV;

class CsvData {

public:
	CsvData(CSV*);
	CsvData& operator=(const std::vector<std::string>& data) {
		cellList_ = data;
		return *this;
	}
	CsvData& operator=(const std::string& line) {
		std::string cell = "";
		std::stringstream lineBuff = std::stringstream(line);
		while (std::getline(lineBuff, cell, ',')) {
			cellList_.push_back(cell);
		}
		return *this;
	}
	const std::string operator[](int index) const { return cellList_.at(index); }

	const std::string& operator[](const std::string& index) const;

	std::string& operator[](int index) { return cellList_.at(index); }

	std::string& operator[](const std::string& index);

	const size_t size() const { return cellList_.size(); }
	const auto begin() const { return cellList_.begin(); }
	const auto end() const { return cellList_.end(); }

private:
	std::vector<std::string> cellList_;
	CSV* const parent_;
};

// CSVクラス
class CSV {
public:
	// コンストラクタ
	CSV() {}
	CSV(const std::string& str) {
		data_.clear();
		std::string line = "";
		std::stringstream sstream(str);
		while (std::getline(sstream, line)) {
			CsvData csvData(this);
			data_.push_back(csvData = line);
		}
	}
	CSV(const char str[]) {
		data_.clear();
		std::string line = "";
		std::stringstream sstream(str);
		while (std::getline(sstream, line)) {
			CsvData csvData(this);
			data_.push_back(csvData = line);
		}
	}
	CSV(File& file) {
		data_.clear();
		std::string line = "";
		while (std::getline(file.ToSStream(), line)) {
			CsvData csvData(this);
			data_.push_back(csvData = line);
		}
	}

	// CSVデータを表示する
	void Printf() {
		for (auto& row : data_) {
			for (auto& cell : row) {
				std::cout << cell << " ";
			}
			std::cout << std::endl;
		}
	}

	CSV& operator+=(const std::vector<std::string>& cellList) {
		CsvData csvData(this);
		csvData = cellList;
		data_.push_back(csvData);
		return *this;
	}

	CSV& operator+=(const std::vector<double>& cellList) {
		std::vector<std::string> strListBuff;
		for (double d : cellList) {
			strListBuff.push_back(std::to_string(d));
		}
		CsvData csvData(this);
		csvData = strListBuff;
		data_.push_back(csvData);
		return *this;
	}

	CSV& operator=(const std::string& str) {
		data_.clear();
		std::string line = "";
		while (std::getline(std::stringstream(str), line)) {
			CsvData csvData(this);
			data_.push_back(csvData = line);
		}
		return *this;
	}

	CSV& operator=(File& file) {
		data_.clear();
		std::string line = "";
		while (std::getline(file.ToSStream(), line)) {
			CsvData csvData(this);
			data_.push_back(csvData = line);
		}
		return *this;
	}

	operator std::string() {
		std::string out = "";
		for (auto& element : data_) {
			for (int i = 0; i < element.size(); ++i) {
				out += element[i];
				if (i != element.size() - 1) {
					out += ",";
				}
			}
			out += "\n";
		}
		return out;
	}

	const auto& GetHeader() {
		if (headHash_.empty()) {
			CreateHeader();
		}
		return headHash_;
	}

	const auto& GetData() const { return data_; }

	CsvData& operator[](const std::string& s) {
		for (int i = 0; i < data_.size(); i++) {
			if (data_[i][0] == s) {
				return data_[i];
			}
		}
	}

	// []演算子をオーバーロード
	CsvData& operator[](int i) { return data_[i]; }

private:
	std::vector<uint32_t> headHash_;
	std::vector<CsvData> data_;

	void CreateHeader() {
		headHash_.clear();
		for (uint32_t i = 0; i < data_[0].size(); i++) {
			uint32_t hash = static_cast<uint32_t>(std::hash<std::string>{}(data_[0][i]));
			headHash_.emplace_back(hash);
		}
	}
};

enum class JsonTokenType {
	ObjectBegin,
	ObjectEnd,
	ArrayBegin,
	ArrayEnd,

	Comma,
	Colon,

	String,
	Number,
	Boolean,
	Null
};

struct Token {
	JsonTokenType type;
	std::string value;
};

/// @brief Jsonに保存された値
class JsonValue {
public:
	virtual ~JsonValue() {}
};

class JsonNumber : public JsonValue {
public:
	/// @brief 数値型保存
	/// @param value 数値
	JsonNumber(const double& value) { value_ = value; }
	double value_;
};

class JsonBoolean : public JsonValue {
public:
	/// @brief 真偽型保存
	/// @param value 真偽値
	JsonBoolean(const bool& value) { value_ = value; }
	bool value_;
};

class JsonString : public JsonValue {
public:
	/// @brief 文字列型保存
	/// @param value 文字列
	JsonString(const std::string& value) { value_ = value; }
	std::string value_;
};

class JsonArray {
public:
	~JsonArray() {
		for (auto& element : value_) {
			delete element;
		}
	}
	std::vector<JsonValue*> value_;
};

class JsonObject : public JsonValue {
public:
	~JsonObject() {
		for (auto& element : value_) {
			delete element.second;
		}
	}
	std::unordered_map<std::string, JsonValue*> value_;
};

std::vector<Token> tokenize(const std::string& jsonString);
JsonValue* parse(std::vector<Token> tokens);

inline JsonObject* StringToJson(const std::string& jsonString) {
	const std::vector<Token> tokens = tokenize(jsonString);
	JsonValue* jsonValue = parse(tokens);
	if (typeid(*jsonValue) == typeid(JsonObject)) {
		return dynamic_cast<JsonObject*>(jsonValue);
	}
	delete jsonValue;
	return nullptr;
}