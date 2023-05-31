#include "File.hpp"

CsvData::CsvData(CSV* csv) : parent_(csv) {}

const std::string& CsvData::operator[](const std::string& index) const {
	uint32_t hash = static_cast<uint32_t>(std::hash<std::string>{}(index));
	for (int i = 0; i < parent_->GetHeader().size(); i++) {
		if (parent_->GetHeader()[i] == hash) {
			return cellList_[i];
		}
	}
	return cellList_[0];
}

std::string& CsvData::operator[](const std::string& index) {
	uint32_t hash = static_cast<uint32_t>(std::hash<std::string>{}(index));
	for (int i = 0; i < parent_->GetHeader().size(); i++) {
		if (parent_->GetHeader()[i] == hash) {
			return cellList_[i];
		}
	}
	return cellList_[0];
}


std::vector<Token> tokenize(const std::string& jsonString) {
	std::vector<Token> tokens;
	for (uint32_t i = 0; i < jsonString.length(); i++) {
		char c = jsonString[i];
		if (c == ' ' || c == '\n' || c == '\r' || c == '\t') {
			continue;
		} else if (c == '{') {
			tokens.push_back(Token{JsonTokenType::ObjectBegin, ""});
		} else if (c == '}') {
			tokens.push_back(Token{JsonTokenType::ObjectEnd, ""});
		} else if (c == '[') {
			tokens.push_back(Token{JsonTokenType::ArrayBegin, ""});
		} else if (c == ']') {
			tokens.push_back(Token{JsonTokenType::ArrayEnd, ""});
		} else if (c == ',') {
			tokens.push_back(Token{JsonTokenType::Comma, ""});
		} else if (c == ':') {
			tokens.push_back(Token{JsonTokenType::Colon, ""});
		} else if (c == '"') {
			c = jsonString[++i];
			std::string str = "";
			size_t jsonLength = jsonString.length();
			while (c != '"' && i < jsonLength) {
				str += c;
				i++;
			}
			tokens.push_back(Token{JsonTokenType::String, ""});
		} else if (c <= '9' && c >= '0' || c == '-') {
			std::string str = "";
			size_t jsonLength = jsonString.length();
			while ((c <= '9' && c >= '0' || c == '-' || c == '.') && i < jsonLength) {
				str += c;
				i++;
			}
			tokens.push_back(Token{JsonTokenType::Number, str});

		} else if (c == 't' || c == 'f') {
			tokens.push_back(Token{JsonTokenType::Boolean, ""});
			while (true) {
				if (jsonString[++i] == 'e') {
					break;
				}
			}
		}
	}

	return tokens;
}

JsonValue* parse(std::vector<Token> tokens) { return nullptr; }
