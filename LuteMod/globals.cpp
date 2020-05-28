#include "pch.h"
#include "globals.h"
#include "SDK/SDK.hpp"

#define FNameCompareFn(name, fnames) bool name(Classes::FName other) { for (auto fn : fnames) { if (other == fn) return true; } return false; }

namespace G
{
	SDK::UWorld* UWorld;
	SDK::UWorld* PWorld;
	SDK::APlayerController* PlayerController;

}