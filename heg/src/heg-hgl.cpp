#define HEG_HGL_CPP_INCLUDED
#include "heg-hgl.hpp"
#include <algorithm>

namespace heg::hgl
{
	namespace
	{
		constexpr std::uint8_t HegFixedBitmapColorStep(std::size_t n) noexcept
		{
			return static_cast<std::uint8_t>(n == 0 ? 0x00 : 0x37 + (n - 1) * 0x28);
		}
	}

	template<std::size_t W, std::size_t H>
	template<std::size_t I>
	constexpr HegColor HegFixedBitmap<W, H>::InitColor() noexcept
	{
		if constexpr (I < 16)
		{
			switch (I)
			{
			case 0x00: return HegColor(0x00, 0x00, 0x00);
			case 0x01: return HegColor(0x80, 0x00, 0x00);
			case 0x02: return HegColor(0x00, 0x80, 0x00);
			case 0x03: return HegColor(0x80, 0x80, 0x00);
			case 0x04: return HegColor(0x00, 0x00, 0x80);
			case 0x05: return HegColor(0x80, 0x00, 0x80);
			case 0x06: return HegColor(0x00, 0x80, 0x80);
			case 0x07: return HegColor(0xc0, 0xc0, 0xc0);
			case 0x08: return HegColor(0x80, 0x80, 0x80);
			case 0x09: return HegColor(0xff, 0x00, 0x00);
			case 0x0A: return HegColor(0x00, 0xff, 0x00);
			case 0x0B: return HegColor(0xff, 0xff, 0x00);
			case 0x0C: return HegColor(0x00, 0x00, 0xff);
			case 0x0D: return HegColor(0xff, 0x00, 0xff);
			case 0x0E: return HegColor(0x00, 0xff, 0xff);
			default: return HegColor(0xff, 0xff, 0xff);
			}
		}
		else if constexpr (I < 232)
		{
			constexpr std::size_t i = I - 16;
			constexpr std::size_t r = i / 36;
			constexpr std::size_t g = (i / 6) % 6;
			constexpr std::size_t b = i % 6;
			return HegColor(HegFixedBitmapColorStep(r), HegFixedBitmapColorStep(g), HegFixedBitmapColorStep(b));
		}
		else
		{
			constexpr std::uint8_t v = static_cast<std::uint8_t>(8 + (I - 232) * 10);
			return HegColor(v, v, v);
		}
	}

	template<std::size_t W, std::size_t H>
	template<std::size_t... I>
	constexpr std::array<HegColor, 256> HegFixedBitmap<W, H>::InitColors(std::index_sequence<I...>) noexcept
	{
		return { InitColor<I>()... };
	}

	template<std::size_t W, std::size_t H>
	constexpr std::array<HegColor, 256> HegFixedBitmap<W, H>::InitColors() noexcept
	{
		return InitColors(std::make_index_sequence<256>{});
	}

	template<std::size_t W, std::size_t H>
	constexpr HegFixedBitmap<W, H>::HegFixedBitmap() noexcept : Colors(InitColors()), Pixels{}
	{
	}

	template<std::size_t W, std::size_t H>
	constexpr const typename HegFixedBitmap<W, H>::HegIndex* HEG_RESTRICT HegFixedBitmap<W, H>::Data() const noexcept
	{
		return Pixels.data();
	}

	template<std::size_t W, std::size_t H>
	constexpr typename HegFixedBitmap<W, H>::HegIndex* HEG_RESTRICT HegFixedBitmap<W, H>::Data() noexcept
	{
		return Pixels.data();
	}

	template<std::size_t W, std::size_t H>
	constexpr const typename HegFixedBitmap<W, H>::HegIndex* HEG_RESTRICT HegFixedBitmap<W, H>::Row(std::size_t y) const noexcept
	{
		return Data() + y * Width;
	}

	template<std::size_t W, std::size_t H>
	constexpr typename HegFixedBitmap<W, H>::HegIndex* HEG_RESTRICT HegFixedBitmap<W, H>::Row(std::size_t y) noexcept
	{
		return Data() + y * Width;
	}

	template<std::size_t W, std::size_t H>
	constexpr typename HegFixedBitmap<W, H>::HegIndex HegFixedBitmap<W, H>::At(std::size_t x, std::size_t y) const noexcept
	{
		return Row(y)[x];
	}

	template<std::size_t W, std::size_t H>
	constexpr typename HegFixedBitmap<W, H>::HegIndex& HegFixedBitmap<W, H>::At(std::size_t x, std::size_t y) noexcept
	{
		return Row(y)[x];
	}

	template<std::size_t W, std::size_t H>
	constexpr const HegColor& HegFixedBitmap<W, H>::Resolve(std::size_t x, std::size_t y) const noexcept
	{
		return Colors[At(x, y)];
	}

	template<std::size_t W, std::size_t H>
	constexpr void HegFixedBitmap<W, H>::Clear(HegIndex color) noexcept
	{
		std::fill(Pixels.begin(), Pixels.end(), color);
	}
}
