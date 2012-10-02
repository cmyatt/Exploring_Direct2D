#pragma once

#include "Log.h"
#include "Drawable.h"

#include <map>
#include <memory>
#include <vector>
#include <d2d1.h>
#include <WTypes.h>
#include <dwrite.h>
#include <wincodec.h>
#include <wincodecsdk.h>

#define DEBUG	true

/*
	REMEMBER TO CHECK DEBUG BEFORE LOGGING ERRORS

	Add functions for loading and drawing images and text
	Create sprite class to encapulate image drawing.
*/

typedef enum {
	NORMAL, BOLD, ITALIC
} font_style;


class AppManager
{
public:
	AppManager(void);
	~AppManager(void);
	static std::shared_ptr<AppManager> getInstance(HWND win = NULL);

	bool loadBitmap(const std::string& filename,
					UINT destinationWidth,
					UINT destinationHeight,
					ID2D1Bitmap **bitmap);

	bool newTextStyle(const std::string& name,
					  const std::string& font = "Verdana",
					  float size = 12.0f,
					  font_style style = NORMAL,
					  D2D1::ColorF colour = D2D1::ColorF::Black);

	bool textToBitmap(const std::string& text,
					  const std::string& style,
					  ID2D1Bitmap **bitmap);

	void update();

	void addDrawable(Drawable *d) {
		drawable_objs.push_back(d);
	};
	ID2D1Factory *getFactory() {
		return factory;
	};
	ID2D1HwndRenderTarget *getRenderTarget() {
		return render_target;
	};

private:
	AppManager(const AppManager& app);
	bool init(HWND window);

	Log logger;
	static std::shared_ptr<AppManager> instance;

	// Vectors for drawable and other interface objects
	std::map<std::string, std::pair<IDWriteTextFormat *, ID2D1SolidColorBrush *>> text_styles;
	std::vector<Drawable *> drawable_objs;

	HWND win;
	ID2D1Factory *factory;
	IDWriteFactory *write_factory;
	IDWriteTextFormat *text_format;
	ID2D1SolidColorBrush *text_brush;
	IWICImagingFactory *img_factory;
	ID2D1HwndRenderTarget *render_target;
};