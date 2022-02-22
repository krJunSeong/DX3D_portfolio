#pragma once

struct ItemData
{
	UINT key;
	string name;
	UINT attack;
	UINT defence;
	UINT price;
	UINT image;
	string info;
};

struct PlayerStatus
{
	UINT key;
	int Hp;
	float Mp;
	float Stamina;
	UINT Att;
	UINT Def;
	UINT Level;
	UINT Exp;
};

struct MonsterStatus
{
	UINT key;
	int Hp;
	float Mp;
	float Stamina;
	UINT Att;
	UINT Def;
};

class DataManager : public Singleton<DataManager>
{
private:
	friend class Singleton;
	struct EquitStatus
	{
		UINT attack;
		UINT defence;
	}playerEquit{};

	map<UINT, ItemData> itemDatas;
	map<UINT, PlayerStatus> playerDatas;
	map<UINT, MonsterStatus> monsterDatas;

	DataManager();
	~DataManager();
public:
	map<UINT, ItemData> GetItemData() { return itemDatas; }
	PlayerStatus GetPlayerData(UINT key) { return playerDatas[key]; }
	MonsterStatus GetMonsterData(UINT key) { return monsterDatas[key]; }
	EquitStatus GetPlayerEquit() { return playerEquit; }
	void AddplayerEquit(UINT att = 0, UINT def = 0) 
	{
		playerEquit.attack += att;
		playerEquit.defence += def;
	};

private:
	void LoadStatus(string filePath);
	void LoadMonsterStatus();
	void LoadItemData();
	
};