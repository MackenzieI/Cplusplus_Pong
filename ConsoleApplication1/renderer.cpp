#include <cctype>

internal void render_background() {
	u32* pixel = (u32*)renderState.memory;
	for (int y = 0; y < renderState.height; y++) {
		for (int x = 0; x < renderState.width; x++) {
			*pixel++ = 0x000000;
		}
	}
}

internal void clear_screen(u32 color) {
	u32* pixel = (u32*)renderState.memory;
	for (int y = 0; y < renderState.height; y++) {
		for (int x = 0; x < renderState.width; x++) {
			*pixel++ = color;
		}
	}
}

internal void 
draw_rect_in_pixels(int x0, int y0, int x1, int y1, u32 color) {

	x0 = clamp(0, x0, renderState.width);
	x1 = clamp(0, x1, renderState.width);
	y0 = clamp(0, y0, renderState.height);
	y1 = clamp(0, y1, renderState.height);

	for (int y = y0; y < y1; y++) {
		u32* pixel = (u32*)renderState.memory + x0 + y * renderState.width;
		for (int x = x0; x < x1; x++) {
			*pixel++ = color;
		}
	}
}

global_variable float renderScale = 0.01f; 

internal void
draw_arena_borders(float arena_x, float arena_y, u32 color) {
	arena_x *= renderState.height * renderScale; 
	arena_y *= renderState.height * renderScale; 

	int x0 = (int)((float)renderState.width * .5f - arena_x);
	int x1 = (int)((float)renderState.width * .5f + arena_x);
	int y0 = (int)((float)renderState.height * .5f - arena_y);
	int y1 = (int)((float)renderState.height * .5f + arena_y);

	draw_rect_in_pixels(0, 0, renderState.width, y0, color);
	draw_rect_in_pixels(0, y1, x1, renderState.height, color);
	draw_rect_in_pixels(0, y0, x0, y1, color);
	draw_rect_in_pixels(x1, y0, renderState.width, renderState.height, color);
}

internal void
draw_rect(float x, float y, float halfsize_x, float halfsize_y, u32 color) {

	x *= renderState.height * renderScale;
	y *= renderState.height * renderScale;
	halfsize_x *= renderState.height * renderScale;
	halfsize_y *= renderState.height * renderScale;
	
	x += renderState.width / 2.f; 
	y += renderState.height / 2.f; 

	// Change to pixels 
	int x0 = x - halfsize_x;
	int x1 = x + halfsize_x;
	int y0 = y - halfsize_y;
	int y1 = y + halfsize_y;

	draw_rect_in_pixels(x0, y0, x1, y1, color);
}

const char* letters[][7] = {
	" 00",
	"0  0",
	"0  0",
	"0000",
	"0  0",
	"0  0",
	"0  0",

	"000",
	"0  0",
	"0  0",
	"000",
	"0  0",
	"0  0",
	"000",

	" 000",
	"0",
	"0",
	"0",
	"0",
	"0",
	" 000",

	"000",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"000",

	"0000",
	"0",
	"0",
	"000",
	"0",
	"0",
	"0000",

	"0000",
	"0",
	"0",
	"000",
	"0",
	"0",
	"0",

	" 000",
	"0",
	"0",
	"0 00",
	"0  0",
	"0  0",
	" 000",

	"0  0",
	"0  0",
	"0  0",
	"0000",
	"0  0",
	"0  0",
	"0  0",

	"000",
	" 0",
	" 0",
	" 0",
	" 0",
	" 0",
	"000",

	" 000",
	"   0",
	"   0",
	"   0",
	"0  0",
	"0  0",
	" 000",

	"0  0",
	"0  0",
	"0 0",
	"00",
	"0 0",
	"0  0",
	"0  0",

	"0",
	"0",
	"0",
	"0",
	"0",
	"0",
	"0000",

	"00 00",
	"0 0 0",
	"0 0 0",
	"0   0",
	"0   0",
	"0   0",
	"0   0",

	"00  0",
	"00  0",
	"0 0 0",
	"0 0 0",
	"0 0 0",
	"0  00",
	"0  00",

	" 00 ",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	" 00 ",

	"000",
	"0  0",
	"0  0",
	"000",
	"0",
	"0",
	"0",

	" 000 ",
	"0   0",
	"0   0",
	"0   0",
	"0 0 0",
	"0  0 ",
	" 00 0",

	"000",
	"0  0",
	"0  0",
	"000",
	"0  0",
	"0  0",
	"0  0",

	" 000",
	"0",
	"0 ",
	" 00",
	"   0",
	"   0",
	"000 ",

	"000",
	" 0",
	" 0",
	" 0",
	" 0",
	" 0",
	" 0",

	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	" 00",

	"0   0",
	"0   0",
	"0   0",
	"0   0",
	"0   0",
	" 0 0",
	"  0",

	"0   0 ",
	"0   0",
	"0   0",
	"0 0 0",
	"0 0 0",
	"0 0 0",
	" 0 0 ",

	"0   0",
	"0   0",
	" 0 0",
	"  0",
	" 0 0",
	"0   0",
	"0   0",

	"0   0",
	"0   0",
	" 0 0",
	"  0",
	"  0",
	"  0",
	"  0",

	"0000",
	"   0",
	"  0",
	" 0",
	"0",
	"0",
	"0000",

	"",
	"",
	"",
	"",
	"",
	" 00",
	" 00",

	"   0",
	"  0",
	"  0",
	" 0",
	" 0",
	"0",
	"0",
};

