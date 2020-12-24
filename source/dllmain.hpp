#pragma once

#include <Windows.h>
#include <detours.h>

void on_dll_attach();

void on_dll_detach();

template<typename Target, typename Hook>
void attach(Target&& target, Hook&& hook) {
	DetourAttach(reinterpret_cast<PVOID*>(&target), hook);
}

template<typename Target, typename Hook>
void detach(Target&& target, Hook&& hook) {
	DetourDetach(reinterpret_cast<PVOID*>(&target), hook);
}