// Fill out your copyright notice in the Description page of Project Settings.


#include "BlueOrbPickup.h"
#include "Core/Crystal/GuidingLightCharacter.h"
#include "HelperFiles/DefinedDebugHelpers.h"

ABlueOrbPickup::ABlueOrbPickup()
{
	//V_LOG("BLUE ORB PICKUP START");
	
	BlueOrb = CreateDefaultSubobject<UBlueOrb>(TEXT("BlueOrb"));
	PickupCollisionBox->SetupAttachment(BlueOrb);
	BlueOrb->SetupAttachment(BlueOrb->DummyRoot);
	BlueOrb->OrbLightCollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABlueOrbPickup::PutOrbInInventory()
{
	if (bIsActive)
	{
		BlueOrb->Crystal->bBlueOrbInInventory = true;
		BlueOrb->Crystal->SwapToPlayerBlueOrb();
		Destroy();
	}
}
