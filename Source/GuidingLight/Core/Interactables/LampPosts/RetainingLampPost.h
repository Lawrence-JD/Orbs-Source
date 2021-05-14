// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/Interactables/LampPosts/LampPost.h"
#include "Components/PointLightComponent.h"
#include "RetainingLampPost.generated.h"

/**
 * The default lamp post. They can start empty or with a predetermined light. 
 * Upon receiving a new light, they switch to it and never change until a new 
 * light is cast into it.
 */
UCLASS()
class GUIDINGLIGHT_API ARetainingLampPost : public ALampPost
{
	GENERATED_BODY()
	
public:
	ARetainingLampPost();

	/** This is the light the orb emits */
	UPROPERTY(VisibleAnywhere, Category = "A\|State")
	UPointLightComponent* LightComp;

	/** The desired Intensity for the light */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "A\|State")
	float LightIntensity;

	/** The desired Radius for the Collision Capsule */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "A\|State")
	float CapsuleRadius;

	struct FLinearColor SavedColor;
	
	/** booleans to determine when it is charging */
	bool bIsCharging;
	bool bIsDischarging;
	bool bchargePrep;
	
	/** floats to determine current charge and rate of charge */
	float ChargePercentage;
	float Speed;

	/** Calls Parent Function to change Color based on Main Hand Color */
	UFUNCTION(BlueprintCallable)
	virtual void ToggleColorChange() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	/** Called when an instance of this class is placed (in editor) or spawned. */
	virtual void OnConstruction(const FTransform& Transform) override;
};
