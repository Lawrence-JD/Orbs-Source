 // Fill out your copyright notice in the Description page of Project Settings.


#include "RedOrbPickup.h"
#include "Core/Crystal/GuidingLightCharacter.h"
#include "HelperFiles/DefinedDebugHelpers.h"

ARedOrbPickup::ARedOrbPickup()
{
	//V_LOG("RED ORB PICKUP START");
	
	RedOrb = CreateDefaultSubobject<URedOrb>(TEXT("RedOrb"));
	PickupCollisionBox->SetupAttachment(RedOrb);
	RedOrb->SetupAttachment(RedOrb->DummyRoot);
	RedOrb->OrbLightCollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
}

void ARedOrbPickup::PutOrbInInventory()
{
	if (bIsActive)
	{
		RedOrb->Crystal->bRedOrbInInventory = true;
		RedOrb->Crystal->SwapToPlayerRedOrb();
		Destroy();
	}
}
