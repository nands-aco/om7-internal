#include "heg.hpp"
#include <array>
#include <cmath>
#include <cstdint>
#include <vector>

namespace heg
{
	namespace
	{
		// SIN関数 ※C標準のSIN関数を使っても全く問題ないけど…。
		constexpr double HegSin(double v) noexcept
		{
			// while (v >  HegMath::Pi) v -= HegMath::Pi * 2.0; while (v < -HegMath::Pi) v += HegMath::Pi * 2.0;
			// double v2 = v * v; double t = v; double r = v;
			// t *= -v2 / ( 2 *  3); r += t; t *= -v2 / ( 4 *  5); r += t; t *= -v2 / ( 6 *  7); r += t;
			// t *= -v2 / ( 8 *  9); r += t; t *= -v2 / (10 * 11); r += t; t *= -v2 / (12 * 13); r += t;
			// return r;
			return std::sin(v);
		}
	}
	constexpr std::array<std::int32_t, HegMath::SinTableLength> HegMath::InitSinTable() noexcept
	{
		std::array<std::int32_t, SinTableLength> table;
		for (std::uint32_t i = 0; i < SinTableLength; ++i)
		{
			table[i] = (std::int32_t)(HegSin(2.0 * Pi * i / table.size()) * HegFixed::Scale);
		}
		return table;
	}
	std::uint32_t HegMath::InitRandSeed() noexcept
	{
		std::uint32_t seed = 0;
		while (seed == 0)
		{
			seed = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		}
		return seed;
	}
	std::array<std::int32_t, HegMath::SinTableLength> HegMath::SinTable = InitSinTable();
	std::uint32_t HegMath::RandSeed = InitRandSeed();
}
