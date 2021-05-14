// Fill out your copyright notice in the Description page of Project Settings.


#include "GuidingLightPlayerController.h"

AGuidingLightPlayerController::AGuidingLightPlayerController()
{

	CheatClass = UGuidingLightCheatManager::StaticClass();
	bIsUsingGamepad = false;
	bResetGamepadDetectionAfterNoInput = true;
	GamepadTimeout = 5.f;
}

bool AGuidingLightPlayerController::InputAxis(FKey Key, float Delta, float DeltaTime, int32 NumSamples, bool bGamepad)
{
	bool ret = Super::InputAxis(Key, Delta, DeltaTime, NumSamples, bGamepad);
	_UpdateGamepad(bGamepad);
	return ret;
}

bool AGuidingLightPlayerController::InputKey(FKey Key, EInputEvent EventType, float AmountDepressed, bool bGamepad)
{
	bool ret = Super::InputKey(Key, EventType, AmountDepressed, bGamepad);
	_UpdateGamepad(bGamepad);
	return ret;
}

void AGuidingLightPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	if (IsLocalController() && bResetGamepadDetectionAfterNoInput && bIsUsingGamepad)
	{
		float now = GetWorld()->TimeSeconds;
		if (now > LastGamepadInputTime + GamepadTimeout)
		{
			bIsUsingGamepad = false;
		}
	}
}