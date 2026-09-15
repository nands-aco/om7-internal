#pragma once
#include "heg-hgl.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <utility>

namespace heg::hgl
{
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
