#include "pch.h"
#include "utilities.h"
#include "SDK\SDK.hpp"
//#include "SDK\SDKC.hpp"
#include "SDK\SDK\Mordhau_Basic.cpp"
#include "SDK\SDK\Mordhau_Engine_functions.cpp"
#include "SDK\SDK\Mordhau_CoreUObject_functions.cpp"
#include "SDK\SDK\Mordhau_BP_Lute_functions.cpp"
#include "SDK\SDK\Mordhau_Mordhau_functions.cpp"
#include "SDK\SDK\Mordhau_BP_MordhauCharacter_functions.cpp"
#include <iostream>
#include <Windows.h>
#include <windowsx.h>
#include "stdio.h"
#include <string>
#include <vector>
#include <locale>
#include <algorithm>
#include <chrono>
#include <vector>
#include <sstream>  
#include "HackController.cpp"
#include <intrin.h>
#include <windows.h>
#include <cstdarg>
#include "detours.h"
//#include "syelog.h"
#define _CRT_SECURE_NO_WARNINGS

HANDLE Utilities::curProcess = (HANDLE)0xFFFFFFFFFFFFFFFF;
HINSTANCE Utilities::hAppInstance = 0;
int PlayNote = 0;
bool exiting = 0;
uintptr_t GWorld = 0;
int counting = 0;
bool rungameLoop = false;

using namespace SDK;
using namespace std;

using namespace std::chrono;

typedef void(__thiscall* tProcessEvent)(UObject* a1, class UFunction* a2, void* a3);
void processCommands();
void gameLoop();
tProcessEvent ProcessEvent = (tProcessEvent)0xA64890;

void UnloadModule(HMODULE moduleHandle)
{
	FreeLibraryAndExitThread(moduleHandle, 0);
}
int firsttime = 0;
void __fastcall HookedPE(UObject* a1, class UFunction* a2, void* a3)
{
	//gameLoop();
	//processCommands();
	APlayerController * pCurrentPlayer = (APlayerController*)a1;
	if ((a1->IsA(APlayerController::StaticClass())))
	{
//		printf("Running.\n");
		counting++;
		if (counting > 0)
		{
			counting = 0;
//			rungameLoop = true;
			gameLoop();
			//while (rungameLoop) Sleep(1);

		}
		//while (*rungameLoop);
		//pCurrentPlayer->SendToConsole(L"Hello");
		//firsttime++;
	}
	ProcessEvent(a1, a2, a3);
}
vector<string> commandBuffer;
void processCommands()
{
	vector<string> readBuffer = commandBuffer;
	UWorld* pUWorld = (UWorld*)(*(PDWORD_PTR)((uint64_t)GetModuleHandleA(0) + GWorld));
	AMordhauPlayerController* pCurrentPlayer = (AMordhauPlayerController*)pUWorld->OwningGameInstance->LocalPlayers[0]->PlayerController;

	wstring messagewW;
	wstring cmdW;
	string cmd;
	for (int i = 0; readBuffer.size() > i; i++)
	{
		cmd = readBuffer.at(i);
		cout << "Sending command: " << cmd << endl;
		cmdW = wstring(cmd.begin(), cmd.end());
		if (pCurrentPlayer->Character != nullptr && atoi(cmd.c_str()) >= 0 && atoi(cmd.c_str()) < 50)
		{
			AMordhauCharacter* pChar = (AMordhauCharacter*)pCurrentPlayer->Character;
			AMordhauEquipment* equip = pChar->LeftHandEquipment;
			if (equip != nullptr)
			{
				if (equip->GetName().find("BP_Lute_C") == 0)
				{
					pCurrentPlayer->EquipmentCommand(atoi(cmd.c_str()));
				}
			}
		}

	}
	commandBuffer.clear();
}

