#pragma once

class BinaryReader
{
private:
	HANDLE file;
	DWORD size;

public:
	BinaryReader(wstring filePath);
	BinaryReader(string filePath);
	~BinaryReader();

	int Int();
	UINT UInt();
	float Float();
	string String();
	wstring WString();

	class Vector3 Vector();
	XMMATRIX Matrix();

	void Byte(void** data, UINT dataSize);

	bool IsFailed() { return file == INVALID_HANDLE_VALUE; }
};