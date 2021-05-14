// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/OrbPickups/OrbPickupParent.h"
#include "BlueOrbPickup.generated.h"

/**
 * 
 */
UCLASS()
class GUIDINGLIGHT_API ABlueOrbPickup : public AOrbPickupParent
{
	GENERATED_BODY()

public:
	
	ABlueOrbPickup();

	virtual void PutOrbInInventory() override;
	
};