vector<string> split(string str, char delimiter) {
	vector<string> internal;
	stringstream ss(str); // Turn the string into a stream.
	string tok;

	while (std::getline(ss, tok, delimiter)) {
		internal.push_back(tok);
	}

	return internal;
}
float range = 0;
int inser = 0;
int dele = 0;
int flying = 0;
int flyingTick = 0;
void gameLoop()
{
	DWORD_PTR base_addr = (DWORD_PTR)GetModuleHandleA(0);
//	while (1)
	{
		if (!rungameLoop)
		{
			rungameLoop = true;
//			Sleep(1);
			processCommands();

			if (GetKeyState(VK_SHIFT) & 0x8000)
			{
				UWorld* pUWorld = (UWorld*)(*(PDWORD_PTR)((uint64_t)base_addr + GWorld));
				AMordhauPlayerController * pCurrentPlayer = (AMordhauPlayerController*)pUWorld->OwningGameInstance->LocalPlayers[0]->PlayerController;

				if (pCurrentPlayer != nullptr)
				{
					ABP_MordhauCharacter_C* pChar = (ABP_MordhauCharacter_C*)pCurrentPlayer->Character;
					ABP_MordhauActor_C a;
					if (pChar != nullptr)
					{
						//if((int)pChar->bClientUpdating != 0)
						if (pChar->FallingTime > 0.8)
						{
							cout << "fall time: " << pChar->FallingTime << endl;
							FVector pos;
							pos = pos = pCurrentPlayer->Pawn->K2_GetActorLocation();
							FVector_NetQuantize posN;
							posN.X = pos.X;
							posN.Y = pos.Y;
							posN.Z = pos.Z;
							//pos.Z += 100;
							pChar->ClimbTargetLocation = pos;
							pChar->ServerSetClimbLocation(posN);
							pChar->RequestClimb(pos);

						}
						//pChar->StartSupersprint();
						pChar->Stamina = 100;
						//pChar->CharacterMovement->MaxTouchForce = 9999999;
						//pChar->ForceNetUpdate();
						ABP_DestroyableActor_C a;
						UMordhauMovementComponent* mov = (UMordhauMovementComponent*)pCurrentPlayer->Pawn->GetMovementComponent();
						//mov->MaxSpeedFalling = 0;
					}

				}
			}

			if (GetKeyState(VK_DELETE) & 0x8000 && dele == 0)
			{
				dele = 1;
				UWorld* pUWorld = (UWorld*)(*(PDWORD_PTR)((uint64_t)base_addr + GWorld));
				AMordhauPlayerController * pCurrentPlayer = (AMordhauPlayerController*)pUWorld->OwningGameInstance->LocalPlayers[0]->PlayerController;
				AMordhauCharacter *pChar = (AMordhauCharacter*)pCurrentPlayer->Character;
				//pChar->ServerQueueAttack
				//auto pChar->RightHandEquipment

				//flying = !flying;
				//flyingTick = 999999;
				/*
				UWorld* pUWorld = (UWorld*)(*(PDWORD_PTR)((uint64_t)base_addr + GWorld));
				AMordhauPlayerController * pCurrentPlayer = (AMordhauPlayerController*)pUWorld->OwningGameInstance->LocalPlayers[0]->PlayerController;

				if (pCurrentPlayer != nullptr)
				{

					ABP_MordhauCharacter_C* pChar = (ABP_MordhauCharacter_C*)pCurrentPlayer->Character;
					if (pChar != nullptr)
					{

					}

				}*/
			}
			else if (!(GetKeyState(VK_DELETE) & 0x8000)) { dele = 0; }

			if (flying && flyingTick++ > 100)
			{
				flyingTick = 0;
				UWorld* pUWorld = (UWorld*)(*(PDWORD_PTR)((uint64_t)base_addr + GWorld));
				AMordhauPlayerController * pCurrentPlayer = (AMordhauPlayerController*)pUWorld->OwningGameInstance->LocalPlayers[0]->PlayerController;

				if (pCurrentPlayer != nullptr)
				{

					ABP_MordhauCharacter_C* pChar = (ABP_MordhauCharacter_C*)pCurrentPlayer->Character;
					if (pChar != nullptr)
					{

						FVector rot = pCurrentPlayer->PlayerCameraManager->GetActorForwardVector();

						FVector pos;
						pos = pCurrentPlayer->Pawn->K2_GetActorLocation();
						//pos.X = atoi(splt[1].c_str());
						//pos.Y = atoi(splt[2].c_str());
						//pos.Z = atoi(splt[3].c_str());
#define FLYSPEED 500
						//pos = pCurrentPlayer->Pawn->K2_GetActorLocation();
						if (GetKeyState(0x57) & 0x8000)//W
						{
							pos.X += rot.X * FLYSPEED;
							pos.Y += rot.Y * FLYSPEED;
							pos.Z += rot.Z * FLYSPEED;
						}
						if (GetKeyState(0x44) & 0x8000)//D
						{
							pos.X -= rot.X * FLYSPEED;
							pos.Y -= rot.Y * FLYSPEED;
							pos.Z -= rot.Z * FLYSPEED;
						}
						if (GetKeyState(0x53) & 0x8000)//S
						{
							rot = pCurrentPlayer->PlayerCameraManager->GetActorRightVector();
							pos.X += rot.X * FLYSPEED;
							pos.Y += rot.Y * FLYSPEED;
							pos.Z += rot.Z * FLYSPEED;
						}
						if (GetKeyState(0x41) & 0x8000)//A
						{
							rot = pCurrentPlayer->PlayerCameraManager->GetActorRightVector();
							pos.X -= rot.X * FLYSPEED;
							pos.Y -= rot.Y * FLYSPEED;
							pos.Z -= rot.Z * FLYSPEED;
						}
						FVector_NetQuantize posN;
						posN.X = pos.X;
						posN.Y = pos.Y;
						posN.Z = pos.Z;
						pChar->ClimbTargetLocation = posN;
						pChar->ServerSetClimbLocation(posN);
						pChar->RequestClimb(pos);
						pChar->ClimbTargetLocation = posN;
						cout << "X: " << pos.X << endl;
						cout << "Y: " << pos.Y << endl;
						cout << "Z: " << pos.Z << endl;
						pCurrentPlayer->ClientDrawTracer(pCurrentPlayer->Pawn->K2_GetActorLocation(), pos);
						//pChar->CharacterMovement->MaxWalkSpeed *= 1.4;

					}

				}
			}

			if (GetKeyState(0x5A) & 0x8000 && inser == 0)
			{
				inser = 1;

				UWorld* pUWorld = (UWorld*)(*(PDWORD_PTR)((uint64_t)base_addr + GWorld));
				AMordhauPlayerController * pCurrentPlayer = (AMordhauPlayerController*)pUWorld->OwningGameInstance->LocalPlayers[0]->PlayerController;

				if (pCurrentPlayer != nullptr)
				{

					ABP_MordhauCharacter_C* pChar = (ABP_MordhauCharacter_C*)pCurrentPlayer->Character;
					if (pChar != nullptr)
					{

						FVector rot = pCurrentPlayer->PlayerCameraManager->GetActorForwardVector();

						FVector pos;
						//pos.X = atoi(splt[1].c_str());
						//pos.Y = atoi(splt[2].c_str());
						//pos.Z = atoi(splt[3].c_str());
						pos = pCurrentPlayer->Pawn->K2_GetActorLocation();
						//pos = pCurrentPlayer->Pawn->K2_GetActorLocation();
						pos.X += rot.X * 1000;
						pos.Y += rot.Y * 1000;
						pos.Z += rot.Z * 1000;
						FVector_NetQuantize posN;
						posN.X = pos.X;
						posN.Y = pos.Y;
						posN.Z = pos.Z;
						pChar->ClimbTargetLocation = posN;
						pChar->ServerSetClimbLocation(posN);
						pChar->RequestClimb(pos);
						pChar->ClimbTargetLocation = posN;
						cout << "X: " << pos.X << endl;
						cout << "Y: " << pos.Y << endl;
						cout << "Z: " << pos.Z << endl;
						pCurrentPlayer->ClientDrawTracer(pCurrentPlayer->Pawn->K2_GetActorLocation(), pos);
						//pChar->CharacterMovement->MaxWalkSpeed *= 1.4;

					}

				}
			}
			else if (!(GetKeyState(0x5A) & 0x8000)) { inser = 0; }
			rungameLoop = false;
		}
//		Sleep(1);
	}
}

