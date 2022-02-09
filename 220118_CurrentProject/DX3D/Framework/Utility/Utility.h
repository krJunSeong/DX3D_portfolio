#pragma once

namespace Utility
{
    vector<string> SplitString(string origin, string tok);
    void Replace(string* str, string comp, string rep);

    string ToString(wstring value);
    wstring ToWString(string value);

    wstring GetExtension(wstring path);
    string GetFileName(string path);
    string GetFileNameWidthoutExtention(string path);

    bool ExistDirectory(string path);
    bool ExistFile(string path);

    void CreateFolders(string path);
}
/*
	Extention: Ȯ����

	Ư�� ��� ���ڿ��� �޾Ƽ� Ȯ���ڸ� �޾ƿ� �� �ְھ�?
	�ڵ��׽�Ʈ �߿� ���ڿ�
*/