// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/Interactables/LampPosts/LampPost.h"
#include "Components/SpotLightComponent.h"
#include "AmplifierLampPost.generated.h"

/**
 * An amplifier (or AMP) is an Empty Socket lamp post which produces a focused 
 * light beam instead of a radial light. These are used in conjunction with
 * mirror systems to carry light effects to places the player can’t reach. 
 * Unlike other lamp posts, AMPs can be rotated about a fixed axis.
 */
UCLASS()
class GUIDINGLIGHT_API AAmplifierLampPost : public ALampPost
{
	GENERATED_BODY()
	
public:
	AAmplifierLampPost();

	/** This should be a Spot Light Component */
	UPROPERTY(VisibleAnywhere, Category = "A\|State")
	USpotLightComponent * LightComp;

	/** Degree Increments*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "A\|State")
	float TurnDegrees;

	/** The desired Intensity for the light */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "A\|State")
	float LightIntensity;

	/** The desired Radius for the Collision Capsule */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "A\|State")
	float CapsuleRadius;

	/** Turn to the next stage of AMP */
	void TurnAmp();
	
	/** Vector Direction of Actor */
	FVector Direction;

	bool bIsTurning;
	bool bOrbInSocket;

	/** Calls Parent Function to change Color based on Main Hand Color */
	virtual void ToggleColorChange() override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

protected:
	/** Called when an instance of this class is placed (in editor) or spawned. */
	virtual void OnConstruction(const FTransform& Transform) override;
};
