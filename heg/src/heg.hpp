#pragma once
#include <array>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <thread>
// まだ、stacktraceは実験段階のプラットフォームがあるため保留…。
// 特にWSLを利用した環境では、色々やらないとビルドできない。
// #include <stacktrace>
// #include <source_location>

//
// C言語のrestrictがC++では標準化されておらず、
// 各コンパイラの拡張となるためマクロを利用します。
//
#if defined(__clang__) || defined(__GNUC__)
#define HEG_RESTRICT __restrict__
#elif defined(_MSC_VER)
#define HEG_RESTRICT __restrict
#else
#define HEG_RESTRICT
#endif
//
// ソースの位置を取得するマクロです。
// HegAbort関数やHegAssert関数と合わせて利用します。
//
#define HEG_DEBUG_LOCATION HegDebugLocation{__FILE__, __func__, __LINE__}

namespace heg
{
	//
	//
	//
	struct HegDebugLocation
	{
		const char* File;
		const char* Function;
		const int Line;
	};
	//
	// 仮実装 （JavaやC#のように、こんなクラス必要ない？）
	//
	class HegRuntimeException : public std::runtime_error
	{
	public:
		HegRuntimeException(const char message[]) : std::runtime_error(message) {}
		HegRuntimeException(const std::string& message) : std::runtime_error(message) {}
		const char* what() const noexcept override { return std::runtime_error::what(); }
	};
	//
	// 仮実装 （JavaやC#のように、こんなクラス必要ない？）
	//
	class HegLogicException : public std::logic_error
	{
	public:
		HegLogicException(const char message[]) : std::logic_error(message) {}
		HegLogicException(const std::string& message) : std::logic_error(message) {}
		const char* what() const noexcept override { return std::logic_error::what(); }
	};
	//
	// 固定小数点演算クラス （Q16.16形式）
	//
	class HegFixed final
	{
	public:
		inline static constexpr std::int32_t FracBits = 16;
		inline static constexpr std::int32_t Scale = static_cast<std::int32_t>(1) << FracBits;
	public:
		// 本当はconstexprで定数化したい値
		static constexpr HegFixed Zero() noexcept { return HegFixed(0); }
		static constexpr HegFixed One() noexcept { return HegFixed(Scale); }
		static constexpr HegFixed Half() noexcept { return HegFixed(Scale >> 1); }
		static constexpr HegFixed Unit() noexcept { return HegFixed(1); }
		static constexpr HegFixed MaxValue() noexcept { return HegFixed(0x7FFFFFFF); }
		static constexpr HegFixed MinValue() noexcept { return HegFixed(0x80000000); }
	private:
		std::int32_t Raw;
		constexpr explicit HegFixed(std::int32_t raw) noexcept : Raw(raw) {}
		constexpr explicit HegFixed(std::int16_t hi, std::uint16_t lo) noexcept : Raw(static_cast<int32_t>((static_cast<std::uint32_t>(hi) << FracBits) | (static_cast<std::uint32_t>(lo) & 0xFFFF))) {}
	public:
		constexpr HegFixed() : Raw(0) {}
		static constexpr HegFixed FromRaw(std::int32_t raw) noexcept { return HegFixed(raw); }
		static constexpr HegFixed FromInt(std::int16_t val) noexcept { return HegFixed(val, 0); }
		static constexpr HegFixed FromLowUint(std::uint16_t val) noexcept { return HegFixed(0, val); }
		static constexpr HegFixed FromFloat(float val) noexcept { return HegFixed(static_cast<std::int32_t>(val * Scale)); }
		static constexpr HegFixed FromDouble(double val) noexcept { return HegFixed(static_cast<std::int32_t>(val * Scale)); }
		constexpr std::int32_t ToRaw() const noexcept { return Raw; }
		constexpr std::int16_t ToInt() const noexcept { return static_cast<std::int16_t>(Raw >> FracBits); }
		constexpr std::uint16_t ToLowUint() const noexcept { return static_cast<std::uint16_t>(Raw); }
		constexpr float ToFloat() const noexcept { return static_cast<float>(Raw) / Scale; }
		constexpr double ToDouble() const noexcept { return static_cast<double>(Raw) / Scale; }
		constexpr HegFixed operator +() const noexcept { return *this; }
		constexpr HegFixed operator -() const noexcept { return HegFixed::FromRaw(-Raw); }
		constexpr HegFixed operator +(HegFixed v) const noexcept { return FromRaw(Raw + v.Raw); }
		constexpr HegFixed operator -(HegFixed v) const noexcept { return FromRaw(Raw - v.Raw); }
		constexpr HegFixed operator *(HegFixed v) const noexcept { return FromRaw(static_cast<std::int32_t>((static_cast<std::int64_t>(Raw) * v.Raw) >> FracBits)); }
		constexpr HegFixed operator /(HegFixed v) const noexcept { return FromRaw(static_cast<std::int32_t>((static_cast<std::int64_t>(Raw) << FracBits) / v.Raw)); }
		constexpr HegFixed operator %(HegFixed v) const noexcept { std::int32_t m = Raw % v.Raw; m += (m < 0) * v.Raw; return FromRaw(m); }
		constexpr HegFixed operator %(std::int16_t v) const noexcept { return *this % FromInt(v); }
		constexpr HegFixed& operator +=(HegFixed v) noexcept { *this = *this + v; return *this; }
		constexpr HegFixed& operator -=(HegFixed v) noexcept { *this = *this - v; return *this; }
		constexpr HegFixed& operator *=(HegFixed v) noexcept { *this = *this * v; return *this; }
		constexpr HegFixed& operator /=(HegFixed v) noexcept { *this = *this / v; return *this; }
		constexpr HegFixed& operator %=(HegFixed v) noexcept { *this = *this % v; return *this; }
		constexpr HegFixed& operator %=(std::int16_t v) noexcept { *this = *this % v; return *this; }
		constexpr bool operator ==(HegFixed v) const noexcept { return Raw == v.Raw; }
		constexpr bool operator !=(HegFixed v) const noexcept { return Raw != v.Raw; }
		constexpr bool operator <(HegFixed v) const noexcept { return Raw < v.Raw; }
		constexpr bool operator <=(HegFixed v) const noexcept { return Raw <= v.Raw; }
		constexpr bool operator >(HegFixed v) const noexcept { return Raw > v.Raw; }
		constexpr bool operator >=(HegFixed v) const noexcept { return Raw >= v.Raw; }
		constexpr HegFixed Min(HegFixed v) const noexcept { return Raw <= v.Raw ? *this : v; }
		constexpr HegFixed Max(HegFixed v) const noexcept { return Raw >= v.Raw ? *this : v; }
		constexpr HegFixed Abs() const noexcept { return Raw < 0 ? FromRaw(-Raw) : *this; }
		constexpr HegFixed Clamp(HegFixed min, HegFixed max) const noexcept { return Clamp(*this, min, max); }
		static constexpr HegFixed Clamp(HegFixed v, HegFixed min, HegFixed max) noexcept { return v < min ? min : v > max ? max : v; }
		static constexpr HegFixed Lerp(HegFixed v1, HegFixed v2, HegFixed t) noexcept { return v1 + (v2 - v1) * t; }
	};
	//
	// 二次元座標処理クラス
	//
	class HegVector final
	{
	public:
		HegFixed X;
		HegFixed Y;
	public:
		constexpr HegVector() noexcept = default;
		constexpr HegVector(const HegFixed x, const HegFixed y) noexcept : X(x), Y(y) {}
	public:
		constexpr HegVector operator+() const noexcept { return *this; }
		constexpr HegVector operator-() const noexcept { return {-X, -Y}; }
		constexpr HegVector operator+(const HegVector& v) const noexcept { return {X + v.X, Y + v.Y}; }
		constexpr HegVector operator-(const HegVector& v) const noexcept { return {X - v.X, Y - v.Y}; }
		constexpr HegVector operator*(HegFixed s) const noexcept { return {X * s, Y * s}; }
		constexpr HegVector operator/(HegFixed s) const noexcept { return {X / s, Y / s}; }
		constexpr HegVector& operator+=(const HegVector& v) noexcept { X += v.X; Y += v.Y; return *this; }
		constexpr HegVector& operator-=(const HegVector& v) noexcept { X -= v.X; Y -= v.Y; return *this; }
		constexpr HegVector& operator*=(HegFixed s) noexcept { X *= s; Y *= s; return *this; }
		constexpr HegVector& operator/=(HegFixed s) noexcept { X /= s; Y /= s; return *this; }
		constexpr bool operator==(const HegVector& v) const noexcept { return X == v.X && Y == v.Y; }
		constexpr bool operator!=(const HegVector& v) const noexcept { return !(*this == v); }
		constexpr HegFixed DotProd(const HegVector& v) const noexcept { return DotProd(*this, v); }
		constexpr std::int32_t DistSqInt(const HegVector& v) const noexcept { return DistSqInt(*this, v); }
		constexpr HegFixed DistSqFixed(const HegVector& v) const noexcept { return DistSqFixed(*this, v); }
		/// @brief 内積を求める。 （ >0: 正面より | <0: 背後より | =0: 真横）
		static constexpr HegFixed DotProd(const HegVector& a, const HegVector& b) noexcept { return a.X * b.X + a.Y * b.Y; }
		/// @brief 二点の距離の二乗を求める。
		static constexpr std::int32_t DistSqInt(const HegVector& a, const HegVector& b) noexcept { const std::int32_t dx = a.X.ToInt() - b.X.ToInt(); const std::int32_t dy = a.Y.ToInt() - b.Y.ToInt(); return dx * dx + dy * dy; }
		/// @brief 二点の距離の二乗を求める。
		static constexpr HegFixed DistSqFixed(const HegVector& a, const HegVector& b) noexcept { const HegFixed dx = a.X - b.X; const HegFixed dy = a.Y - b.Y; return dx * dx + dy * dy; }
		/// @brief 法線を求める。（高速化のために雑な計算で済ませる。）
		static constexpr HegVector Normal(const HegVector& a, const HegVector& b) noexcept { const HegVector vec = b - a; const HegFixed len = vec.X.Abs() + vec.Y.Abs(); return len == HegFixed::Zero() ? HegVector(HegFixed::Zero(), HegFixed::Zero()) : HegVector(vec.X, vec.Y) / len; }
	};
	inline constexpr HegVector operator*(HegFixed s, const HegVector& v) noexcept { return { v.X * s, v.Y * s }; }
	inline constexpr HegVector operator/(HegFixed s, const HegVector& v) noexcept { return { v.X / s, v.Y / s }; }
	//
	// 算術関数クラス
	//
	class HegMath final
	{
	private:
		inline static constexpr std::int32_t SinTableLength = 1024;
		// TODO: できればconstexprにしたいがWindowsのコンパイルが怪しくなる。
		static std::array<std::int32_t, SinTableLength> SinTable;
		static std::uint32_t RandSeed;
	private:
		static constexpr std::array<std::int32_t, SinTableLength> InitSinTable() noexcept;
		static std::uint32_t InitRandSeed() noexcept;
	public:
		inline static constexpr double Pi = 3.1415926535897932384626433832795;
	public:
		//static constexpr std::int32_t Sin(std::uint16_t v) noexcept { return SinTable[v >> 6]; }
		static constexpr std::int32_t Sin(std::uint16_t v) noexcept { return SinTable[static_cast<std::uint16_t>(static_cast<std::uint32_t>(v) * SinTableLength / 65536)]; }
		static constexpr std::int32_t Cos(std::uint16_t v) noexcept { return Sin(v + 65536 / 4); }
		static constexpr HegFixed Sin(HegFixed v) noexcept { return HegFixed::FromRaw(Sin(static_cast<std::uint16_t>(v.ToRaw()))); }
		static constexpr HegFixed Cos(HegFixed v) noexcept { return HegFixed::FromRaw(Cos(static_cast<std::uint16_t>(v.ToRaw()))); }
		static constexpr std::int32_t Mod(std::int32_t a, std::int32_t b) noexcept { return (a % b + b) % b; }
		static constexpr std::uint32_t Seed(std::uint32_t seed) noexcept { return RandSeed = seed ? seed : 1; }
		static std::uint32_t RandUint() noexcept { std::uint32_t x = RandSeed; x ^= x << 13; x ^= x >> 17; x ^= x << 5; RandSeed = x; x ^= x >> 16; x *= 0x7feb352d; x ^= x >> 15; x *= 0x846ca68b; x ^= x >> 16; return x; }
		static HegFixed RandFixed() noexcept { return HegFixed::FromRaw(RandUint()); }
		static std::uint64_t GetEpochTime() noexcept { return std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch().count(); }
	private:
		HegMath() = delete;
	};
	//
	//
	//
	[[noreturn]]
	inline void HegAbort(const std::string& message, const HegDebugLocation& location) noexcept
	{
		std::cerr << "called heg::HegAbort : " << message << std::endl;
		std::cout << "FileName: " << location.File << std::endl;
		std::cout << "FunctionName: " << location.Function << std::endl;
		std::cout << "Line: " << location.Line << std::endl;
		std::abort();
	}
	//
	//
	//
	// std::source_locationが使えるようになるまで保留します。
	// [[noreturn]]
	// inline void HegAbort(const std::string& message = std::string(""), const std::source_location& location = std::source_location::current()) noexcept
	// {
	// 	std::cerr << "called heg::HegAbort : " << message << std::endl;
	// 	std::cout << "FileName: " << location.file_name() << std::endl;
	// 	std::cout << "FunctionName: " << location.function_name() << std::endl;
	// 	std::cout << "Line: " << location.line() << std::endl;
	// 	std::cout << "Column: " << location.column() << std::endl;
	// 	// まだ、stacktraceは実験段階のプラットフォームがあるため保留…。
	// 	// 特にWSLを利用した環境では、色々やらないとビルドできない。
	// 	// std::cerr << std::stacktrace::current() << std::endl;
	// 	std::abort();
	// }
	//
	//
	//
	inline void HegAssert(bool expr, const std::string& message, const HegDebugLocation& location) noexcept
	{
		if (!expr) HegAbort(message, location);
	}
	//
	//
	//
	inline void HegAssert(bool expr, const HegDebugLocation& location) noexcept
	{
		HegAssert(expr, "", location);
	}
	//
	//
	//
	// std::source_locationが使えるようになるまで保留します。
	// inline void HegAssert(bool expr, const std::string& message = std::string(""), const std::source_location& location = std::source_location::current()) noexcept
	// {
	// 	if (!expr) HegAbort(message, location);
	// }
	//
	//
	//
	inline void HegSleep(std::uint32_t nsec) noexcept
	{
		std::this_thread::sleep_for(std::chrono::nanoseconds(nsec));
	}
}
