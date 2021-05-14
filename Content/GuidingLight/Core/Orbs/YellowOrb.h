// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OrbParent.h"
#include "YellowOrb.generated.h"

/**
 * 
 */
UCLASS()
class GUIDINGLIGHT_API AYellowOrb : public AOrbParent
{
	GENERATED_BODY()
		AYellowOrb();

	UFUNCTION(BlueprintCallable, Category = "Activate")
	void TurnOffLight();
};
