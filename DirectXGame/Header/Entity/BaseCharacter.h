#pragma once
#include <string>
#include <unordered_map>

class Model;

class BaseCharacter {

protected:
	std::unordered_map<std::string, Model*> model_;

public:
	BaseCharacter() = default;
	virtual ~BaseCharacter() = default;


	void Init(const std::unordered_map<std::string, Model*>& model);

private:
};