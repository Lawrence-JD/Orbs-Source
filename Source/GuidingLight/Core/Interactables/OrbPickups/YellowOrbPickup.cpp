// Fill out your copyright notice in the Description page of Project Settings.


#include "YellowOrbPickup.h"
#include "Core/Crystal/GuidingLightCharacter.h"
#include "HelperFiles/DefinedDebugHelpers.h"


AYellowOrbPickup::AYellowOrbPickup()
{
	YellowOrb = CreateDefaultSubobject<UYellowOrb>(TEXT("YellowOrb"));
	PickupCollisionBox->SetupAttachment(YellowOrb);
	YellowOrb->SetupAttachment(YellowOrb->DummyRoot);
	YellowOrb->OrbLightCollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
}

void AYellowOrbPickup::PutOrbInInventory()
{
	if (bIsActive)
	{
		YellowOrb->Crystal->bYellowOrbInInventory = true;
		YellowOrb->Crystal->SwapToPlayerYellowOrb();
		Destroy();
	}
}