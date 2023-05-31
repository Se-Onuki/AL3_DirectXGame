#pragma once
#include "Header/File/File.hpp"
#include "math/Vector3.h"
#include <list>

struct SpawnerCommand {
	uint32_t flame = 0u;
	Vector3 spawnPoint = Vector3::zero();
	std::string commandName = {};
};
class GameScene;

class EnemySpawner {
public:
	void Init();
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }
	void LoadCSV(const std::string& file_path);
	void Update();

private:
	/*CSV spawnerData_;*/
	std::list<SpawnerCommand> spawnerData_;
	uint32_t nowFlame_ = 0u;
	GameScene* gameScene_ = nullptr;
};
