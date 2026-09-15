#pragma once
#include "heg.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <utility>

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
		template<std::size_t I>
		static constexpr HegColor InitColor() noexcept;
		template<std::size_t... I>
		static constexpr std::array<HegColor, 256> InitColors(std::index_sequence<I...>) noexcept;
		static constexpr std::array<HegColor, 256> InitColors() noexcept;
	public:
		std::array<HegColor, 256> Colors;
		std::array<HegIndex, PixelCount> Pixels;
	public:
		constexpr HegFixedBitmap() noexcept;
		constexpr const HegIndex* HEG_RESTRICT Data() const noexcept;
		constexpr HegIndex* HEG_RESTRICT Data() noexcept;
		constexpr const HegIndex* HEG_RESTRICT Row(std::size_t y) const noexcept;
		constexpr HegIndex* HEG_RESTRICT Row(std::size_t y) noexcept;
		constexpr HegIndex At(std::size_t x, std::size_t y) const noexcept;
		constexpr HegIndex& At(std::size_t x, std::size_t y) noexcept;
		constexpr const HegColor& Resolve(std::size_t x, std::size_t y) const noexcept;
		constexpr void Clear(HegIndex color = 0) noexcept;
	};
}

#ifndef HEG_HGL_CPP_INCLUDED
#include "heg-hgl.cpp"
#endif
