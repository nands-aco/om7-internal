#include <cstdint>

#include <cstdint>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <algorithm>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace
{

	constexpr std::int32_t kTargetFps = 60;
	constexpr std::chrono::nanoseconds kFrameDurationNs{1'000'000'000 / kTargetFps};
	constexpr int kMaxRenderSkipFrames = 3;

	// ---- GLFW Event Stubs -----------------------------------------------------

	void onError(int errorCode, const char *description)
	{
		std::cerr << "[GLFW Error] code=" << errorCode
				  << " description=" << (description ? description : "(null)")
				  << '\n';
	}

	void onWindowPos(GLFWwindow * /*window*/, int /*xpos*/, int /*ypos*/) {}
	void onWindowSize(GLFWwindow * /*window*/, int /*width*/, int /*height*/) {}
	void onWindowClose(GLFWwindow * /*window*/) {}
	void onWindowRefresh(GLFWwindow * /*window*/) {}
	void onWindowFocus(GLFWwindow * /*window*/, int /*focused*/) {}
	void onWindowIconify(GLFWwindow * /*window*/, int /*iconified*/) {}
	void onWindowMaximize(GLFWwindow * /*window*/, int /*maximized*/) {}
	void onFramebufferSize(GLFWwindow * /*window*/, int width, int height)
	{
		glViewport(0, 0, width, height);
	}
	void onWindowContentScale(GLFWwindow * /*window*/, float /*xscale*/, float /*yscale*/) {}
	void onMouseButton(GLFWwindow * /*window*/, int /*button*/, int /*action*/, int /*mods*/) {}
	void onCursorPos(GLFWwindow * /*window*/, double /*xpos*/, double /*ypos*/) {}
	void onCursorEnter(GLFWwindow * /*window*/, int /*entered*/) {}
	void onScroll(GLFWwindow * /*window*/, double /*xoffset*/, double /*yoffset*/) {}
	void onKey(GLFWwindow *window, int key, int /*scancode*/, int action, int /*mods*/)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}
	}
	void onChar(GLFWwindow * /*window*/, unsigned int /*codepoint*/) {}
	void onCharMods(GLFWwindow * /*window*/, unsigned int /*codepoint*/, int /*mods*/) {}
	void onDrop(GLFWwindow * /*window*/, int /*pathCount*/, const char *[] /*paths*/) {}

	void registerEventStubs(GLFWwindow *window)
	{
		glfwSetWindowPosCallback(window, onWindowPos);
		glfwSetWindowSizeCallback(window, onWindowSize);
		glfwSetWindowCloseCallback(window, onWindowClose);
		glfwSetWindowRefreshCallback(window, onWindowRefresh);
		glfwSetWindowFocusCallback(window, onWindowFocus);
		glfwSetWindowIconifyCallback(window, onWindowIconify);
		glfwSetWindowMaximizeCallback(window, onWindowMaximize);
		glfwSetFramebufferSizeCallback(window, onFramebufferSize);
		glfwSetWindowContentScaleCallback(window, onWindowContentScale);

		glfwSetMouseButtonCallback(window, onMouseButton);
		glfwSetCursorPosCallback(window, onCursorPos);
		glfwSetCursorEnterCallback(window, onCursorEnter);
		glfwSetScrollCallback(window, onScroll);

		glfwSetKeyCallback(window, onKey);
		glfwSetCharCallback(window, onChar);
#if GLFW_VERSION_MAJOR >= 3 && GLFW_VERSION_MINOR >= 3
		glfwSetCharModsCallback(window, onCharMods);
#endif
		glfwSetDropCallback(window, onDrop);
	}

	void updateFixedStep()
	{
		// TODO: 60FPS固定で処理したいゲームロジックをここに実装
	}

	void renderFrame()
	{
		// TODO: OpenGL 3.3 Core の描画処理をここに実装
		glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

} // namespace

void run(std::int32_t width, std::int32_t height)
{
	glfwSetErrorCallback(onError);

	if (glfwInit() != GLFW_TRUE)
	{
		std::cerr << "glfwInit failed.\n";
		return;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if defined(__APPLE__)
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

	GLFWmonitor *monitor = nullptr;
	int windowWidth = width;
	int windowHeight = height;

	if (width == 0 && height == 0)
	{
		monitor = glfwGetPrimaryMonitor();
		if (!monitor)
		{
			std::cerr << "Primary monitor is not available.\n";
			glfwTerminate();
			return;
		}

		const GLFWvidmode *mode = glfwGetVideoMode(monitor);
		if (!mode)
		{
			std::cerr << "Failed to get video mode.\n";
			glfwTerminate();
			return;
		}

		windowWidth = mode->width;
		windowHeight = mode->height;
	}

	GLFWwindow *window = glfwCreateWindow(windowWidth, windowHeight, "OverMode7", monitor, nullptr);
	if (!window)
	{
		std::cerr << "glfwCreateWindow failed.\n";
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0); // 自前60FPS制御のためVSync無効

	if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == 0)
	{
		std::cerr << "Failed to initialize GLAD.\n";
		glfwDestroyWindow(window);
		glfwTerminate();
		return;
	}

	registerEventStubs(window);

	int fbWidth = 0;
	int fbHeight = 0;
	glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
	glViewport(0, 0, fbWidth, fbHeight);

	using Clock = std::chrono::steady_clock;
	auto previous = Clock::now();
	auto accumulator = std::chrono::nanoseconds::zero();

	while (glfwWindowShouldClose(window) == GLFW_FALSE)
	{
		const auto now = Clock::now();
		auto delta = std::chrono::duration_cast<std::chrono::nanoseconds>(now - previous);
		previous = now;

		// 巨大な一時停止復帰時に暴走しないよう上限を設定
		delta = std::min(delta, kFrameDurationNs * 8);
		accumulator += delta;

		glfwPollEvents();

		// 入力・更新は60FPS固定
		int updateCount = 0;
		while (accumulator >= kFrameDurationNs)
		{
			updateFixedStep();
			accumulator -= kFrameDurationNs;
			++updateCount;

			// 追いつかない場合は描画スキップ許容を超えないよう更新回数を制限
			if (updateCount >= (kMaxRenderSkipFrames + 1))
			{
				break;
			}
		}

		renderFrame();
		glfwSwapBuffers(window);

		// 次フレーム開始まで待機（CPU使用率抑制）
		const auto frameEnd = Clock::now();
		const auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(frameEnd - now);
		if (elapsed < kFrameDurationNs)
		{
			std::this_thread::sleep_for(kFrameDurationNs - elapsed);
		}
	}

	glfwDestroyWindow(window);
	glfwTerminate();
}

int main()
{
	// (0, 0) 指定でフルスクリーン起動
	run(0, 0);
	return 0;
}
