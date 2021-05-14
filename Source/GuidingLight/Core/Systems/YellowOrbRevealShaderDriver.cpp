// Fill out your copyright notice in the Description page of Project Settings.


#include "YellowOrbRevealShaderDriver.h"

// Sets default values for this component's properties
UYellowOrbRevealShaderDriver::UYellowOrbRevealShaderDriver()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UYellowOrbRevealShaderDriver::BeginPlay()
{
	Super::BeginPlay();

	// This Spawns an instance of the material collection
	// which is added as a property in editor
	MaterialPCInstance = GetWorld()->GetParameterCollectionInstance(MaterialParameterCollectionAsset);

	// Gets a pointer to the player character
	ThePlayer = Cast<AGuidingLightCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	
}


// Called every frame
void UYellowOrbRevealShaderDriver::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (MaterialPCInstance && ThePlayer)
	{
		// Sets the parameter of the revealing light's position in the MPC
		MaterialPCInstance->SetVectorParameterValue("LightPosition", ThePlayer->GetActorLocation() + (ThePlayer->GetActorForwardVector().Normalize() * FVector(57, -7, 80)));
		if (ThePlayer->Orbs.PYellowOrb->bIsActive)
		{
			MaterialPCInstance->SetScalarParameterValue("CurrentOrbColor", 1); //(float)ThePlayer->CurrentMainHand
		}
		else
		{
			MaterialPCInstance->SetScalarParameterValue("CurrentOrbColor", 0);
		}
		//UE_LOG(LogTemp, Warning, TEXT("Orb Value: %d"), (int)ThePlayer->CurrentMainHand);
	}
}

