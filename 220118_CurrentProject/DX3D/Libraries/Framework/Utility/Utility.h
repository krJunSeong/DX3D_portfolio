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
	Extention: 확장자

	특정 경로 문자열을 받아서 확장자만 받아올 수 있겠어?
	코딩테스트 중에 문자열
*/