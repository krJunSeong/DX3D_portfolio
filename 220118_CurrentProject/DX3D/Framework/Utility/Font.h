#pragma once

class Font : public Singleton<Font>
{
private:
	friend class Singleton;

	struct FontDesc
	{
		ID2D1SolidColorBrush* brush = nullptr;
		IDWriteTextFormat* format = nullptr;

		~FontDesc()
		{
			brush->Release();
			format->Release();
		}
	};

	ID2D1Factory1* factory;
	IDWriteFactory* writeFactory;

	ID2D1Device* device;
	ID2D1DeviceContext* context;

	ID2D1Bitmap1* targetBitmap;

	map<string, FontDesc*> fonts;

	Font();
	~Font();

public:
	void Add(string key, wstring font, Float3 color = Float3(1, 1, 1), float size = 20.0f,
		DWRITE_FONT_WEIGHT weight = DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE style = DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH stretch = DWRITE_FONT_STRETCH_NORMAL);

	void RenderText(wstring text, string key, Float2 pos);
	void RenderText(wstring text, string key, Float2 pos, Float2 size);

	void RenderText(string text, string key, Float2 pos);
	void RenderText(string text, string key, Float2 pos, Float2 size);

	ID2D1DeviceContext* GetDC() { return context; }

	static string ToString(wstring value);
	static wstring ToWString(string value);
};