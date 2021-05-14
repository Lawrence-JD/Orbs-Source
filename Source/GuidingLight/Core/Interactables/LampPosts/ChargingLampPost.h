// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/Interactables/LampPosts/RetainingLampPost.h"
#include "ChargingLampPost.generated.h"

/**
 * This lamp post behaves the same as the standard, however they do not take 
 * their light immediately nor do they keep it forever. These lamp posts must 
 * be charged, with a visual to indicate the charge level. After the source 
 * light leaves, the light in these lamp posts will slowly fade as the charge 
 * depletes.
 */
UCLASS()
class GUIDINGLIGHT_API AChargingLampPost : public ARetainingLampPost
{
	GENERATED_BODY()

	AChargingLampPost();

public:
	// 0 - 100 Charge of Light 
	float ChargePercentage;

	// How fast Charge Depletes/Charges
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "A\|State")
	float Speed;

	// Is Active Button Pressed
	bool bIsCharging;

	/** Calls Parent Function to change Color based on Main Hand Color */
	virtual void ToggleColorChange() override;

	void ReleaseCharge();

	/** Called when an instance of this class is placed (in editor) or spawned. */
	virtual void OnConstruction(const FTransform& Transform) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
