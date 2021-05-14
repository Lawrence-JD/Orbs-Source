// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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

	virtual void PutOrbInInventory() override;

};
