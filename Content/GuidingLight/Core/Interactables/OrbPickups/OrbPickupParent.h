// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Orbs/OrbParent.h"
#include "Components/BoxComponent.h"
#include "HelperFiles/DefinedDebugHelpers.h"
#include "OrbPickupParent.generated.h"

/**
 * 
 */
UCLASS()
class GUIDINGLIGHT_API AOrbPickupParent : public AOrbParent
{
	GENERATED_BODY()

public:

	AOrbPickupParent();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Pickup, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* PickupCollisionBox;

	/* Sets the specific "Orb is in Inventory" boolean on the player to be true. The specific orb is decided by the children of this Actor */
	UFUNCTION()
	virtual void PutOrbInInventory();
	
};
