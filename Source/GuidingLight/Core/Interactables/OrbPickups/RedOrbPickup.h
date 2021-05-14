// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Orbs/RedOrb.h"

#include "CoreMinimal.h"
#include "Interactables/OrbPickups/OrbPickupParent.h"
#include "RedOrbPickup.generated.h"

/**
 * 
 */
UCLASS()
class GUIDINGLIGHT_API ARedOrbPickup : public AOrbPickupParent
{
	GENERATED_BODY()

public:

	ARedOrbPickup();

	URedOrb* RedOrb;

	virtual void PutOrbInInventory() override;

};
