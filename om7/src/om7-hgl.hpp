#include <cstdint>

namespace om7
{
	// 1秒間に対するフレーム数を表します。
	inline constexpr double FramesPerSecond = 60.0;
	// 1フレームの時間間隔(秒)表します。
	inline constexpr double FrameInterval = 1.0 / FramesPerSecond;
	// ドロップを許容するフレーム数を表します。
	inline constexpr std::int32_t MaxDropFrames = 3;
	// 画面の横幅を表します。
	inline constexpr std::int32_t ScreenWidth = 240;
	// 画面の縦幅を表します。
	inline constexpr std::int32_t ScreenHeight = 240;
	// 画面のデータサイズを表します。
	inline constexpr std::int32_t ScreenBufferSize = ScreenWidth * ScreenHeight;
	// 画面のバッファ数を表します。
	inline constexpr std::int32_t ScreenBufferCount = 2;
	// 画面バッファを表します。
	inline std::uint16_t ScreenBuffers[ScreenBufferCount][ScreenBufferSize];
	// 使用中の画面バッファのインデックスを表します。
	inline std::int32_t ScreenBufferIndex = 0;

	extern void GlInit();
	extern void GlTerm();
	extern void GlRender();
}
