#include "EnemySpawner.h"
#include "GameScene.h"

void EnemySpawner::Init() { nowFlame_ = 0u; }

void EnemySpawner::LoadCSV(const std::string& file_path) {
	CSV csv{File(file_path)};
	// for (const CsvData& line : csv.GetData()) {
	for (uint32_t i = 1; i < csv.GetData().size(); i++) {
		const CsvData& line = csv[i];
		SpawnerCommand spawnCommand;
		spawnCommand.flame = static_cast<uint32_t>(std::stoul(line["Flame"]));
		spawnCommand.commandName = line["CommanName"];
		spawnCommand.spawnPoint = {
		    std::stof(line["x"]), std::stof(line["y"]), std::stof(line["z"])};
		spawnerData_.push_back(spawnCommand);
	}
}

void EnemySpawner::Update() {
	if (spawnerData_.empty())
		return;
	if (spawnerData_.front().flame > nowFlame_++) { // もし指定した時間に達してなかったら無視
		return;
	}
	gameScene_->AddEnemy(spawnerData_.front().spawnPoint);
	spawnerData_.pop_front();
}
