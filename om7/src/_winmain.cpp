extern int main(int argc, char* argv[]);

#if defined(_WIN32)
extern "C"
{
#include <windows.h>
#include <corecrt_startup.h>
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int argc = *__p___argc();
	char** argv = *__p___argv();
	return main(__argc, __argv);
}
#endif
