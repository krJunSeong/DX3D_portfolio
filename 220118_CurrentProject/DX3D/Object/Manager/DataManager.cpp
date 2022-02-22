#include "Framework.h"

DataManager::DataManager()
{
	LoadItemData();
	LoadStatus("TextData/StatusTable.txt");
	LoadMonsterStatus();
}

DataManager::~DataManager()
{
}

void DataManager::LoadStatus(string filePath)
{
	//"TextData/StatusTable.txt"

	ifstream loadStream("TextData/StatusTable.txt", ios::in);

	UINT colCount = 8;

	string str;

	int count = 0;
	while (true)
	{
		if (count < colCount)
		{
			loadStream >> str;
			count++;
			continue;
		}

		PlayerStatus data;
		data.key = 0;
		loadStream >> data.key;

		if (data.key == 0) return;

		// ������ �Էºκ�
		loadStream >> data.Hp >> data.Mp >> data.Stamina >> data.Att
			>> data.Def >> data.Level >> data.Exp;

		// ��������� ������ �־��ִ� �κ�
		playerDatas[data.key] = data;
	}
}

void DataManager::LoadMonsterStatus()
{
	ifstream loadStream("TextData/MonsterTable.txt", ios::in);

	UINT colCount = 6;

	string str;

	int count = 0;
	while (true)
	{
		if (count < colCount)
		{
			loadStream >> str;
			count++;
			continue;
		}

		MonsterStatus data;
		data.key = 0;
		loadStream >> data.key;

		if (data.key == 0) return;

		/*				Monster Data
			UINT key;
			int Hp;
			float Mp;
			float Stamina;
			UINT Att;
			UINT Def;
		*/

		loadStream >> data.Hp >> data.Mp >> data.Stamina >> data.Att
			>> data.Def;

		monsterDatas[data.key] = data;
	}
}

void DataManager::LoadItemData()
{
	ifstream loadStream("TextData/TalesItemTable.txt", ios::in);

	UINT colCount = 7;	// �� ����, �� ���� �ٲ�� ������ ��

	string str;

	// Key ~ price ��Ʈ����ƮName �����κ�
	int count = 0;
	while (true)
	{
		if (count < colCount)
		{
			loadStream >> str;
			count++;
			continue;
		}

		ItemData data;
		data.key = 0;
		loadStream >> data.key;

		if (data.key == 0) return;	// data�� ������ 0���� ��

		loadStream >> data.name >> data.attack >> data.defence >> data.price
			>> data.image >> data.info;

		string tempString;
		for (int i = 0; i < data.info.size(); i++)
		{
			if (data.info[i] == '_')
				data.info[i] = ' ';
		}

		itemDatas[data.key] = data;
	}
}
