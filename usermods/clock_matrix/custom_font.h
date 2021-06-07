// Custom font for numerical characters only
#include "font.h"

static const unsigned char __numbers_font_bitmap__[] = {
// 0
	XXX_____,
	X_X_____,
	X_X_____,
	X_X_____,
	XXX_____,
	________,
// 1
	_X______,
	_X______,
	_X______,
	_X______,
	_X______,
	________,
//  2
	XXX_____,
	__X_____,
	XXX_____,
	X_______,
	XXX_____,
	________,
//  3
	XXX_____,
	__X_____,
	_XX_____,
	__X_____,
	XXX_____,
	________,
//  4
	X_X_____,
	X_X_____,
	XXX_____,
	__X_____,
	__X_____,
	________,
//  5
	XXX_____,
	X_______,
	XXX_____,
	__X_____,
	XXX_____,
	________,
//  6
	XXX_____,
	X_______,
	XXX_____,
	X_X_____,
	XXX_____,
	________,
//  7
	XXX_____,
	__X_____,
	__X_____,
	__X_____,
	__X_____,
	________,
//  8
	XXX_____,
	X_X_____,
	XXX_____,
	X_X_____,
	XXX_____,
	________,
//  9
	XXX_____,
	X_X_____,
	XXX_____,
	__X_____,
	XXX_____,
	________,
};


static const unsigned char __numbers_font_widths__[] = {
	4,
    4,
	4,
	4,
	4,
	4,
	4,
	4,
	4,
	4,
};

static const unsigned short __numbers_font_index__[] = {
    0,
    1,
    2,
    3,
    4,
    5,
    6,
    7,
    8,
    9
};

	/// bitmap font structure
const struct bitmap_font numbers_font = {
	.Width = 3, .Height = 6,
	.Chars = 10,
	.Widths = __numbers_font_widths__,
	.Index = __numbers_font_index__,
	.Bitmap = __numbers_font_bitmap__,
};
