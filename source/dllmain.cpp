#include "dllmain.hpp"
#include <Windows.h>
#include <detours.h>

static auto SystemParametersInfoA_Original = SystemParametersInfoA;
static auto SystemParametersInfoW_Original = SystemParametersInfoW;

template<bool IsUnicode>
BOOL WINAPI SystemParametersInfo_Hook(
	UINT uiAction,
	UINT uiParam,
	PVOID pvParam,
	UINT fWinIni
) {
	// Don't touch my mouse configuration.
	if (uiAction == SPI_SETMOUSE)
		return TRUE;

	return [] {
		if constexpr (IsUnicode)
			return SystemParametersInfoW_Original;
		else
			return SystemParametersInfoA_Original;
	}()(uiAction, uiParam, pvParam, fWinIni);
}

BOOL WINAPI DllMain(HINSTANCE, DWORD dwReason, LPVOID) {
	if (DetourIsHelperProcess())
		return TRUE;

	switch (dwReason) {
	case DLL_PROCESS_ATTACH:
		on_dll_attach();
		break;
	case DLL_PROCESS_DETACH:
		on_dll_detach();
		break;
	}

	return TRUE;
}

void on_dll_attach() {
	DetourRestoreAfterWith();

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	attach(SystemParametersInfoA_Original, SystemParametersInfo_Hook<false>);
	attach(SystemParametersInfoW_Original, SystemParametersInfo_Hook<true>);

	DetourTransactionCommit();
}

void on_dll_detach() {
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	detach(SystemParametersInfoA_Original, SystemParametersInfo_Hook<false>);
	detach(SystemParametersInfoW_Original, SystemParametersInfo_Hook<true>);

	DetourTransactionCommit();
}
