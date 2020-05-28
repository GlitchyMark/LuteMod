#include <Catch.hpp>

#include "headers/PE/IatHook.hpp"
#include "headers/Tests/TestEffectTracker.hpp"

EffectTracker iatEffectTracker;

typedef DWORD(__stdcall* tGetCurrentThreadId)();
tGetCurrentThreadId oGetCurrentThreadID;

NOINLINE DWORD __stdcall hkGetCurrentThreadId() {
	iatEffectTracker.PeakEffect().trigger();
	return oGetCurrentThreadID();
}

TEST_CASE("Iat Hook Tests", "[IatHook]") {
	SECTION("Verify api thunk is found and hooked") {
		PLH::IatHook hook("kernel32.dll", "GetCurrentThreadId", (char*)&hkGetCurrentThreadId, (uint64_t*)&oGetCurrentThreadID, L"");
		REQUIRE(hook.hook());
		
		iatEffectTracker.PushEffect();
		volatile DWORD thrdId = GetCurrentThreadId();
		thrdId++;
		REQUIRE(iatEffectTracker.PopEffect().didExecute());
		REQUIRE(hook.unHook());
	}

	SECTION("Verify api thunk is found and hooked when module explicitly named") {
		PLH::IatHook hook("kernel32.dll", "GetCurrentThreadId", (char*)&hkGetCurrentThreadId, (uint64_t*)&oGetCurrentThreadID, L"polyhook_2.exe");
		REQUIRE(hook.hook());

		iatEffectTracker.PushEffect();
		volatile DWORD thrdId = GetCurrentThreadId();
		thrdId++;
		REQUIRE(iatEffectTracker.PopEffect().didExecute());
		REQUIRE(hook.unHook());
	}
}