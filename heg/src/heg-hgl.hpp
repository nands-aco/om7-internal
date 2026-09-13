#include <cstdint>

namespace heg
{
	// １ピクセルのフォーマットを表す構造体です。
	// ※プラットフォームごとに定義を調整する可能性があります。
	struct HegGraphPixel
	{
		const std::uint16_t A : 4;
		const std::uint16_t B : 4;
		const std::uint16_t G : 4;
		const std::uint16_t R : 4;
		HegGraphPixel() = default;
		HegGraphPixel(const HegGraphPixel&) = default;
		constexpr HegGraphPixel(std::uint8_t r, std::uint8_t g, std::uint8_t b) noexcept : HegGraphPixel(r, g, b, 0xFF) {}
		constexpr HegGraphPixel(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a) noexcept : R(r >> 4), G(g >> 4), B(b >> 4), A(a >> 4) {}
		constexpr operator std::uint16_t() const noexcept { return *reinterpret_cast<const std::uint16_t*>(this); }
	};
	static_assert(sizeof(HegGraphPixel) == sizeof(std::uint16_t));
}
