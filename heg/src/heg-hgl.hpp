#include <cstdint>

namespace heg
{
	// １ピクセルのフォーマットを表す構造体です。
	// ※プラットフォームごとに定義を調整する可能性があります。
	struct HegGraphPixel
	{
		HegGraphPixel() : R(0), G(0), B(0), A(15) {}
		HegGraphPixel(std::uint16_t r, std::uint16_t g, std::uint16_t b) : R(r), G(g), B(b), A(15) {}
		HegGraphPixel(std::uint16_t r, std::uint16_t g, std::uint16_t b, std::uint16_t a) : R(r), G(g), B(b), A(a) {}
		std::uint16_t A : 4;
		std::uint16_t B : 4;
		std::uint16_t G : 4;
		std::uint16_t R : 4;
	};
	static_assert(sizeof(HegGraphPixel) == 2);
}
