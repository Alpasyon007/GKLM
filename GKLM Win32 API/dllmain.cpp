#include "pch.h"

HWND _hWnd;
HWND _childhWnd;
HINSTANCE _hInstance;

HHOOK hHook = 0;
HWND currentWindow = 0;
HWND overrideWindow = 0;
HKL overrideKeyboardLayout = reinterpret_cast<HKL>(0x00000809);

DWORD prevKey = 0;
bool toggleOverride = false;

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK GetWindows(HWND hwnd, LPARAM lParam);
void AppLoop();

extern "C" {
	DLLEXPORT void HookKeyboard() {
		// Hook into keyboard
		hHook = SetWindowsHookEx(WH_KEYBOARD_LL, &LowLevelKeyboardProc, _hInstance, NULL);
	}

	DLLEXPORT void PassHandle(HWND hwnd) {
		overrideWindow = hwnd;
	}
}

//int __stdcall wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD fdwReason, LPVOID lpReserved)
{
	_hInstance = hInstance;
	// Perform actions based on the reason for calling.
	switch (fdwReason)
	{
		case DLL_PROCESS_ATTACH:
			// Initialize once for each new process.
			// Return FALSE to fail DLL load.
			break;

		case DLL_THREAD_ATTACH:
			// Do thread-specific initialization.
			break;

		case DLL_THREAD_DETACH:
			// Do thread-specific cleanup.
			break;

		case DLL_PROCESS_DETACH:
			// Perform any necessary cleanup.
			break;
	}
	return TRUE;  // Successful DLL_PROCESS_ATTACH.

	//// Hook into keyboard
	//hHook = SetWindowsHookEx(WH_KEYBOARD_LL, &LowLevelKeyboardProc, hInstance, NULL);

	//if (hHook == NULL)
	//{
	//	std::cout << "Error" << std::endl;
	//	return 1;
	//}

	//std::map<HWND, std::wstring> windows;
	//EnumWindows(GetWindows, reinterpret_cast<LPARAM>(&windows));
	//// At this point, titles if fully populated and could be displayed, e.g.:
	//OutputDebugString(L"\n");
	//for (auto& window : windows) {
	//	OutputDebugString(window.second.c_str());
	//	OutputDebugString(L"\n");
	//}

	////// Run the message loop.
	//MSG msg = { };
	//while (true) {
	//	while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	//	{
	//		TranslateMessage(&msg);
	//		DispatchMessage(&msg);
	//	}

	//	AppLoop();
	//}

	//return 0;
}

void AppLoop() {
	HWND foregroundWindow = GetForegroundWindow();
	if (currentWindow != foregroundWindow) {
		std::wstring title(GetWindowTextLength(foregroundWindow) + 1, L'\0');
		GetWindowTextW(foregroundWindow, &title[0], title.size());
		currentWindow = foregroundWindow;
		OutputDebugString(title.c_str());
		OutputDebugString(L"\n");
	}
}

BOOL CALLBACK GetWindows(HWND hwnd, LPARAM lParam) {
	int length = ::GetWindowTextLength(hwnd);
	std::wstring title(length + 1, L'\0');
	GetWindowTextW(hwnd, &title[0], title.size());

	if (!IsWindowVisible(hwnd) || length == 0 || std::wcscmp(title.c_str(), L"Program Manager") == 0 || std::wcscmp(title.c_str(), L"Settings") == 0) {
		return TRUE;
	}

	std::map<HWND, std::wstring>& window = *reinterpret_cast<std::map<HWND, std::wstring>*>(lParam);
	window.emplace(std::pair<HWND, std::wstring>(hwnd, title));

	return TRUE;
}


LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (overrideWindow != GetForegroundWindow()) {
		return CallNextHookEx(hHook, nCode, wParam, lParam);
	}

	if (nCode == HC_ACTION) {
		const KBDLLHOOKSTRUCT* const kbd = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);
		const DWORD vkCode = kbd->vkCode;

		if (wParam == WM_KEYDOWN && prevKey != VK_RETURN && vkCode == VK_RETURN) { toggleOverride = !toggleOverride; }

		prevKey = vkCode;

		if (toggleOverride) {
			const DWORD scanCode = kbd->scanCode;
			BYTE vKey = MapVirtualKeyEx(scanCode, MAPVK_VSC_TO_VK, overrideKeyboardLayout);

			if (vKey != VK_CONTROL) {
				switch (wParam) {
					case WM_KEYDOWN:
					{
						keybd_event(vKey, 0x1D, 0 | 0, 0);
						break;
					}
					case WM_KEYUP:
					{
						keybd_event(vKey, 0x1D, 0 | KEYEVENTF_KEYUP, 0);
						break;
					}
				}
				return 1;
			}
		}
	}

	return CallNextHookEx(hHook, nCode, wParam, lParam);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg)
	{
		case WM_DESTROY:
			UnhookWindowsHookEx(hHook);
			PostQuitMessage(0);
			return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}