#include <cstdint>
#include <string>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
// #include "heg/heg.hpp"

// //
// //
// //
// inline bool MixGamepadState(heg::HegGamepad& gamepad, const heg::HegGamepad& state)
// {
// 	if (state.ButtonA || state.ButtonB || state.ButtonX || state.ButtonY || state.ButtonL || state.ButtonR || state.HatL || state.HatR || state.HatU || state.HatD || state.SystemV || state.SystemM)
// 	{
// 		gamepad.ButtonA = state.ButtonA ? gamepad.ButtonA + 1 : 0;
// 		gamepad.ButtonB = state.ButtonB ? gamepad.ButtonB + 1 : 0;
// 		gamepad.ButtonX = state.ButtonX ? gamepad.ButtonX + 1 : 0;
// 		gamepad.ButtonY = state.ButtonY ? gamepad.ButtonY + 1 : 0;
// 		gamepad.ButtonL = state.ButtonL ? gamepad.ButtonL + 1 : 0;
// 		gamepad.ButtonR = state.ButtonR ? gamepad.ButtonR + 1 : 0;
// 		gamepad.HatL    = state.HatL    ? gamepad.HatL    + 1 : 0;
// 		gamepad.HatR    = state.HatR    ? gamepad.HatR    + 1 : 0;
// 		gamepad.HatU    = state.HatU    ? gamepad.HatU    + 1 : 0;
// 		gamepad.HatD    = state.HatD    ? gamepad.HatD    + 1 : 0;
// 		gamepad.SystemV = state.SystemV ? gamepad.SystemV + 1 : 0;
// 		gamepad.SystemM = state.SystemM ? gamepad.SystemM + 1 : 0;
// 		return true;
// 	}
// 	else
// 	{
// 		return false;
// 	}
// }
// //
// //
// //
// void UpdateGamepad(heg::HegGamepad& gamepad, GLFWwindow* window) noexcept
// {
// 	heg::HegGamepad nowGamepad = {};
// 	nowGamepad.ButtonA = glfwGetKey(window, GLFW_KEY_LEFT ) ? 1 : 0;
// 	nowGamepad.ButtonB = glfwGetKey(window, GLFW_KEY_DOWN ) ? 1 : 0;
// 	nowGamepad.ButtonX = glfwGetKey(window, GLFW_KEY_UP   ) ? 1 : 0;
// 	nowGamepad.ButtonY = glfwGetKey(window, GLFW_KEY_RIGHT) ? 1 : 0;
// 	nowGamepad.ButtonL = glfwGetKey(window, GLFW_KEY_Q) ? 1 : 0;
// 	nowGamepad.ButtonR = glfwGetKey(window, GLFW_KEY_E) ? 1 : 0;
// 	nowGamepad.HatL = glfwGetKey(window, GLFW_KEY_A) ? 1 : 0;
// 	nowGamepad.HatR = glfwGetKey(window, GLFW_KEY_D) ? 1 : 0;
// 	nowGamepad.HatU = glfwGetKey(window, GLFW_KEY_W) ? 1 : 0;
// 	nowGamepad.HatD = glfwGetKey(window, GLFW_KEY_S) ? 1 : 0;
// 	nowGamepad.SystemV = glfwGetKey(window, GLFW_KEY_ESCAPE) || glfwGetKey(window, GLFW_KEY_TAB  ) ? 1 : 0;
// 	nowGamepad.SystemM = glfwGetKey(window, GLFW_KEY_ENTER ) || glfwGetKey(window, GLFW_KEY_SPACE) ? 1 : 0;
// 	if (MixGamepadState(gamepad, nowGamepad))
// 	{
// 		//
// 		// キーボードに入力があった場合…
// 		//
// 	}
// 	else
// 	{
// 		//
// 		// キーボードに入力がなかった場合…
// 		//
// 		bool flag = false;
// 		for (std::uint32_t i = GLFW_JOYSTICK_1; i <= GLFW_JOYSTICK_16; ++i)
// 		{
// 			GLFWgamepadstate state;
// 			if (glfwGetGamepadState(i, &state))
// 			{
// 				nowGamepad.ButtonA = state.buttons[GLFW_GAMEPAD_BUTTON_A] ? 1 : 0;
// 				nowGamepad.ButtonB = state.buttons[GLFW_GAMEPAD_BUTTON_B] ? 1 : 0;
// 				nowGamepad.ButtonX = state.buttons[GLFW_GAMEPAD_BUTTON_X] ? 1 : 0;
// 				nowGamepad.ButtonY = state.buttons[GLFW_GAMEPAD_BUTTON_Y] ? 1 : 0;
// 				nowGamepad.ButtonL = state.buttons[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER ] ? 1 : 0;
// 				nowGamepad.ButtonR = state.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER] ? 1 : 0;
// 				nowGamepad.HatL = state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_LEFT ] ? 1 : 0;
// 				nowGamepad.HatR = state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT] ? 1 : 0;
// 				nowGamepad.HatU = state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_UP   ] ? 1 : 0;
// 				nowGamepad.HatD = state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_DOWN ] ? 1 : 0;
// 				nowGamepad.SystemV = state.buttons[GLFW_GAMEPAD_BUTTON_BACK ] ? 1 : 0;
// 				nowGamepad.SystemM = state.buttons[GLFW_GAMEPAD_BUTTON_START] ? 1 : 0;
// 				if (MixGamepadState(gamepad, nowGamepad))
// 				{
// 					//
// 					// いずれかのゲームパッドに全く入力があった場合…
// 					//
// 					flag = true;
// 					break;
// 				}
// 			}
// 		}
// 		if (!flag)
// 		{
// 			//
// 			// ゲームパッドに全く入力がなかった場合…
// 			//
// 			gamepad = {};
// 		}
// 	}
// 	// nowGamepad.MouseL = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT  ) ? 1 : 0;
// 	// nowGamepad.MouseM = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) ? 1 : 0;
// 	// nowGamepad.MouseR = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT ) ? 1 : 0;
// 	// if (nowGamepad.MouseL || nowGamepad.MouseM || nowGamepad.MouseR)
// 	// {
// 	// 	gamepad->MouseL += nowGamepad.MouseL;
// 	// 	gamepad->MouseM += nowGamepad.MouseM;
// 	// 	gamepad->MouseR += nowGamepad.MouseR;
// 	// }
// 	// double x, y;
// 	// glfwGetCursorPos(window, &x, &y);
// 	// gamepad->MouseX = static_cast<std::int32_t>(x);
// 	// gamepad->MouseY = static_cast<std::int32_t>(y);
// }
//
//
//
struct SzaMainParams
{
	bool Fullscreen;
};
//
//
//
int SzaMain(const SzaMainParams &params)
{
	const char* title = "HEGURDELNO";
	const std::int32_t screenWidth = 240;
	const std::int32_t screenHeight = 240;
	//
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// 可能な限り低ビット深度のデフォルトフレームバッファを避けます。
	// glfwWindowHint(GLFW_RED_BITS, 8);
	// glfwWindowHint(GLFW_GREEN_BITS, 8);
	// glfwWindowHint(GLFW_BLUE_BITS, 8);
	// glfwWindowHint(GLFW_ALPHA_BITS, 8);
	//
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	int width = mode->width;
	int height = mode->height;
	GLFWwindow* window;
	if (params.Fullscreen)
	{
		glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
		glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
		window = glfwCreateWindow(width, height, title, monitor, nullptr);
		glfwSetWindowPos(window, 0, 0);
		//glfwSetWindowSizeLimits(window, width, height, GLFW_DONT_CARE, GLFW_DONT_CARE);
		//glfwSetWindowAspectRatio(window, width, height);
	}
	else
	{
		width = height = (width >= height) ? (height / screenHeight) * screenHeight : (width / screenWidth) * screenWidth;
		window = glfwCreateWindow(width, height, title, nullptr, nullptr);
		glfwSetWindowUserPointer(window, nullptr);
		glfwSetWindowSizeLimits(window, screenWidth, screenHeight, GLFW_DONT_CARE, GLFW_DONT_CARE);
		glfwSetWindowAspectRatio(window, screenWidth, screenHeight);
	}
	glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height) { });
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) { });
	glfwMakeContextCurrent(window);
	//glfwSwapInterval(1);
	gladLoadGL((GLADloadfunc)glfwGetProcAddress);
	//
	//
	//
	const float fraTime = 1.0 / 60.0;
	float delTime = glfwGetTime() + fraTime;
	int drop = 0;
	bool dropFlag = false;
	while (!glfwWindowShouldClose(window))
	{
		drop = delTime <= glfwGetTime() ? drop + 1 : 0;
		while (delTime >= glfwGetTime()) glfwPollEvents();
		delTime = glfwGetTime() + fraTime;
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	//
	//
	//
	glfwDestroyWindow(window);
	glfwTerminate();
	//
	//
	//
	return 0;
}
//
//
//
int main(int argc, char* argv[])
{
	SzaMainParams params;
	params.Fullscreen = false;
	return SzaMain(params);
}
