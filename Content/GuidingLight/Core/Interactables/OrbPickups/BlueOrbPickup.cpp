// Fill out your copyright notice in the Description page of Project Settings.


#include "BlueOrbPickup.h"
#include "Core/Crystal/GuidingLightCharacter.h"
#include "HelperFiles/DefinedDebugHelpers.h"

ABlueOrbPickup::ABlueOrbPickup()
{
	PrimaryActorTick.bCanEverTick = true;

	//Creating the Tags for the Red Orb
	this->Tags.Add(FName("Blue"));
	OrbLightCollisionSphere->ComponentTags.Add(FName("Blue"));
	DummyRoot->ComponentTags.Add(FName("Blue"));

	//Setting up the values for the Orb Point Light
	OrbLight->SetLightColor(FLinearColor(0.5, 1, 0.99, 1.0));
	OrbLight->bUseTemperature = false;
	OrbLight->SetLightFalloffExponent(10000);
	OrbLight->SetIntensity(35000);
	OrbLight->SetAttenuationRadius(750);
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

void ABlueOrbPickup::PutOrbInInventory()
{
	Crystal->bBlueOrbInInventory = true;
	Crystal->SwapToPlayerBlueOrb();
	Destroy();
}