bool spwnAct(class UObject* WorldContextObject, class AActor** SpawnedActor)
{
	static auto fn = UObject::FindObject<UFunction>("Function GameplayTasks.GameplayTask_SpawnActor.BeginSpawningActor");

	UGameplayTask_SpawnActor_BeginSpawningActor_Params params;
	params.WorldContextObject = WorldContextObject;

	auto flags = fn->FunctionFlags;
	fn->FunctionFlags |= 0x400;

	WorldContextObject->ProcessEvent(fn, &params);

	fn->FunctionFlags = flags;

	if (SpawnedActor != nullptr)
		* SpawnedActor = params.SpawnedActor;

	return params.ReturnValue;
}

void spwnACT(const TScriptInterface<class UGameplayTaskOwnerInterface>& TaskOwner, const struct FVector& SpawnLocation, const struct FRotator& SpawnRotation, class UClass* Class, bool bSpawnOnlyOnAuthority, UWorld *pUWorld)
{
	static auto fn = UObject::FindObject<UFunction>("Function GameplayTasks.GameplayTask_SpawnActor.SpawnActor");

	UGameplayTask_SpawnActor_SpawnActor_Params params;
	params.TaskOwner = TaskOwner;
	params.SpawnLocation = SpawnLocation;
	params.SpawnRotation = SpawnRotation;
	params.Class = Class;
	params.bSpawnOnlyOnAuthority = bSpawnOnlyOnAuthority;

	auto flags = fn->FunctionFlags;
	fn->FunctionFlags |= 0x400;

	pUWorld->ProcessEvent(fn, &params);

	fn->FunctionFlags = flags;

	//return params.ReturnValue;
}
static __int64 gameFunctionHook(char* var1, const wchar_t* var2, __int64 var3)
{
	//GetVFunction<void(*)(UObject*, class UFunction*, void*)>(this, 65)
	printf("Anyone there?\n");
	return 0;
}
/*
__int64 gameFunctionHook()
{
	cout << 'a';
	__int64 v0; // rax
	char v2; // rax
	char v4; // [rsp+30h] [rbp+8h]


	__int64(__cdecl * sub_141A7B870)();
	__int64(__cdecl * sub_1408A8B30)(char* i, const wchar_t* c, __int64 i2);
	__int64(__cdecl * sub_140A73090)(__int64 i);
	//__int64 *sub_1405E63B0(__int64);// = (__int64*)0x05E63B0
	sub_141A7B870 = (__int64(__cdecl*)()) (0x1A7B870 + GetModuleHandleA(0));
	sub_1408A8B30 = (__int64(__cdecl*)(char*, const wchar_t*, __int64)) (0x08A8B30 + GetModuleHandleA(0));
	sub_140A73090 = (__int64(__cdecl*)(__int64)) (0x0A73090 + GetModuleHandleA(0));

	v0 = (__int64)sub_1408A8B30(&v4, L"TickFunction", 1i64);
	return sub_140A73090((__int64)sub_141A7B870);

}
*/
/*
__int64 gameFunctionHook()
{
  __int64 v0; // rax
  __int64 v1; // rbx
  __int64 *v2; // rax
  char v4; // [rsp+30h] [rbp+8h]
  __int64(__cdecl * sub_1405E63B0)(__int64 i);
  __int64(__cdecl * sub_1408A8B30)(char *i, const wchar_t* c, __int64 i2);
  __int64(__cdecl * sub_14092DEF0)(__int64 i, __int64 i2);
  //__int64 *sub_1405E63B0(__int64);// = (__int64*)0x05E63B0
  sub_1405E63B0 = (__int64(__cdecl*)(__int64)) (0x05E63B0 + GetModuleHandleA(0));
  sub_1408A8B30 = (__int64(__cdecl*)(char*, const wchar_t*, __int64)) (0x05E63B0 + GetModuleHandleA(0));
  sub_14092DEF0 = (__int64(__cdecl*)(__int64, __int64)) (0x05E63B0 + GetModuleHandleA(0));
  
  v0 = sub_1405E63B0(16);
  
  v1 = v0;
  if ( v0 )
  {
	*(DWORD *)(v0 + 8) = 80;
	*(DWORD *)(v0 + 12) = 8;
	*(__int64*)v0 = *(__int64*)(0x2933CB0 + GetModuleHandleA(0));
  }
  else
  {
	v1 = 0i64;
  }
  v2 = (__int64 *)sub_1408A8B30(&v4, L"TickFunction", 1i64);
  cout << 'a';
  return sub_14092DEF0(*v2, v1);

}
*/
void init_sdk()
{
	AllocConsole();
	SetConsoleTitle(L"LuteMod v1.0");
	const auto gname_addr = Utilities::FindPattern("48 8B 05 ? ? ? ? 48 85 C0 75 5F");
	auto offset = *(uint32_t*)(gname_addr + 3);
	SDK::FName::GNames = (SDK::TNameEntryArray*)(*(uintptr_t*)(gname_addr + 7 + offset));

	const auto objects_addr = Utilities::FindPattern("4C 8B 15 ? ? ? ? 8D 43 01");
	offset = *(int32_t*)(objects_addr + 3);
	SDK::TUObjectArray::g_objects = (uintptr_t)(objects_addr + 7 + offset) - (uintptr_t)GetModuleHandleA(0);

	const auto total_objects_addr = Utilities::FindPattern("44 8B 15 ? ? ? ? 45 33 C9 45 85 D2");
	offset = *(int32_t*)(total_objects_addr + 3);
	SDK::TUObjectArray::g_total_objects = (uintptr_t)(total_objects_addr + 7 + offset) - (uintptr_t)GetModuleHandleA(0);


	const auto gworld_addr = Utilities::FindPattern("48 8B 1D ? ? ? ? 48 85 DB 74 3B 41 B0 01");
	offset = *(int32_t*)(gworld_addr + 3);
	GWorld = (uintptr_t)(gworld_addr + 7 + offset) - (uintptr_t)GetModuleHandleA(0);

	//__int64(__cdecl * sub_1401C6150)();
	//1C6150
	//*((__int64*(__cdecl*)()) (0x1C6150 + GetModuleHandleA(0))) = (__int64(__cdecl*)()) (0x05E63B0 + GetModuleHandleA(0));
	size_t bytesWritten = 0;
	printf("Attempting to hook...\n");

	//PLH::x86Detour detour((char*)& printf, (char*)& h_hookPrintf, &hookPrintfTramp, dis);

	/*
	cout << "Writing: " << WriteProcessMemory(Utilities::hAppInstance, (void*)(0x1A7B870 + GetModuleHandleA(0)), &gameFunctionHook, 4, &bytesWritten) << endl;
	printf("Function size: %d\n", bytesWritten);
	DWORD addr = (DWORD)&gameFunctionHook;
	*(DWORD*)(0x1C6C96 + GetModuleHandleA(0)) = addr;
	*/

	UWorld* pUWorld = (UWorld*)(*(PDWORD_PTR)((uint64_t)GetModuleHandleA(0) + GWorld));
	AMordhauPlayerController * pCurrentPlayer = (AMordhauPlayerController*)pUWorld->OwningGameInstance->LocalPlayers[0]->PlayerController;
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	ProcessEvent = (tProcessEvent)(GetVFunction<void(*)(UObject*, class UFunction*, void*)>(pCurrentPlayer, 65));
	DetourAttach(&(PVOID&)ProcessEvent, HookedPE);
	long error = DetourTransactionCommit();
	if(error == NOERROR)
		printf("Hooked successfully at address %08X\n", ProcessEvent);
	else
		printf("Hooked error!\n");

	printf("Found GNames        at: %08X.\n", SDK::FName::GNames);
	printf("Found Objects       at: %08X.\n", ::TUObjectArray::g_objects);
	printf("Found Total Objects at: %08X.\n", SDK::TUObjectArray::g_total_objects);
	printf("Found GWorld        at: %08X.\n", GWorld);
}

