#include "Framework.h"

vector<string> Utility::SplitString(string origin, string tok)
{
    vector<string> result;

    size_t cutAt = 0;

    while ((cutAt = origin.find_first_of(tok)) != origin.npos)
    {
        if (cutAt > 0)
            result.push_back(origin.substr(0, cutAt));

        origin = origin.substr(cutAt + 1);
    }

    return result;
}

void Utility::Replace(string* str, string comp, string rep)
{
    string temp = *str;

    size_t at = 0;
    while ((at = temp.find(comp, at)) != string::npos)
    {
        temp.replace(at, comp.length(), rep);
        at += rep.length();
    }

    *str = temp;
}

string Utility::ToString(wstring value)
{
    string temp;
    temp.assign(value.begin(), value.end());
    return temp;
}

wstring Utility::ToWString(string value)
{
    wstring temp;
    temp.assign(value.begin(), value.end());
    return temp;
}

wstring Utility::GetExtension(wstring path)
{
    string temp = ToString(path);
    Replace(&temp, "\\", "/");
    path = ToWString(temp);

    size_t index = path.find_last_of('.');

    return path.substr(index + 1, path.length());
}

string Utility::GetFileName(string path)
{
    Replace(&path, "\\", "/");

    size_t index = path.find_last_of('/');

    return path.substr(index + 1);
}

string Utility::GetFileNameWidthoutExtention(string path)
{
    string fileName = GetFileName(path);

    size_t index = fileName.find_last_of('.');

    return fileName.substr(0, index);
}

bool Utility::ExistDirectory(string path)
{
    DWORD fileValue = GetFileAttributesA(path.c_str());

    BOOL temp = (fileValue != INVALID_FILE_ATTRIBUTES && (fileValue & FILE_ATTRIBUTE_DIRECTORY));

    return temp == TRUE;
}

bool Utility::ExistFile(string path)
{
    DWORD fileValue = GetFileAttributesA(path.c_str());

    return fileValue < 0xffffffff;
}

void Utility::CreateFolders(string path)
{
    vector<string> folders = SplitString(path, "/");

    string temp = "";
    for (string folder : folders)
    {
        temp += folder + "/";

        if (!ExistDirectory(temp))
            CreateDirectoryA(temp.c_str(), 0);
    }
}
