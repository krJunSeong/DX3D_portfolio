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

		// 데이터 입력부분
		loadStream >> data.Hp >> data.Mp >> data.Stamina >> data.Att
			>> data.Def >> data.Level >> data.Exp;

		// 멤버변수에 데이터 넣어주는 부분
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

	UINT colCount = 7;	// 열 갯수, 콜 갯수 바뀌면 수정할 것

	string str;

	// Key ~ price 애트리뷰트Name 생략부분
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

		if (data.key == 0) return;	// data가 없으면 0값이 들어감

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
