#include "Font.h"
#include "DxLib.h"

int Font::fonts[3];

void Font::createFont() {
	fonts[0] = CreateFontToHandle( NULL, 50, 3, DX_FONTTYPE_ANTIALIASING_EDGE );
	fonts[1] = CreateFontToHandle( NULL, 20, 3, DX_FONTTYPE_ANTIALIASING_EDGE );
	fonts[2] = CreateFontToHandle( NULL, 36, 3, DX_FONTTYPE_ANTIALIASING_EDGE );
}