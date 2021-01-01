#include <Windows.h>

#include <optional>
#include <string>

std::optional<DEVMODE> setScreenResolution(int width, int height)
{
	DEVMODE prevMode;
	DEVMODE devMode;

	ZeroMemory(&devMode, sizeof(DEVMODE));
	devMode.dmSize = sizeof(DEVMODE);

	if (!EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devMode))
		return std::nullopt;

	CopyMemory(&prevMode, &devMode, sizeof(DEVMODE));
	devMode.dmPelsWidth = width;
	devMode.dmPelsHeight = height;

	if (ChangeDisplaySettingsEx(NULL, &devMode, NULL, 0, NULL) == DISP_CHANGE_SUCCESSFUL)
		return prevMode;
	else
		return std::nullopt;
}

bool restoreScreenResolution(DEVMODE& mode)
{
	return ChangeDisplaySettingsEx(NULL, &mode, NULL, 0, NULL) == DISP_CHANGE_SUCCESSFUL;
}

bool executeProgram(std::string command)
{
	STARTUPINFO si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);

	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(pi));

	if (CreateProcess(NULL, command.data(), NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
	{
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		return true;
	}
	else
		return false;
}

void showError(const char* message)
{
	MessageBox(NULL, message, "Error", MB_OK);
}

void showUsage()
{
	showError(
	    "Usage:\n"
	    "screen-resolution-changer.exe <width> <height> <executable> [args...]");
}

int main(int argc, char* argv[])
{
	if (argc < 4)
	{
		showUsage();
		return 1;
	}

	unsigned int width = 0;
	unsigned int height = 0;

	try
	{
		width = std::stoi(argv[1]);
		height = std::stoi(argv[2]);
	}
	catch (const std::exception& e)
	{
		showUsage();
		return 1;
	}

	std::string command = argv[3];
	for (int i = 4; i < argc; i++)
		command.append(" ").append(argv[i]);

	auto prevMode = setScreenResolution(width, height);
	if (prevMode)
	{
		int result = 1;
		if (executeProgram(command))
			result = 0;
		else
		{
			result = 1;
			showError("Failed to start application");
		}

		if (restoreScreenResolution(*prevMode))
			return result;
		else
		{
			showError("Failed to restore screen resolution");
			return 1;
		}
	}
	else
	{
		showError("Failed to change screen resolution");
		return 1;
	}
}
