 // Fill out your copyright notice in the Description page of Project Settings.


#include "RedOrbPickup.h"
#include "Core/Crystal/GuidingLightCharacter.h"
#include "HelperFiles/DefinedDebugHelpers.h"

ARedOrbPickup::ARedOrbPickup()
{
	PrimaryActorTick.bCanEverTick = true;

	//Creating the Tags for the Red Orb
	this->Tags.Add(FName("Red"));
	OrbLightCollisionSphere->ComponentTags.Add(FName("Red"));
	DummyRoot->ComponentTags.Add(FName("Red"));

	//Setting up the values for the Orb Point Light
	OrbLight->SetLightColor(FLinearColor(0.72, 0.09, 0.07, 1.0));
	OrbLight->bUseTemperature = false;
	OrbLight->SetLightFalloffExponent(10000);
	OrbLight->SetIntensity(35000);
	OrbLight->SetAttenuationRadius(675);
	OrbLight->SetShadowBias(1);


	//Setting up collision radius
	OrbLightCollisionSphere->SetSphereRadius(550);

	//Set Orb Mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MaterialSphere(TEXT("StaticMesh'/Game/StarterContent/Props/MaterialSphere.MaterialSphere'"));
	if (MaterialSphere.Object)
	{
		OrbMesh->SetStaticMesh(MaterialSphere.Object);
		FVector Scale = FVector(.25);
		OrbMesh->SetWorldScale3D(Scale);
	}
}

void ARedOrbPickup::PutOrbInInventory()
{
	Crystal->bRedOrbInInventory = true;
	Crystal->SwapToPlayerRedOrb();
	Destroy();
}
