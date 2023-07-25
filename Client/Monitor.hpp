#pragma once

class Monitor
{
private:

	struct MonitorData
	{
		HMONITOR hMonitor;
		RECT rect;
		string name;
	};

	static BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
	{
		vector<MonitorData>* monitorList = reinterpret_cast<vector<MonitorData>*>(dwData);
		MONITORINFOEX monitorInfo;
		monitorInfo.cbSize = sizeof(MONITORINFOEX);
		GetMonitorInfo(hMonitor, &monitorInfo);

		MonitorData data;
		data.hMonitor = hMonitor;
		data.rect = *lprcMonitor;

		data.name = monitorInfo.szDevice;

		monitorList->push_back(data);

		return TRUE;
	}

public:

	vector<MonitorData> Monitors;

	void GetMonitorList()
	{
		EnumDisplayMonitors(nullptr, nullptr, MonitorEnumProc, reinterpret_cast<LPARAM>(&Monitors));
	}

	void Clear()
	{
		Monitors.clear();
	}

	Mat GetMatScreenshot(MonitorData monitor)
	{
		MONITORINFOEX monitorInfo;
		monitorInfo.cbSize = sizeof(MONITORINFO);
		GetMonitorInfo(monitor.hMonitor, &monitorInfo);

		int monitorWidth = monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left;
		int monitorHeight = monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top;

		HDC hDCScreen = GetDC(0);
		HDC hDCMonitor = CreateCompatibleDC(hDCScreen);

		// Creazione di un bitmap compatibile con il monitor
		HBITMAP hBitmap = CreateCompatibleBitmap(hDCScreen, monitorWidth, monitorHeight);

		SelectObject(hDCMonitor, hBitmap);

		// Copia del contenuto del monitor nel bitmap
		BitBlt(hDCMonitor, 0, 0, monitorWidth, monitorHeight, hDCScreen, monitorInfo.rcMonitor.left, monitorInfo.rcMonitor.top, SRCCOPY);

		// Draw the mouse
		CURSORINFO cursor = { sizeof(cursor) };
		GetCursorInfo(&cursor);
		if (cursor.flags == CURSOR_SHOWING)
		{
			DrawIcon(hDCMonitor, cursor.ptScreenPos.x, cursor.ptScreenPos.y, cursor.hCursor);
		}

		// Creazione di un oggetto Mat da usare con OpenCV
		cv::Mat screenshot;

		// Creazione di un oggetto Mat utilizzando i dati del bitmap
		BITMAPINFOHEADER bitmapInfoHeader;
		bitmapInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
		bitmapInfoHeader.biWidth = monitorWidth;
		bitmapInfoHeader.biHeight = -monitorHeight; // Meno per invertire l'asse y (origine in alto a sinistra)
		bitmapInfoHeader.biPlanes = 1;
		bitmapInfoHeader.biBitCount = 32;
		bitmapInfoHeader.biCompression = BI_RGB;
		bitmapInfoHeader.biSizeImage = 0;
		bitmapInfoHeader.biXPelsPerMeter = 0;
		bitmapInfoHeader.biYPelsPerMeter = 0;
		bitmapInfoHeader.biClrUsed = 0;
		bitmapInfoHeader.biClrImportant = 0;

		screenshot.create(monitorHeight, monitorWidth, CV_8UC4);

		GetDIBits(hDCMonitor, hBitmap, 0, monitorHeight, screenshot.data, (BITMAPINFO*)&bitmapInfoHeader, DIB_RGB_COLORS);

		// Pulizia delle risorse
		DeleteDC(hDCMonitor);
		DeleteObject(hBitmap);
		ReleaseDC(NULL, hDCScreen);

		return screenshot;
	}

};