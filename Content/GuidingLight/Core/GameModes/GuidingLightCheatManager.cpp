// Fill out your copyright notice in the Description page of Project Settings.


#include "GuidingLightCheatManager.h"
#include "GameFramework/PlayerController.h"
#include "HelperFiles/DefinedDebugHelpers.h"
#include "Core/DamageTypes/DamageTypeFall.h"

// Cheat Command ex. SwitchOrb "Red" switched Main Hand Color
void UGuidingLightCheatManager::SwitchOrb(FString Orb)
{
	APawn* Pawn = GetOuterAPlayerController()->GetPawn();

	// Note: only works on characters
	AGuidingLightCharacter* Character = Cast<AGuidingLightCharacter>(Pawn);
	if (Character)
	{
		// If Successful Switch Message Prints to Screen
		if (Character->SwitchOrb(Orb))
		{
			SCREENMSG2C("Cheat Code Activated: MainHand Orb Color ", Orb, Red);
		}
	}
}

void UGuidingLightCheatManager::TakeHealth(int HealthLoss)
{
	APawn* Pawn = GetOuterAPlayerController()->GetPawn();

	// Note: only works on characters
	AGuidingLightCharacter* Character = Cast<AGuidingLightCharacter>(Pawn);
	if (Character)
	{
		TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(UDamageTypeFall::StaticClass());
		FDamageEvent DamageEvent(ValidDamageTypeClass);
		
		const float ActualDamage = Character->TakeDamage(HealthLoss, DamageEvent, GetOuterAPlayerController(), Character);
		
		SCREENMSG2C("Cheat Code Activated: Taking Damage -", FString::FromInt(ActualDamage), Red);
		
	}
}
