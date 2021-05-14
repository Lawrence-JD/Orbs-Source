// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Orbs/YellowOrb.h"

#include "CoreMinimal.h"
#include "Interactables/OrbPickups/OrbPickupParent.h"
#include "YellowOrbPickup.generated.h"

/**
 * 
 */
UCLASS()
class GUIDINGLIGHT_API AYellowOrbPickup : public AOrbPickupParent
{
	GENERATED_BODY()

public:

	AYellowOrbPickup();

	UYellowOrb* YellowOrb;

	virtual void PutOrbInInventory() override;

};
