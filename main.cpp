#include <Windows.h>
#include <Xinput.h>
#include <sstream>

#pragma comment(lib, "Xinput.lib")

const wchar_t* className = L"X360Man";

int WinMain(HINSTANCE hInst, HINSTANCE n, LPSTR lpCmdLine, int nCmdShow)
{
	HINSTANCE xInput = LoadLibraryA("XInput1_4.dll");

	int result = MessageBox(0, "Para desligar o controle, aperte \"Sim\"\nPara ver o nível de Bateria, aperte \"Não\".", "Bem-vindo '-'", MB_YESNO | MB_ICONINFORMATION);

	if (xInput == NULL)
	{
		MessageBox(0, "Falha ao carregar dll XInput1_4. Certifique-se de que você a tenha instalada.", "ERRO:", MB_ICONERROR | MB_OK);
		return EXIT_FAILURE;
	}

	if (result == IDNO)
	{
		XINPUT_BATTERY_INFORMATION bInfo;
		memset(&bInfo, 0, sizeof(bInfo));
		DWORD res = XInputGetBatteryInformation(0, BATTERY_DEVTYPE_GAMEPAD, &bInfo);

		if (res != ERROR_SUCCESS || bInfo.BatteryType == BATTERY_TYPE_DISCONNECTED)
		{
			MessageBox(0, "Falha ao reportar nível de Bateria. Certifique-se de que seu controle esteja conectado ou que seja um controle de Xbox360.\n", "ERRO", MB_ICONERROR | MB_OK);
			FreeLibrary(xInput);
			return EXIT_FAILURE;
		}

		std::stringstream msgBox;
		if (bInfo.BatteryType == BATTERY_TYPE_NIMH)
		{
			msgBox << "Nível de Bateria: " << (int)bInfo.BatteryLevel;
			switch (bInfo.BatteryLevel)
			{
			case BATTERY_LEVEL_FULL:
				msgBox << "\nBateria cheia!";
				break;
			case BATTERY_LEVEL_MEDIUM:
				msgBox << "\nBateria em nível médio!";
				break;
			case BATTERY_LEVEL_LOW:
				msgBox << "\nBateria baixa!";
				break;
			case BATTERY_LEVEL_EMPTY:
				msgBox << "\nBateria vazia!";
				break;
			}
		}
		if (bInfo.BatteryType == BATTERY_TYPE_WIRED)
		{
			msgBox << "Controle conectado por fio.";
		}
		if (bInfo.BatteryType == BATTERY_TYPE_ALKALINE)
		{
			msgBox << "Nível de Pilha: " << (int)bInfo.BatteryLevel;
			switch (bInfo.BatteryLevel)
			{
			case BATTERY_LEVEL_FULL:
				msgBox << "\nPilha cheia!";
				break;
			case BATTERY_LEVEL_MEDIUM:
				msgBox << "\nPilha em nível médio!";
				break;
			case BATTERY_LEVEL_LOW:
				msgBox << "\nPilha baixa!";
				break;
			case BATTERY_LEVEL_EMPTY:
				msgBox << "\nPilha vazia!";
				break;
			}
		}
		MessageBoxA(0, msgBox.str().c_str(), "STATUS BATERIA", MB_OK);
	}
	else
	{
		for (UINT8 i = 0; i < 4; i++)
		{
			XINPUT_STATE state;
			memset(&state, 0, sizeof(state));
			if (XInputGetState(i, &state) == ERROR_SUCCESS)
			{
				typedef DWORD(WINAPI* TurnOffController)(DWORD i);
				TurnOffController func = (TurnOffController)GetProcAddress(xInput, (LPCSTR)103);
				if (func(i) == ERROR_SUCCESS)
				{
					MessageBox(0, "Controle desligado com sucesso! =)", "SUCESSO", MB_OK);
				}
			}
		}
	}
	FreeLibrary(xInput);

	return EXIT_SUCCESS;
}