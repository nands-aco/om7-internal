#pragma once
#include "heg.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <utility>
#include <algorithm>

namespace heg::hgl
{
	// １ピクセルのフォーマットを表す構造体です。
	// ※プラットフォームごとに定義を調整する可能性があります。
	struct HegColor final
	{
		const std::uint16_t A : 4;
		const std::uint16_t B : 4;
		const std::uint16_t G : 4;
		const std::uint16_t R : 4;
		constexpr HegColor() : HegColor(0, 0, 0, 0xFF) { }
		constexpr HegColor(const HegColor &rgba) noexcept : R(rgba.R), G(rgba.G), B(rgba.B), A(rgba.A) { }
		constexpr HegColor(std::uint8_t r, std::uint8_t g, std::uint8_t b) noexcept : HegColor(r, g, b, 0xFF) { }
		constexpr HegColor(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a) noexcept : R(r >> 4), G(g >> 4), B(b >> 4), A(a >> 4) {}
		constexpr operator std::uint16_t() const noexcept { return *reinterpret_cast<const std::uint16_t*>(this); }
	};
	static_assert(sizeof(HegColor) == sizeof(std::uint16_t));

	template<std::size_t W, std::size_t H>
	class HegFixedBitmap final
	{
		static_assert(W > 0);
		static_assert(H > 0);
	public:
		using HegIndex = std::uint8_t;
		inline static constexpr std::size_t Width = W;
		inline static constexpr std::size_t Height = H;
		inline static constexpr std::size_t PixelCount = W * H;
	private:
		namespace
		{
			constexpr std::uint8_t HegFixedBitmapColorStep(std::size_t n) noexcept
			{
				return static_cast<std::uint8_t>(n == 0 ? 0x00 : 0x37 + (n - 1) * 0x28);
			}
		}

		template<std::size_t I>
		static constexpr HegColor InitColor() noexcept
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

		template<std::size_t... I>
		static constexpr std::array<HegColor, 256> InitColors(std::index_sequence<I...>) noexcept
		{
			return { InitColor<I>()... };
		}

		static constexpr std::array<HegColor, 256> InitColors() noexcept
		{
			return InitColors(std::make_index_sequence<256>{});
		}

	public:
		std::array<HegColor, 256> Colors;
		std::array<HegIndex, PixelCount> Pixels;

	public:
		constexpr HegFixedBitmap() noexcept : Colors(InitColors()), Pixels{}
		{
		}

		constexpr const HegIndex* HEG_RESTRICT Data() const noexcept
		{
			return Pixels.data();
		}

		constexpr HegIndex* HEG_RESTRICT Data() noexcept
		{
			return Pixels.data();
		}

		constexpr const HegIndex* HEG_RESTRICT Row(std::size_t y) const noexcept
		{
			return Data() + y * Width;
		}

		constexpr HegIndex* HEG_RESTRICT Row(std::size_t y) noexcept
		{
			return Data() + y * Width;
		}

		constexpr HegIndex At(std::size_t x, std::size_t y) const noexcept
		{
			return Row(y)[x];
		}

		constexpr HegIndex& At(std::size_t x, std::size_t y) noexcept
		{
			return Row(y)[x];
		}

		constexpr const HegColor& Resolve(std::size_t x, std::size_t y) const noexcept
		{
			return Colors[At(x, y)];
		}

		constexpr void Clear(HegIndex color = 0) noexcept
		{
			std::fill(Pixels.begin(), Pixels.end(), color);
		}
	};
}
