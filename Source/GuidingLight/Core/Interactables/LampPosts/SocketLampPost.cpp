// Fill out your copyright notice in the Description page of Project Settings.


#include "SocketLampPost.h"
#include "Core/Crystal/GuidingLightCharacter.h"
#include "HelperFiles/DefinedDebugHelpers.h"

ASocketLampPost::ASocketLampPost()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Default Light Variables
	LightIntensity = 40000.0f;
	LightComp->Intensity = LightIntensity;
	LightComp->bVisible = true;
	LightComp->SetupAttachment(MeshComp);

	CurrentLightState = EColorState::Empty;
	LightComp->ToggleVisibility();

	bOrbInSocket = false;
	CapsuleRadius = 100;
	CollisionComp->SetCapsuleHalfHeight(CapsuleRadius);
	CollisionComp->SetCapsuleRadius(CapsuleRadius);
}

void ASocketLampPost::ToggleColorChange()
{
	// Record Lamp Post Color to Return Orb to Main Hand
	EColorState TempLightState = CurrentLightState;
	Super::ToggleColorChange();

	// Return Orb to Main Hand and turn off Lamp Post Light
	if (bOrbInSocket)
	{
		LightComp->SetLightColor(FColor::Black);
		LightComp->SetVisibility(false);
		PlayerCharacter->SwitchOrb(GETENUMSTRING("EMainHandColorState", TempLightState));
		SCREENMSG2C("Current Orb Color is:  ", GETENUMSTRING("EMainHandColorState", PlayerCharacter->CurrentMainHand), Red);
		bOrbInSocket = false;
	}
	else 	// Set Orb to Socket Lamp Post and remove Orb from Main Hand
	{
		LightComp->bVisible = true;
		switch (CurrentLightState)
		{
		case EColorState::Blue:
			LightComp->SetLightColor(FColor::Blue);
			PlayerCharacter->SwitchOrb("Empty");
			bOrbInSocket = true;
			SCREENMSG2C("Current Orb Color is:  ", GETENUMSTRING("EMainHandColorState", PlayerCharacter->CurrentMainHand), Red);
			break;
		case EColorState::Yellow:
			LightComp->SetLightColor(FColor::Yellow);
			PlayerCharacter->SwitchOrb("Empty");
			bOrbInSocket = true;
			SCREENMSG2C("Current Orb Color is:  ", GETENUMSTRING("EMainHandColorState", PlayerCharacter->CurrentMainHand), Red);
			break;
		case EColorState::Red:
			LightComp->SetLightColor(FColor::Red);
			PlayerCharacter->SwitchOrb("Empty");
			bOrbInSocket = true;
			SCREENMSG2C("Current Orb Color is:  ", GETENUMSTRING("EMainHandColorState", PlayerCharacter->CurrentMainHand), Red);
			break;
		case EColorState::Empty:
			LightComp->SetLightColor(FColor::Black);
			break;
		}
		// This is needed to change light from Empty to a Color light
		this->MarkComponentsRenderStateDirty();
	}
}

/**
  * Called when an instance of this class is placed (in editor) or spawned.
  * @param    Transform            The transform the actor was constructed at.
  */
void ASocketLampPost::OnConstruction(const FTransform& Transform)
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

