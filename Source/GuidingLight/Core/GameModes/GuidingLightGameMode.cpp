// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "GuidingLightGameMode.h"
#include "Core/Crystal/GuidingLightCharacter.h"
#include "Core/GameModes/GuidingLightPlayerController.h"
#include "UObject/ConstructorHelpers.h"

AGuidingLightGameMode::AGuidingLightGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/GuidingLight/Core/Character/Crystal/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
		PlayerControllerClass = AGuidingLightPlayerController::StaticClass();
	}
}
