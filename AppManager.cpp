#include "AppManager.h"
#include "util.h"
#include <ctime>
using namespace std;

shared_ptr<AppManager> AppManager::instance = nullptr;

AppManager::~AppManager(void)
{
	for (auto i = text_styles.begin(); i != text_styles.end(); ++i) {
		SafeRelease(&(i->second.first));
		SafeRelease(&(i->second.second));
	}
	SafeRelease(&text_brush);
	SafeRelease(&text_format);
	SafeRelease(&write_factory);
	SafeRelease(&img_factory);
	SafeRelease(&render_target);
	SafeRelease(&factory);
	CoUninitialize();
}

shared_ptr<AppManager> AppManager::getInstance(HWND win)
{
	if (instance == nullptr) {
		if (!win)
			return nullptr;
		instance = make_shared<AppManager>();
		if (!instance->init(win))
			instance = nullptr;
	}
	return instance;
}

bool AppManager::loadBitmap(const string& filename,
							   UINT destinationWidth,
							   UINT destinationHeight,
							   ID2D1Bitmap **bitmap)
{
	IWICBitmapDecoder *decoder = NULL;
	IWICBitmapFrameDecode *source = NULL;
	IWICStream *stream = NULL;
	IWICFormatConverter *converter = NULL;
	IWICBitmapScaler *scaler = NULL;
	IWICBitmapSource *src = NULL;
	string err_msg;
	HRESULT hr;

	for (;;) {
		hr = img_factory->CreateDecoderFromFilename(convertToWideString(filename).c_str(),
													NULL,
													GENERIC_READ,
													WICDecodeMetadataCacheOnLoad,
													&decoder);
		if (!SUCCEEDED(hr)) {
			err_msg = "Unable to create bitmap decoder for: \"" + filename + "\"";
			break;
		}

		// Create the initial frame.
		hr = decoder->GetFrame(0, &source);
		src = source;
		if (!SUCCEEDED(hr)) {
			err_msg = "Unable to get initial image frame";
			break;
		}

		// Convert the image format to 32bppPBGRA
		// (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
		hr = img_factory->CreateFormatConverter(&converter);
		if (!SUCCEEDED(hr)) {
			err_msg = "Unable to create format converter";
			break;
		}

		// If a new width or height was specified resize the image.
		if (destinationWidth != 0 || destinationHeight != 0) {
			UINT originalWidth, originalHeight;
			hr = source->GetSize(&originalWidth, &originalHeight);
			if (!SUCCEEDED(hr)) {
				err_msg = "Unable to get image size";
				break;
			}

			if (destinationWidth == 0) {
				float scalar = static_cast<float>(destinationHeight) / static_cast<float>(originalHeight);
				destinationWidth = static_cast<UINT>(scalar * static_cast<float>(originalWidth));
			} else if (destinationHeight == 0) {
				float scalar = static_cast<float>(destinationWidth) / static_cast<float>(originalWidth);
				destinationHeight = static_cast<UINT>(scalar * static_cast<float>(originalHeight));
			}

			hr = img_factory->CreateBitmapScaler(&scaler);
			if (!SUCCEEDED(hr)) {
				err_msg = "Unable to create image scaler to resize image";
				break;
			}

			hr = scaler->Initialize(source, destinationWidth, destinationHeight, WICBitmapInterpolationModeCubic);
			src = scaler;
			if (!SUCCEEDED(hr)) {
				err_msg = "Unable to initialize image scaler";
				break;
			}
		}
		hr = converter->Initialize(src,
								   GUID_WICPixelFormat32bppPBGRA,
								   WICBitmapDitherTypeNone,
								   NULL,
								   0.f,
								   WICBitmapPaletteTypeMedianCut);
		if (!SUCCEEDED(hr)) {
			err_msg = "Unable to initialize image converter";
			break;
		}
		
		hr = render_target->CreateBitmapFromWicBitmap(converter, NULL, bitmap);
		if (!SUCCEEDED(hr)) {
			err_msg = "Unable to create bitmap from WIC bitmap";
		}
		break;
	}

	SafeRelease(&decoder);
	SafeRelease(&source);
	SafeRelease(&stream);
	SafeRelease(&converter);
	SafeRelease(&scaler);

	if (DEBUG && !SUCCEEDED(hr))
		logger.error("AppManager.cpp", "loadBitmap()", err_msg);

	return SUCCEEDED(hr)? true : false;
}

// Overwrites any existing formats with the same name.
// Returns true if format created successfully, false otherwise.
bool AppManager::newTextStyle(const string& name,
							  const string& font,
							  float size,
							  font_style style,
							  D2D1::ColorF colour)
{
	HRESULT hr;
	string err_msg;
	IDWriteTextFormat *format;
	ID2D1SolidColorBrush *brush = NULL;
	DWRITE_FONT_STYLE f_style = DWRITE_FONT_STYLE_NORMAL;

	if (style == BOLD)
		f_style = DWRITE_FONT_STYLE_OBLIQUE;
	else if (style == ITALIC)
		f_style = DWRITE_FONT_STYLE_ITALIC;

	for (;;) {
		hr = write_factory->CreateTextFormat(convertToWideString(font).c_str(),
											 NULL,
											 DWRITE_FONT_WEIGHT_NORMAL,
											 f_style,
											 DWRITE_FONT_STRETCH_NORMAL,
											 size,
											 L"",
											 &format);
		if (!SUCCEEDED(hr)) {
			err_msg = "Unable to create text format";
			break;
		}

		hr = render_target->CreateSolidColorBrush(colour, &brush);
		if (!SUCCEEDED(hr)) {
			err_msg = "Unable to create brush";
			break;
		}

		if (text_styles[name].first != NULL)
			SafeRelease(&text_styles[name].first);
		if (text_styles[name].second != NULL)
			SafeRelease(&text_styles[name].second);

		text_styles[name].first = format;
		text_styles[name].second = brush;
		break;
	}

	if (DEBUG && !SUCCEEDED(hr))
		logger.error("AppManager.cpp", "newTextStyle()", err_msg);

	return SUCCEEDED(hr)? true : false;
}

