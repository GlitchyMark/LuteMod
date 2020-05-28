// Fill out your copyright notice in the Description page of Project Settings.

#include "pch.h"
#include "SDK/SDK.hpp"
#include <iostream>

namespace SDK
{

	class HackController : public UObject
	{
	public:
		static UClass* StaticClass()
		{
			static auto ptr = UObject::FindClass("Class Engine.Actor");
			return ptr;
		}


		void ReceiveTick(float DeltaSeconds)
		{
			std::cout << "this is a test: " << DeltaSeconds << std::endl;
		}
	};
}