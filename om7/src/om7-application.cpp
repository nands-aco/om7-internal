//
// GLFWに依存する処理は、このソースコードに限定する方針です。
// GLFWでハンドリングしたイベントをOm7Applicationに通知するため、
// Om7Applicationのインスタンスを引数として受け取るrun関数を定義します。
//
#include <cstdint>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <algorithm>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "om7-application.hpp"

namespace
{
	// タイトルバーのテキストを表します。
	constexpr const char *Title = "OverMode7";
	// 1秒間に対するフレーム数を表します。
	constexpr std::int32_t FramesPerSecond = 60;
	// 1フレームの時間間隔(ナノ秒)表します。
	constexpr std::chrono::nanoseconds FrameInterval{1'000'000'000 / FramesPerSecond};
	// スキップを許容するフレーム数を表します。
	constexpr std::int32_t MaxRenderSkipFrames = 3;

	void OnError(int errorCode, const char *description)
	{
		std::cerr << "[GLFW Error] code=" << errorCode  << " description=" << (description ? description : "(null)")  << std::endl;
	}
	void OnWindowPos(GLFWwindow * /*window*/, int /*xpos*/, int /*ypos*/) {}
	void OnWindowSize(GLFWwindow * /*window*/, int /*width*/, int /*height*/) {}
	void OnWindowClose(GLFWwindow * /*window*/) {}
	void OnWindowRefresh(GLFWwindow * /*window*/) {}
	void OnWindowFocus(GLFWwindow * /*window*/, int /*focused*/) {}
	void OnWindowIconify(GLFWwindow * /*window*/, int /*iconified*/) {}
	void OnWindowMaximize(GLFWwindow * /*window*/, int /*maximized*/) {}
	void OnFramebufferSize(GLFWwindow * /*window*/, int width, int height)
	{
		glViewport(0, 0, width, height);
	}
	void OnWindowContentScale(GLFWwindow * /*window*/, float /*xscale*/, float /*yscale*/) {}
	void OnMouseButton(GLFWwindow * /*window*/, int /*button*/, int /*action*/, int /*mods*/) {}
	void OnCursorPos(GLFWwindow * /*window*/, double /*xpos*/, double /*ypos*/) {}
	void OnCursorEnter(GLFWwindow * /*window*/, int /*entered*/) {}
	void OnScroll(GLFWwindow * /*window*/, double /*xoffset*/, double /*yoffset*/) {}
	void OnKey(GLFWwindow *window, int key, int /*scancode*/, int action, int /*mods*/)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}
	}
	void OnChar(GLFWwindow * /*window*/, unsigned int /*codepoint*/) {}
	void OnCharMods(GLFWwindow * /*window*/, unsigned int /*codepoint*/, int /*mods*/) {}
	void OnDrop(GLFWwindow * /*window*/, int /*pathCount*/, const char *[] /*paths*/) {}

	void registerEventStubs(GLFWwindow *window)
	{
		glfwSetWindowPosCallback(window, OnWindowPos);
		glfwSetWindowSizeCallback(window, OnWindowSize);
		glfwSetWindowCloseCallback(window, OnWindowClose);
		glfwSetWindowRefreshCallback(window, OnWindowRefresh);
		glfwSetWindowFocusCallback(window, OnWindowFocus);
		glfwSetWindowIconifyCallback(window, OnWindowIconify);
		glfwSetWindowMaximizeCallback(window, OnWindowMaximize);
		glfwSetFramebufferSizeCallback(window, OnFramebufferSize);
		glfwSetWindowContentScaleCallback(window, OnWindowContentScale);

		glfwSetMouseButtonCallback(window, OnMouseButton);
		glfwSetCursorPosCallback(window, OnCursorPos);
		glfwSetCursorEnterCallback(window, OnCursorEnter);
		glfwSetScrollCallback(window, OnScroll);

		glfwSetKeyCallback(window, OnKey);
		glfwSetCharCallback(window, OnChar);
#if GLFW_VERSION_MAJOR >= 3 && GLFW_VERSION_MINOR >= 3
		glfwSetCharModsCallback(window, OnCharMods);
#endif
		glfwSetDropCallback(window, OnDrop);
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

namespace om7
{
	Om7Application::Om7Application()
	{
		glfwSetErrorCallback(OnError);
		if (glfwInit() != GLFW_TRUE) throw Om7RuntimeException("glfwInit failed.");

		int width = 240;
		int height = 240;

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	#if defined(__APPLE__)
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	#endif

		if (width == 0 && height == 0)
		{
			GLFWmonitor *monitor = glfwGetPrimaryMonitor();
			if (!monitor) throw Om7RuntimeException("Primary monitor is not available.");
			const GLFWvidmode *mode = glfwGetVideoMode(monitor);
			if (!mode) throw Om7RuntimeException("Failed to get video mode.");
			Window = glfwCreateWindow(mode->width, mode->height, Title, monitor, nullptr);
		}
		else
		{
			Window = glfwCreateWindow(width, height, Title, nullptr, nullptr);
		}
		if (!Window) throw Om7RuntimeException("glfwCreateWindow failed.");
		glfwMakeContextCurrent(Window);
		glfwSwapInterval(0); // 自前60FPS制御のためVSync無効
		if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == 0) throw Om7RuntimeException("Failed to initialize GLAD.");
		registerEventStubs(Window);
	}

	Om7Application::~Om7Application()
	{
		glfwDestroyWindow(Window);
		glfwTerminate();
	}

	void Om7Application::Init()
	{
	}

	void Om7Application::Term()
	{
	}

	void Om7Application::Run()
	{
		OnInit();
		// int fbWidth = 0;
		// int fbHeight = 0;
		// glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
		// glViewport(0, 0, fbWidth, fbHeight);

		using Clock = std::chrono::steady_clock;
		auto previous = Clock::now();
		auto accumulator = std::chrono::nanoseconds::zero();

		while (glfwWindowShouldClose(Window) == GLFW_FALSE)
		{
			const auto now = Clock::now();
			auto delta = std::chrono::duration_cast<std::chrono::nanoseconds>(now - previous);
			previous = now;

			// 巨大な一時停止復帰時に暴走しないよう上限を設定
			delta = std::min(delta, FrameInterval * 8);
			accumulator += delta;

			glfwPollEvents();

			// 入力・更新は60FPS固定
			int updateCount = 0;
			while (accumulator >= FrameInterval)
			{
				updateFixedStep();
				accumulator -= FrameInterval;
				++updateCount;

				// 追いつかない場合は描画スキップ許容を超えないよう更新回数を制限
				if (updateCount >= (MaxRenderSkipFrames + 1))
				{
					break;
				}
			}

			renderFrame();
			glfwSwapBuffers(Window);

			// 次フレーム開始まで待機（CPU使用率抑制）
			const auto frameEnd = Clock::now();
			const auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(frameEnd - now);
			if (elapsed < FrameInterval)
			{
				std::this_thread::sleep_for(FrameInterval - elapsed);
			}
		}
		OnTerm();
	}
} // namespace om7
