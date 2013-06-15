#include "utility.h"

namespace utility
{
	D2D1_RECT_F CreateUnitRectangle(int x_offset, int y_offset, int x, int y )
	{
		D2D1_RECT_F rectangle = D2D1::RectF(
			x_offset + (GAME_UNIT_SIZE * x),
			y_offset + (GAME_UNIT_SIZE * y),
			x_offset + (GAME_UNIT_SIZE * x) + GAME_UNIT_SIZE,
			y_offset + (GAME_UNIT_SIZE * y) + GAME_UNIT_SIZE
			);
		return rectangle;
	}
}