// Fill out your copyright notice in the Description page of Project Settings.


#include "AmplifierLampPost.h"
#include "Core/Crystal/GuidingLightCharacter.h"
#include "HelperFiles/DefinedDebugHelpers.h"

AAmplifierLampPost::AAmplifierLampPost()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;


	LightIntensity = 40000.0f;
	LightComp = CreateDefaultSubobject<USpotLightComponent>(TEXT("Spot Light"));
	LightComp->SetSourceRadius(100);
	LightComp->SetAttenuationRadius(2000);
	LightComp->SetOuterConeAngle(15);
	LightComp->Intensity = LightIntensity;
	LightComp->bVisible = true;
	LightComp->SetupAttachment(MeshComp);

	CurrentLightState = EColorState::Empty;
	LightComp->ToggleVisibility();

	// Default Variables for AMP State
	TurnDegrees = 60;
	Direction = FVector(1.0, 0.0, 0.0);
	bIsTurning = false;
	bOrbInSocket = false;
	CapsuleRadius = 160;
	CollisionComp->SetCapsuleHalfHeight(CapsuleRadius);
	CollisionComp->SetCapsuleRadius(CapsuleRadius);

	
}

// Set Turning Flag to True
void AAmplifierLampPost::TurnAmp()
{
	//if (!bIsTurning)
	//{
	//	bIsTurning = true;
	//}
}

// Calls Parent Function to change Color based on Main Hand Color
void AAmplifierLampPost::ToggleColorChange()
{
	// Record Lamp Post Color to Return Orb to Main Hand
	//EColorState TempLightState = CurrentLightState;
	//Super::ToggleColorChange();

	// Return Orb to Main Hand and turn off Lamp Post Light
	//if (bOrbInSocket)
	//{
	//	LightComp->SetLightColor(FColor::Black);
	//	LightComp->SetVisibility(false);
	//	PlayerCharacter->SwitchOrb(GETENUMSTRING("EMainHandColorState", TempLightState));
	//	SCREENMSG2C("MainHand Orb Color is:  ", GETENUMSTRING("EMainHandColorState", PlayerCharacter->CurrentMainHand), Red);
	//	bOrbInSocket = false;
	//}
	//else 	// Set Orb to AMP Lamp Post and remove Orb from Main Hand
	//{
	//	LightComp->bVisible = true;
	//	switch (CurrentLightState)
	//	{
	//	case EColorState::Blue:
	//		LightComp->SetLightColor(FColor::Blue);
	//		PlayerCharacter->SwitchOrb("Empty");
	//		bOrbInSocket = true;
	//		SCREENMSG2C("MainHand Orb Color is:  ", GETENUMSTRING("EMainHandColorState", PlayerCharacter->CurrentMainHand), Red);
	//		break;
	//	case EColorState::Yellow:
	//		LightComp->SetLightColor(FColor::Yellow);
	//		PlayerCharacter->SwitchOrb("Empty");
	//		bOrbInSocket = true;
	//		SCREENMSG2C("MainHand Orb Color is:  ", GETENUMSTRING("EMainHandColorState", PlayerCharacter->CurrentMainHand), Red);
	//		break;
	//	case EColorState::Red:
	//		LightComp->SetLightColor(FColor::Red);
	//		PlayerCharacter->SwitchOrb("Empty");
	//		bOrbInSocket = true;
	//		SCREENMSG2C("MainHand Orb Color is:  ", GETENUMSTRING("EMainHandColorState", PlayerCharacter->CurrentMainHand), Red);
	//		break;
	//	case EColorState::Empty:
	//		LightComp->SetLightColor(FColor::Black);
	//		break;
	//	}
	//	// This is needed to change light from Empty to a Color light
	//	this->MarkComponentsRenderStateDirty();
	//}
}

void AAmplifierLampPost::BeginPlay()
{
	Super::BeginPlay();
}

void AAmplifierLampPost::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Active when Actor is Turning
	//if (bIsTurning)
	//{
	//	Direction.Normalize();
	//	// Target Direction
	//	FVector TargetDir = Direction.RotateAngleAxis(TurnDegrees, FVector(0, 0, 1));
	//	TargetDir.Normalize();
	//	// Smooth Rotation From Current Direction to Target Direction
	//	SetActorRotation(FMath::Lerp(GetActorRotation(), TargetDir.Rotation(), 0.1f));
	//	
	//	FVector ForwardVec = GetActorForwardVector();
	//	ForwardVec.Normalize();
	//
	//	// Difference in angles between Actor Forward Vector and Target Direction Vector
	//	if(FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(ForwardVec, TargetDir))) < .5)
	//	{
	//		// Finish off Rotation
	//		SetActorRotation(FRotator(0, FMath::CeilToFloat(GetActorRotation().Yaw), 0));
	//		Direction = TargetDir;
	//		// Set Turning to False and Ready for next Stage
	//		bIsTurning = false;
	//	}
	//	
	//	
	//}
}

/**
  * Called when an instance of this class is placed (in editor) or spawned.
  * @param    Transform            The transform the actor was constructed at.
  */
void AAmplifierLampPost::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// For In Editor Changes, When Light State changes, all of its corresponding features do as well.
	LightComp->Intensity = LightIntensity;
	CollisionComp->SetCapsuleHalfHeight(CapsuleRadius);
	CollisionComp->SetCapsuleRadius(CapsuleRadius);

	switch (CurrentLightState)
	{
	case EColorState::Blue:
		LightComp->SetLightColor(FColor::Blue);
		LightComp->bVisible = true;
		break;
	case EColorState::Yellow:
		LightComp->SetLightColor(FColor::Yellow);
		LightComp->bVisible = true;
		break;
	case EColorState::Red:
		LightComp->SetLightColor(FColor::Red);
		LightComp->bVisible = true;
		break;
	case EColorState::Empty:
		LightComp->SetLightColor(FColor::Black);
		LightComp->bVisible = false;
		break;
	}
}