// Will probably need to change size of bitmap
// Currently set to square with side length = text.size() * fontSize()
bool AppManager::textToBitmap(const string& text,
							  const string& style,
							  ID2D1Bitmap **bitmap)
{
	HRESULT hr;
	string err_msg;
	ID2D1BitmapRenderTarget *bitmapTarget = NULL;

	for (;;) {
		float size = text_styles[style].first->GetFontSize()*text.size();
		hr = render_target->CreateCompatibleRenderTarget(D2D1::SizeF(size, size),&bitmapTarget);
		if (!SUCCEEDED(hr)) {
			err_msg = "Unable to create bitmap render target";
			break;
		}

		if (text_styles[style].first == NULL) {
			err_msg = "Text format for \""+style+"\" is NULL";
			hr = E_FAIL;
			break;
		} else if (text_styles[style].second == NULL) {
			err_msg = "Brush for \""+style+"\" is NULL";
			hr = E_FAIL;
			break;
		}

		D2D1_RECT_F rect;
		rect.left = rect.top = 0;
		rect.right = bitmapTarget->GetSize().width;
		rect.bottom = bitmapTarget->GetSize().height;
		bitmapTarget->DrawText(convertToWideString(text).c_str(),
							   text.size(),
							   text_styles[style].first,
							   &rect,
							   text_styles[style].second);
		hr = bitmapTarget->EndDraw();
		if (!SUCCEEDED(hr)) {
			err_msg = "Problem drawing to bitmap render target";
			break;
		}

		hr = bitmapTarget->GetBitmap(bitmap);
		if (!SUCCEEDED(hr))
			err_msg = "Unable to get bitmap from render target";
		break;
	}

	SafeRelease(&bitmapTarget);

	if (DEBUG && !SUCCEEDED(hr))
		logger.error("AppManager.cpp", "textToBitmap()", err_msg, win);
	
	return SUCCEEDED(hr)? true : false;
}

void AppManager::update()
{
	HRESULT hr;
	D2D1::ColorF col = D2D1::ColorF::LightGreen;
	render_target->BeginDraw();
	render_target->Clear(col);
	for (auto i = drawable_objs.cbegin(); i != drawable_objs.cend(); ++i) {
		hr = (*i)->draw(render_target);
		if (DEBUG && !SUCCEEDED(hr))
			logger.error("AppManager.cpp", "update()", "Problem drawing object");
	}
	hr = render_target->EndDraw();
	if (DEBUG && !SUCCEEDED(hr))
		logger.error("AppManager.cpp", "update()", "Unable to end drawing");
}

AppManager::AppManager(void)
	: win(NULL),
	  factory(NULL),
	  write_factory(NULL),
	  text_format(NULL),
	  text_brush(NULL),
	  img_factory(NULL),
	  render_target(NULL)
{}

bool AppManager::init(HWND window)
{
	win = window;
	srand(static_cast<unsigned int>(time(NULL)));

	if (DEBUG && !logger.init("C:/Users/Chris/Documents/Visual Studio 2010/Projects/Game/log.txt")) {
		MessageBox(window, "Unable to initialize logger", "Error", MB_OK);
		return false;
	}

	HRESULT hr = S_OK;
	string err_msg;

	for (;;) {
		hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);
		if (!SUCCEEDED(hr)) {
			err_msg = "Unable to create Direct2D factory";
			break;
		}

		RECT rc;
		GetClientRect(win, &rc);
		hr = factory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(win, D2D1::SizeU(rc.right-rc.left, rc.bottom-rc.top)),
			&render_target);
		if (!SUCCEEDED(hr)) {
			err_msg = "Unable to create Direct2D render target";
			break;
		}

		hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&img_factory));
		if (!SUCCEEDED(hr)) {
			err_msg = "Unable to create Windows Imaging Component factory";
			break;
		}
		if (!SUCCEEDED(hr = CoInitialize(NULL))) {
			err_msg = "Unable to initialize windows COM components";
			break;
		}

		hr = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(write_factory),
			reinterpret_cast<IUnknown **>(&write_factory));
		if (!SUCCEEDED(hr)) {
			err_msg = "Unable to create write factory";
			break;
		}

		hr = write_factory->CreateTextFormat(
			convertToWideString("Verdana").c_str(),
			NULL,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			12.0f,
			L"",
			&text_format);
		if (!SUCCEEDED(hr)) {
			err_msg = "Unable to create write format";
			break;
		}

		D2D1::ColorF colour = D2D1::ColorF::Black;
		hr = render_target->CreateSolidColorBrush(colour, &text_brush);
		if (!SUCCEEDED(hr)) {
			err_msg = "Unable to create text brush";
		}
		break;
	}

	if (DEBUG && !SUCCEEDED(hr))
		logger.error("AppManager.cpp", "init()", err_msg, win);

	return SUCCEEDED(hr)? true : false;
}