internal void
draw_text(const char *text, float x, float y, float size, u32 color) {
	float half_size = size * .5f; 
	float original_y = y; 

	while (*text) {
		if (*text != 32) {
			char uptext = std::toupper(*text); // Prevents the compiler from throwing an error, if a lowercase text is inserted

			const char** letter;
			if (*text == 47) letter = letters[27];
			else if (*text == 46) letter = letters[26];
			else letter = letters[uptext - 'A'];
			float original_x = x;

			for (int i = 0; i < 7; i++) {
				const char* row = letter[i];
				while (*row) {
					if (*row == '0') {
						draw_rect(x, y, half_size, half_size, color);
					}
					x += size;
					row++;
				}
				y -= size;
				x = original_x;
			}
		}
		text++; 
		x += size * 6.f; 
		y = original_y; 
	}

}

internal void 
draw_number(int number, float x, float y, float size, u32 color) {
	float half_size = size * .5f; 

	do {
		int digit = number % 10; 
		number = number / 10; 

		switch (digit) {
			case 0: {
				draw_rect(x - size, y, half_size, 2.5f * size, color);
				draw_rect(x + size, y, half_size, 2.5f * size, color);
				draw_rect(x, y + size * 2.f, half_size, half_size, color);
				draw_rect(x, y - size * 2.f, half_size, half_size, color);
				x -= size * 4.f;
			} break;
			case 1: {
				draw_rect(x - size, y, half_size, 2.5f * size, color);
				x -= size * 4.f;
			} break;
			case 2: {
				draw_rect(x, y + size * 2.f, 1.5f * size, half_size, color);
				draw_rect(x, y, 1.5f * size, half_size, color);
				draw_rect(x, y - size * 2.f, 1.5f * size, half_size, color);
				draw_rect(x + size, y + size, half_size, half_size, color);
				draw_rect(x - size, y - size, half_size, half_size, color);
				x -= size * 4.f;
			} break;
			case 3: {
				draw_rect(x - half_size, y + size * 2.f, size, half_size, color);
				draw_rect(x - half_size, y, size, half_size, color);
				draw_rect(x - half_size, y - size * 2.f, size, half_size, color);
				draw_rect(x + size, y, half_size, 2.5f * size, color);
				x -= size * 4.f;
			} break;
			case 4: {
				draw_rect(x + size, y, half_size, 2.5f * size, color);
				draw_rect(x - size, y + size, half_size, 1.5f * size, color);
				draw_rect(x, y, half_size, half_size, color);
				x -= size * 4.f;
			} break;
			case 5: {
				draw_rect(x, y + size * 2.f, 1.5f * size, half_size, color);
				draw_rect(x, y, 1.5f * size, half_size, color);
				draw_rect(x, y - size * 2.f, 1.5f * size, half_size, color);
				draw_rect(x - size, y + size, half_size, half_size, color);
				draw_rect(x + size, y - size, half_size, half_size, color);
				x -= size * 4.f;
			} break;
			case 6: {
				draw_rect(x + half_size, y + size * 2.f, size, half_size, color);
				draw_rect(x + half_size, y, size, half_size, color);
				draw_rect(x + half_size, y - size * 2.f, size, half_size, color);
				draw_rect(x - size, y, half_size, 2.5f * size, color);
				draw_rect(x + size, y - size, half_size, half_size, color);
				x -= size * 4.f;
			} break;
			case 7: {
				draw_rect(x + size, y, half_size, 2.5f * size, color);
				draw_rect(x - half_size, y + size * 2.f, size, half_size, color);
				x -= size * 4.f;
			} break;
			case 8: {
				draw_rect(x - size, y, half_size, 2.5f * size, color);
				draw_rect(x + size, y, half_size, 2.5f * size, color);
				draw_rect(x, y + size * 2.f, half_size, half_size, color);
				draw_rect(x, y - size * 2.f, half_size, half_size, color);
				draw_rect(x, y, half_size, half_size, color);
				x -= size * 4.f;
			} break;
			case 9: {
				draw_rect(x - half_size, y + size * 2.f, size, half_size, color);
				draw_rect(x - half_size, y, size, half_size, color);
				draw_rect(x - half_size, y - size * 2.f, size, half_size, color);
				draw_rect(x + size, y, half_size, 2.5f * size, color);
				draw_rect(x - size, y + size, half_size, half_size, color);
				x -= size * 4.f;
			} break;
		}
	} while (number);
}