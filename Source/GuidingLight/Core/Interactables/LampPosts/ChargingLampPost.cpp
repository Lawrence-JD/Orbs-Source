// Fill out your copyright notice in the Description page of Project Settings.


#include "ChargingLampPost.h"
#include "HelperFiles/DefinedDebugHelpers.h"

#include "Core/Crystal/GuidingLightCharacter.h"

AChargingLampPost::AChargingLampPost()
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

	ChargePercentage = 0;
	Speed = 0.1;
	CapsuleRadius = 100;
	CollisionComp->SetCapsuleHalfHeight(CapsuleRadius);
	CollisionComp->SetCapsuleRadius(CapsuleRadius);
}

void AChargingLampPost::ToggleColorChange()
{
	if (!bIsCharging)
	{
		// Only Change Color if Charge is Empty
		if (ChargePercentage <= 0)
		{
			Super::ToggleColorChange();
		}
		bIsCharging = true;
	}
}

void AChargingLampPost::ReleaseCharge()
{
	if (bIsCharging)
	{
		bIsCharging = false;
	}
}

/**
  * Called when an instance of this class is placed (in editor) or spawned.
  * @param    Transform            The transform the actor was constructed at.
  */
void AChargingLampPost::OnConstruction(const FTransform& Transform)
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

void AChargingLampPost::Tick(float DeltaTime)
{
	// If Active Button is Pressed
	if (bIsCharging)
	{
		// As long as Charge % is not full AND Main Hand Color matches LampPost Color
		if (ChargePercentage <= 100 && ( GETENUMSTRING("EMainHandColorState", PlayerCharacter->CurrentMainHand) == GETENUMSTRING("EColorState", CurrentLightState)))
		{
			ChargePercentage += 2 * Speed * DeltaTime;
			LightComp->SetIntensity(ChargePercentage * 400);
			V_LOGF("+Charge: ", ChargePercentage);
		}
		else 
		{
			// If Charge % has juice
			if (ChargePercentage >= 0)
			{
				ChargePercentage -= 0.5 * Speed * DeltaTime;
				LightComp->SetIntensity(ChargePercentage * 400);
				V_LOGF("-Charge: ", ChargePercentage);
				// When Charge Depletes
				if (ChargePercentage < 0)
				{
					ChangeColor(EColorState::Empty);
				}
			}
		}
	}
	else // When Active Button is not Pressed
	{
		// If Charge % has juice
		if (ChargePercentage >= 0)
		{
			ChargePercentage -= 0.5 * Speed * DeltaTime;
			LightComp->SetIntensity(ChargePercentage * 400);
			V_LOGF("-Charge: ", ChargePercentage);
			// When Charge Depletes
			if (ChargePercentage < 0)
			{
				ChangeColor(EColorState::Empty);
			}
		}
	}
}
