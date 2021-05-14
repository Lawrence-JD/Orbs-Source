// Fill out your copyright notice in the Description page of Project Settings.


#include "YellowOrb.h"
 
AYellowOrb::AYellowOrb()
{
	//Allows for orb to be edited from the editor
	this->bEditable = true;

	//Creating the Tags for the Yellow Orb
	this->Tags.Add(FName("Yellow"));
	OrbLightCollisionSphere->ComponentTags.Add(FName("Yellow"));
	DummyRoot->ComponentTags.Add(FName("Yellow"));

	//Setting up the values for the Orb Point Light
	OrbLight->SetLightColor(FLinearColor(1, 1, 0.491, 1.0));
	OrbLight->bUseTemperature = false;
	OrbLight->SetLightFalloffExponent(10000);
	OrbLight->SetIntensity(1700);
	OrbLight->SetAttenuationRadius(1500);
	OrbLight->SetShadowBias(1);


	OrbLightCollisionSphere->SetSphereRadius(600);
	OrbLightCollisionSphere->SetVisibility(true);

	//Set Orb Mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MaterialSphere(TEXT("StaticMesh'/Game/StarterContent/Props/SM_YellowOrb.SM_YellowOrb'"));
	if (MaterialSphere.Object)
	{
		OrbMesh->SetStaticMesh(MaterialSphere.Object);
		FVector Scale = FVector(.25);
		OrbMesh->SetWorldScale3D(Scale);
	}
		
		
}

void AYellowOrb::TurnOffLight()
{
	OrbLight->bVisible = false;
}
