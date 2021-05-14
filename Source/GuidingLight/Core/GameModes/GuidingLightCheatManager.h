// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"

#include "Core/Crystal/GuidingLightCharacter.h"

#include "GuidingLightCheatManager.generated.h"

/**
 * 
 */
UCLASS()
class GUIDINGLIGHT_API UGuidingLightCheatManager : public UCheatManager
{
	GENERATED_BODY()
	
	/** Cheat Command ex. SwitchOrb "Red" : switched Main Hand Color */
	UFUNCTION(Exec)
	void SwitchOrb(FString Orb);

	/** Cheat Command ex. TakeHealth 10 : Lose 10 Health */
	UFUNCTION(Exec)
	void TakeHealth(int HealthLoss);
};