DWORD WINAPI DisplayMessageBox()
{
	init_sdk();
	//const int Offset_WorldPtr = 0x30BA2B8;
	DWORD_PTR base_addr = (DWORD_PTR)GetModuleHandleA(0);
	//FreeConsole();
	FILE* dummyStream;
	//freopen_s((FILE * *)stdin, "CONOUT$", "w", stdin);
	SetConsoleMode(GetModuleHandleA(0), ENABLE_WINDOW_INPUT);
	//SetConsoleMode(GetModuleHandleA(0), ENABLE_ECHO_INPUT);

	/*SetConsoleMode(GetModuleHandleA(0), ENABLE_ECHO_INPUT);
	SetConsoleMode(GetModuleHandleA(0), ENABLE_INSERT_MODE);
	SetConsoleMode(GetModuleHandleA(0), ENABLE_LINE_INPUT);
	SetConsoleMode(GetModuleHandleA(0), ENABLE_MOUSE_INPUT);
	SetConsoleMode(GetModuleHandleA(0), ENABLE_PROCESSED_INPUT);
	SetConsoleMode(GetModuleHandleA(0), ENABLE_QUICK_EDIT_MODE);
	SetConsoleMode(GetModuleHandleA(0), ENABLE_WINDOW_INPUT);
	SetConsoleMode(GetModuleHandleA(0), ENABLE_VIRTUAL_TERMINAL_INPUT);

	SetConsoleMode(GetModuleHandleA(0), ENABLE_PROCESSED_OUTPUT);
	SetConsoleMode(GetModuleHandleA(0), ENABLE_WRAP_AT_EOL_OUTPUT);
	SetConsoleMode(GetModuleHandleA(0), ENABLE_VIRTUAL_TERMINAL_PROCESSING);
	SetConsoleMode(GetModuleHandleA(0), DISABLE_NEWLINE_AUTO_RETURN);
	SetConsoleMode(GetModuleHandleA(0), ENABLE_LVB_GRID_WORLDWIDE);*/

	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
	string test;
	//freopen_s((FILE * *)stdin, "CONIN$", "rb", stdin);
	LPVOID base_address = GetModuleHandleA(0);

	//FName::GNames = (TNameEntryArray*)(*(PDWORD_PTR)((uint64_t)base_address + 0x2FB6E10));
	//UObject::GObjects = (FUObjectArray*)(*(PDWORD_PTR)((uint64_t)base_address + 0x2C51480));

	//init_sdk();

	printf("LuteMod v1 initialized.\n");
	printf("Console initialized.\n");
	UWorld* pUWorld = (UWorld*)(*(PDWORD_PTR)((uint64_t)base_address + GWorld));
	auto v1 = pUWorld->OwningGameInstance->LocalPlayers[0]->ViewportClient->ViewportConsole->ConsoleTargetPlayer->UnknownData00;
	auto v2 = pUWorld->OwningGameInstance->LocalPlayers[0]->ViewportClient->ViewportConsole->ConsoleTargetPlayer->UnknownData01;
	auto v3 = pUWorld->OwningGameInstance->LocalPlayers[0]->ViewportClient->ViewportConsole->ConsoleTargetPlayer->UnknownData02;
	auto v4 = pUWorld->OwningGameInstance->LocalPlayers[0]->ViewportClient->ViewportConsole->ConsoleTargetPlayer->UnknownData03;
	auto v5 = pUWorld->OwningGameInstance->LocalPlayers[0]->ViewportClient->ViewportConsole->ConsoleTargetPlayer->UnknownData04;
	while (true)
	{
		char input[100];
		cin.getline(input, sizeof(input));

		string read(input);
		cout << read << endl;
		read.erase(std::remove(read.begin(), read.end(), '\n'), read.end());
		transform(read.begin(), read.end(), read.begin(), ::tolower);
		if(read == "exit")
		{
			printf("Exiting LuteMod.\n");
			UWorld* pUWorld = (UWorld*)(*(PDWORD_PTR)((uint64_t)GetModuleHandleA(0) + GWorld));
			AMordhauPlayerController * pCurrentPlayer = (AMordhauPlayerController*)pUWorld->OwningGameInstance->LocalPlayers[0]->PlayerController;
			DetourTransactionBegin();
			DetourUpdateThread(GetCurrentThread());
			DetourDetach(&(PVOID&)ProcessEvent, HookedPE);
			long error = DetourTransactionCommit();
			printf("Detaching mod: %d\n", error);

			exiting = 1;
			UnloadModule(HMODULE(Utilities::hAppInstance));

			break;
		}
		else if (read == "test")
		{
			//uint64_t *ticks = (uint64_t*)(*(PDWORD_PTR)((uint64_t)base_address + 0x7D4D70));
			cout << "Tick count: " << counting << ".\n" << endl;
			//cout << "Bool val: " << *rungameLoop << ".\n" << endl;
			/*HackController* hack = new HackController();

			FVector *vec = new FVector();
			FRotator *rotor = new FRotator();
			TScriptInterface<class UGameplayTaskOwnerInterface> s;
			
			for (int i = 0; i < UObject::GetGlobalObjects().Num(); ++i)
			{
				auto object = UObject::GetGlobalObjects().GetByIndex(i);

				if (object == nullptr)
				{
					continue;
				}
				if (object->IsA(UGameplayTaskOwnerInterface::StaticClass()))
				{
					cout << "Object: " << object->GetFullName() << endl;
					//s = (TScriptInterface<class UGameplayTaskOwnerInterface>)object;
				}
			}*/
			//UGameplayTask_SpawnActor ugtsa();
			//spwnACT(s, *vec, *rotor, s., true, pUWorld);
			/*
			const std::string find = "243";
			Utilities::LoadModule();
			//auto guworld_addr = Utilities::FindPattern("48 8B 1D ? ? ? ? 48 85 DB 74 3B 41 B0 01");
			//UWorld* pUWorld = *reinterpret_cast<SDK::UWorld * *>(guworld_addr + 3);
			//printf("World offset: 0x%x\n", base_addr - (uint64_t)pUWorld);

			cout << "hi" << endl;

			//int objectcount = *(int*)(*(PDWORD_PTR)((uint64_t)base_address + 0x2C49844));
			//return *(int*)((uintptr_t)GetModuleHandleA(0) + 0x2C49844);

			cout << "FName::GNames = " << FName::GNames << endl;
			cout << "FName::GNames->Num() = " << FName::GNames->Num() << endl;


			cout << "UObject::GObjects = " << UObject::GObjects << endl;
			cout << "UObject::GObjects->ObjObjects.Num() = " << UObject::GObjects->ObjObjects.Num() << endl;
			//cout << "Object Count = " << objectcount << endl;

			UWorld* pUWorld = (UWorld*)(*(PDWORD_PTR)((uint64_t)base_address + 0x30BA2B8));
			APlayerController * pCurrentPlayer = (APlayerController*)pUWorld->OwningGameInstance->LocalPlayers[0]->PlayerController;
			printf("Player Controller offset: 0x%p\n", pCurrentPlayer);
			FString message(L"test");
			printf("Times jumped: 0x%d\n", pCurrentPlayer->Character->JumpForceTimeRemaining);
			FVector vec = FVector();
			vec.X = 4;
			vec.Y = 4;
			vec.Z = 4;
			cout << "Player name: " << pCurrentPlayer->GetFullName() << endl;
			AMordhauCharacter *pChar = (AMordhauCharacter*)pCurrentPlayer->Character;
			AMordhauEquipment *equip = pChar->LeftHandEquipment;
			if(equip != nullptr)
				cout << "Val 1: " << equip->GetName() << endl;
			
			cout << "Val 2: " << (v2 == pUWorld->OwningGameInstance->LocalPlayers[0]->ViewportClient->ViewportConsole->ConsoleTargetPlayer->UnknownData01) << endl;
			cout << "Val 3: " << (v3 == pUWorld->OwningGameInstance->LocalPlayers[0]->ViewportClient->ViewportConsole->ConsoleTargetPlayer->UnknownData02) << endl;
			cout << "Val 4: " << (v4 == pUWorld->OwningGameInstance->LocalPlayers[0]->ViewportClient->ViewportConsole->ConsoleTargetPlayer->UnknownData03) << endl;
			cout << "Val 5: " << (v5 == pUWorld->OwningGameInstance->LocalPlayers[0]->ViewportClient->ViewportConsole->ConsoleTargetPlayer->UnknownData04) << endl;
			v1 == pUWorld->OwningGameInstance->LocalPlayers[0]->ViewportClient->ViewportConsole->ConsoleTargetPlayer->UnknownData00;
			v2 == pUWorld->OwningGameInstance->LocalPlayers[0]->ViewportClient->ViewportConsole->ConsoleTargetPlayer->UnknownData01;
			v3 == pUWorld->OwningGameInstance->LocalPlayers[0]->ViewportClient->ViewportConsole->ConsoleTargetPlayer->UnknownData02;
			v4 == pUWorld->OwningGameInstance->LocalPlayers[0]->ViewportClient->ViewportConsole->ConsoleTargetPlayer->UnknownData03;
			v5 == pUWorld->OwningGameInstance->LocalPlayers[0]->ViewportClient->ViewportConsole->ConsoleTargetPlayer->UnknownData04;
			auto child = pCurrentPlayer->Character->BlueprintCreatedComponents[0];
			
			for (int i = 0; i < pCurrentPlayer->Character->BlueprintCreatedComponents.Num(); i++)
			{
				child = pCurrentPlayer->Character->BlueprintCreatedComponents[i];
				cout << "Child name: " << child->GetFullName() << endl;
			}
			//cout << "Val 1: " << pUWorld->GameState->GetGameTimeSinceCreation << endl;
			pCurrentPlayer->SendToConsole(message);
			//UKismetSystemLibrary::*/
		}
		else if (read.compare(0, 2, "c ") == 0)
		{

			UWorld* pUWorld = (UWorld*)(*(PDWORD_PTR)((uint64_t)base_address + GWorld));
			APlayerController * pCurrentPlayer = (APlayerController*)pUWorld->OwningGameInstance->LocalPlayers[0]->PlayerController;

			wstring messagewW = wstring(read.begin() + 2, read.end());
			const wchar_t* message = messagewW.c_str();
			cout << "Sending command: " << messagewW.c_str() << endl;

			pCurrentPlayer->SendToConsole(message);
		}
		else if (read.compare(0, 17, "equipmentcommand ") == 0 || read.compare(0, 16, "equipmentcomand ") == 0)
		{
			try {
				//cout << "History: " << pUWorld->OwningGameInstance->LocalPlayers[0]->ViewportClient->ViewportConsole->HistoryBuffer.Num() << endl;
			
				/*int notSame = 0;
				for (int i = 0; i < pUWorld->OwningGameInstance->LocalPlayers[0]->ViewportClient->ViewportConsole->HistoryBuffer.Num(); i++)
				{
				 /if (!dat[i].IsValid() || !pUWorld->OwningGameInstance->LocalPlayers[0]->ViewportClient->ViewportConsole->HistoryBuffer[i].IsValid())
					{
						notSame = true;
						break;
					}
					//if (dat[i].c_str() != pUWorld->OwningGameInstance->LocalPlayers[0]->ViewportClient->ViewportConsole->HistoryBuffer[i].c_str())
						notSame = true;
				}
				if (notSame > 0)
				{*/
				UWorld* pUWorld = (UWorld*)(*(PDWORD_PTR)((uint64_t) GetModuleHandleA(0) + GWorld));
				AMordhauPlayerController* pCurrentPlayer = (AMordhauPlayerController*)pUWorld->OwningGameInstance->LocalPlayers[0]->PlayerController;
				std::vector<std::string> splt = split(read, ' ');

				string messagewW = string(splt[1].begin(), splt[1].end());
				//messagewW = "equipmentcommand " + messagewW;
				messagewW = messagewW;
				//const wchar_t* message = messagewW.c_str();
				if (splt.size() <= 1)
					continue;
				int musicNote = atoi(splt[1].c_str());

				if (pCurrentPlayer != nullptr)
				{
					AMordhauCharacter* pChar = (AMordhauCharacter*)pCurrentPlayer->Character;
					


					AMordhauEquipment* equip = pChar->LeftHandEquipment;
					//pCurrentPlayer->SendToConsole(message);
					commandBuffer.push_back(messagewW);
					if (equip != nullptr)
					{
						if (equip->GetName().find("BP_Lute_C") == 0)
						{
							ABP_Lute_C* lute = (ABP_Lute_C*)equip;
							//DetourFunction
							float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
							PlayNote = musicNote;
							//cout << "Sending Music: " << PlayNote << endl;
							//pChar->RequestVoiceCommand((unsigned char)PlayNote, 0);
							//float note = -2.5 + r * 5;
							//pChar->RequestAttack(EAttackMove::AltStab, note);
							//lute->EquipmentCommand(&PlayNote);
							//WaitForSingleObject(Utilities::hAppInstance, 10000);
							//pCurrentPlayer->EquipmentCommand(PlayNote);
							//pUWorld->NetDriver->Time
							//cout << "Playing note: " << note << endl;
							
						}

					}
				}
				//	pCurrentPlayer->SendToConsole(message);
				//}
			}
			catch (int n)
			{
				cout << n << endl;
			}

		}
		else if (read.compare(0, 5, "play ") == 0)
		{
			std::vector<std::string> splt = split(read, ' ');

			int arg1 = atoi(splt[1].c_str());
			//float arg2 = atof(splt[2].c_str());


			UWorld* pUWorld = (UWorld*)(*(PDWORD_PTR)((uint64_t)base_address + GWorld));
			AMordhauPlayerController* pCurrentPlayer = (AMordhauPlayerController*)pUWorld->OwningGameInstance->LocalPlayers[0]->PlayerController;
			if (pCurrentPlayer != nullptr)
			{
				AMordhauCharacter* pChar = (AMordhauCharacter*)pCurrentPlayer->Character;
				AMordhauEquipment* equip = pChar->LeftHandEquipment;
				if (equip != nullptr)
				{
					if (equip->GetName().find("BP_Lute_C") == 0)
					{
						ABP_Lute_C* lute = (ABP_Lute_C*)equip;
						//pChar->RequestAttack((EAttackMove)arg1, arg2);
						//pChar->RequestParry((EBlockType)arg1, arg2);
						//pUWorld->OwningGameInstance->LocalPlayers[0]->ViewportClient->ViewportConsole

						wstring messagewW;
						messagewW = L"equipmentcommand " + arg1;

						const wchar_t* message = messagewW.c_str();
						UKismetSystemLibrary a;
						//a.STATIC_ExecuteConsoleCommand(pUWorld, message, pCurrentPlayer);
						pCurrentPlayer->EquipmentCommand(arg1);
						cout << "Sending Music: " << arg1 << endl;

					}

				}
			}
		}
		else if (read.compare(0, 8, "teleport") == 0)
		{
		std::vector<std::string> splt = split(read, ' ');



		UWorld* pUWorld = (UWorld*)(*(PDWORD_PTR)((uint64_t)base_address + GWorld));
		APlayerController * pCurrentPlayer = (APlayerController*)pUWorld->OwningGameInstance->LocalPlayers[0]->PlayerController;
		if (pCurrentPlayer != nullptr)
		{
			AMordhauCharacter* pChar = (AMordhauCharacter*)pCurrentPlayer->Character;
			if (pChar != nullptr)
			{


				FVector rot = pCurrentPlayer->Pawn->GetActorForwardVector();
				

				FVector pos;
				pos.X = atoi(splt[1].c_str());
				pos.Y = atoi(splt[2].c_str());
				pos.Z = atoi(splt[3].c_str());
				//pos = pCurrentPlayer->Pawn->K2_GetActorLocation();

				//pos.X += rot.X * 1000;
				//pos.Y += rot.Y * 1000;
				//pos.Z += rot.Z * 1000;
				pChar->ClimbTargetLocation = pos;
				pChar->RequestClimb(pos);
				cout << "X: " << pos.X << endl;
				cout << "Y: " << pos.Y << endl;
				cout << "Z: " << pos.Z << endl;
				//pChar->CharacterMovement->MaxWalkSpeed *= 1.4;

			}
		}
		}
	}
	return 0;
}
BOOL APIENTRY DllMain(HMODULE moduleHandle, DWORD reason, LPVOID reserved)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
	{
		DisableThreadLibraryCalls(moduleHandle);
		Utilities::curProcess = GetCurrentProcess();
		Utilities::hAppInstance = moduleHandle;
		Utilities::LoadModule();
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)DisplayMessageBox, 0, 0, 0);
		//CreateThread(0, 0, (LPTHREAD_START_ROUTINE)gameLoop, 0, 0, 0);
		break;
	}

	case DLL_THREAD_ATTACH:
	{
		break;
	}

	case DLL_PROCESS_DETACH:
	{
		UnloadModule(moduleHandle);


		break;
	}

	case DLL_THREAD_DETACH:
	{
		break;
	}
	}

	return true;
}