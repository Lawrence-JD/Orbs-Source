// Fill out your copyright notice in the Description page of Project Settings.


#include "RetainingLampPost.h"
#include "ChargingLampPost.h"
#include "HelperFiles/DefinedDebugHelpers.h"


ARetainingLampPost::ARetainingLampPost()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Default Light Variables
	LightIntensity = 40000.0f;
	LightComp = CreateDefaultSubobject<UPointLightComponent>(TEXT("Point Light"));
	LightComp->Intensity = LightIntensity;
	LightComp->bVisible = true;
	LightComp->SetupAttachment(MeshComp);
	LightComp->SetShadowBias(1);

	CurrentLightState = EColorState::Empty;
	LightComp->ToggleVisibility();
	
	bIsCharging = false;
	bIsDischarging = false;
	bchargePrep = false;
	ChargePercentage = 0;
	Speed = 50;
	CapsuleRadius = 100;
	SavedColor = FColor::Black;
	CollisionComp->SetCapsuleHalfHeight(CapsuleRadius);
	CollisionComp->SetCapsuleRadius(CapsuleRadius);

}

// Calls Parent Function to change Color based on Main Hand Color
void ARetainingLampPost::ToggleColorChange()
{
	CollisionComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_EngineTraceChannel3, ECollisionResponse::ECR_Ignore);
	if (ChargePercentage <= 0)
	{
		bIsCharging = true;
		Super::ToggleColorChange();
		if (this->IsA(ARetainingLampPost::StaticClass()) || this->IsA(AChargingLampPost::StaticClass()))
		{
			LightComp->bVisible = true;
			switch (CurrentLightState)
			{
			case EColorState::Blue:
				SavedColor = FLinearColor(0.5, 1, 0.99, 1.0);
				LightComp->SetLightColor(FLinearColor(0.5, 1, 0.99, 1.0));
				break;
			case EColorState::Yellow:
				SavedColor = FLinearColor(1, 1, 0.491, 1.0);
				LightComp->SetLightColor(FLinearColor(1, 1, 0.491, 1.0));
				break;
			case EColorState::Red:
				SavedColor = FLinearColor(0.72, 0.09, 0.07, 1.0);
				LightComp->SetLightColor(FLinearColor(0.72, 0.09, 0.07, 1.0));
				break;
			case EColorState::Empty:
				SavedColor = FColor::Black;
				LightComp->SetLightColor(FColor::Black);
				break;
			}
			// This is needed to change light from Empty to a Color light
			this->MarkComponentsRenderStateDirty();
		}
	}else if (ChargePercentage >= 100)
	{
		bIsDischarging = true;
		Super::ToggleColorChange();
		switch (CurrentLightState)
		{
		case EColorState::Blue:
			if (SavedColor != FLinearColor(0.5, 1, 0.99, 1.0))
			{
				SavedColor = FLinearColor(0.5, 1, 0.99, 1.0);
				bchargePrep = true;
			}
			break;
		case EColorState::Yellow:
			if (SavedColor != FLinearColor(1, 1, 0.491, 1.0))
			{
				SavedColor = FLinearColor(1, 1, 0.491, 1.0);
				bchargePrep = true;
			}
			break;
		case EColorState::Red:
			if (SavedColor != FLinearColor(0.72, 0.09, 0.07, 1.0))
			{
				SavedColor = FLinearColor(0.72, 0.09, 0.07, 1.0);
				bchargePrep = true;
			}
			break;
		case EColorState::Empty:
			if (SavedColor != FColor::Black)
			{
				SavedColor = FColor::Black;
				bchargePrep = true;
			}
			break;
		}
	}
	CollisionComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_EngineTraceChannel3, ECollisionResponse::ECR_Overlap);
}

void ARetainingLampPost::Tick(float DeltaTime)
{
	//if (bPlayerInRange)
	//{
	//	ToggleColorChange();
	//}
	// If Active Button is Pressed
	if (bIsCharging)
	{
		// As long as Charge % is not full AND Main Hand Color matches LampPost Color
		if (ChargePercentage <= 100 /*&& (GETENUMSTRING("EMainHandColorState", PlayerCharacter->CurrentMainHand) == GETENUMSTRING("EColorState", CurrentLightState)*/)
		{
			ChargePercentage += 2 * Speed * DeltaTime;
			LightComp->SetIntensity(ChargePercentage * 400);
			V_LOGF("+Charge: ", ChargePercentage);
		}
		else
		{
			bIsCharging = false;
		}
	}
	else if (bIsDischarging) // When Active Button is not Pressed
	{
		// If Charge % has juice
		if (ChargePercentage >= 0)
		{
			ChargePercentage -= 2 * Speed * DeltaTime;
			LightComp->SetIntensity(ChargePercentage * 400);
			V_LOGF("-Charge: ", ChargePercentage);
			
			// When Charge Depletes
			/*if (ChargePercentage < 0 && bchargePrep == false)
			{
				LightComp->SetLightColor(FColor::Black);
			}*/
		}
		else
		{
			bIsDischarging = false;
			if (bchargePrep == true)
			{
				bIsCharging = true;
				LightComp->SetLightColor(SavedColor);
				bchargePrep = false;
			}
			else if (bchargePrep == false)
			{
				SavedColor = FColor::Black;
				LightComp->SetLightColor(FColor::Black);
			}
		}
	}
}

/**
  * Called when an instance of this class is placed (in editor) or spawned.
  * @param    Transform            The transform the actor was constructed at.
  */
void ARetainingLampPost::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// For In Editor Changes, When Light State changes, all of its corresponding features do as well.
	LightComp->Intensity = LightIntensity;
	CollisionComp->SetCapsuleHalfHeight(CapsuleRadius);
	CollisionComp->SetCapsuleRadius(CapsuleRadius);
	switch (CurrentLightState)
	{
		case EColorState::Blue:
			SavedColor = FLinearColor(0.5, 1, 0.99, 1.0);
			LightComp->SetLightColor(FColor::Blue);
			LightComp->bVisible = true;
			break;
		case EColorState::Yellow:
			SavedColor = FLinearColor(1, 1, 0.491, 1.0);
			LightComp->SetLightColor(FColor::Yellow);
			LightComp->bVisible = true;
			break;
		case EColorState::Red:
			SavedColor = FLinearColor(0.72, 0.09, 0.07, 1.0);
			LightComp->SetLightColor(FColor::Red);
			LightComp->bVisible = true;
			break;
		case EColorState::Empty:
			SavedColor = FColor::Black;
			LightComp->SetLightColor(FColor::Black);
			LightComp->bVisible = false;
			break;
	}
}
