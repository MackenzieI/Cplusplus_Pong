#include "utils.cpp"
#include <windows.h>
#include <cstdio>

global_variable bool running = true; 
global_variable bool speedbtn_clicked = false;

POINT ptOld; 

struct Render_State {
	int height, width;
	void* memory; 
	BITMAPINFO bitmapinfo;
};

Render_State renderState;

#include "platform_common.cpp"; 
#include "renderer.cpp";
#include "game.cpp"; 

LRESULT CALLBACK window_callback(HWND hwnd, UINT umsg, WPARAM wParam, LPARAM lParam) {
	LRESULT result = 0; 
	switch (umsg) {
		case WM_CLOSE:
		case WM_DESTROY:{ 
			running = false; }
		break; 

		case WM_SIZE: {
			RECT rect;
			GetClientRect(hwnd, &rect);
			renderState.width = rect.right - rect.left; 
			renderState.height = rect.bottom - rect.top; 

			int size = renderState.width * renderState.height * sizeof(unsigned int); 

			if (renderState.memory) VirtualAlloc(renderState.memory, 0, MEM_RELEASE, NULL);
			renderState.memory = VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE); // malloc() for general, cross-platform function 
		
			renderState.bitmapinfo.bmiHeader.biSize = sizeof(renderState.bitmapinfo.bmiHeader); 
			renderState.bitmapinfo.bmiHeader.biWidth = renderState.width; 
			renderState.bitmapinfo.bmiHeader.biHeight = renderState.height; 
			renderState.bitmapinfo.bmiHeader.biPlanes = 1;
			renderState.bitmapinfo.bmiHeader.biBitCount = 32;
			renderState.bitmapinfo.bmiHeader.biCompression = BI_RGB;

		} break;

		default: {
			result = DefWindowProc(hwnd, umsg, wParam, lParam);
		}
	}
	return result; 
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {

	ShowCursor(false);

	// Create Window Class
	WNDCLASS wc = { };
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszClassName = L"Game Window Class";
	wc.lpfnWndProc = window_callback;

	//  Register Class
	RegisterClass(&wc);

	// Create Window
	HWND window = CreateWindow(
		wc.lpszClassName,
		L"Game of Pong - First Game in C++",    // Window text
		WS_OVERLAPPEDWINDOW | WS_VISIBLE, // Window style
		// Size and position // x , y , width , height 
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,       // Parent window    
		NULL,       // Menu
		hInstance,  // Instance handle
		NULL        // Additional application data
	); /**
	{
		// Full Screen
		SetWindowLong(window, GWL_STYLE, GetWindowLong(window, GWL_STYLE) & -WS_OVERLAPPEDWINDOW);
		MONITORINFO mi = { sizeof(mi) };
		GetMonitorInfo(MonitorFromWindow(window, MONITOR_DEFAULTTOPRIMARY), &mi);
		SetWindowPos(window, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left,
			mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
	}*/
	HDC hdc = GetDC(window);

	Input input = {};

	float delta_time = 0.016666f;
	LARGE_INTEGER frame_begin_time;
	QueryPerformanceCounter(&frame_begin_time);

	float performance_frequency;
	{
		LARGE_INTEGER perf;
		QueryPerformanceFrequency(&perf);
		performance_frequency = (float)perf.QuadPart;
	}

	while (running) {
		//input
		MSG msg; 

		if (GetAsyncKeyState(VK_ESCAPE)) running = false;

		for (int i = 0; i < BUTTON_COUNT; i++) {
			input.buttons[i].changed = false;
		}

		while (PeekMessage(&msg, window, 0, 0, PM_REMOVE)) {
			if (GetAsyncKeyState(VK_ESCAPE)) PostQuitMessage(0);

			switch (msg.message) {
			
				case WM_KEYUP:
				case WM_KEYDOWN: {
					u32 vk_code = (u32)msg.wParam;
					bool is_down = ((msg.lParam & (1 << 31)) == 0);


#define process_button(b, vk)\
case vk: {\
input.buttons[b].changed = is_down && !input.buttons[b].is_down;\
input.buttons[b].is_down = is_down;\
} break; 

					switch (vk_code) {
						printf("button pressed");
						process_button(BUTTON_UP, VK_UP);
						process_button(BUTTON_DOWN, VK_DOWN);
						process_button(BUTTON_W, 'W');
						process_button(BUTTON_S, 'S');
						process_button(BUTTON_LEFT, VK_LEFT);
						process_button(BUTTON_RIGHT, VK_RIGHT);
						process_button(BUTTON_ENTER, VK_RETURN); 
					}

				} break; 

				default: {
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
		}


		//simulate
		simulate_game(&input, delta_time); 
		//render
		StretchDIBits(hdc, 0, 0, renderState.width, renderState.height, 0, 0, renderState.width, renderState.height, renderState.memory, &renderState.bitmapinfo, DIB_RGB_COLORS, SRCCOPY);
		
		LARGE_INTEGER frame_end_time; 
		QueryPerformanceCounter(&frame_end_time); 
		delta_time = (float)(frame_end_time.QuadPart - frame_begin_time.QuadPart) / performance_frequency; 
		frame_begin_time = frame_end_time; 
	}
	return 0;
} 