// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/Interactables/LampPosts/RetainingLampPost.h"
#include "SocketLampPost.generated.h"

/**
 * The empty socket lamp posts require that the player places an orb into 
 * them in order to produce a radial light. All other behavior is standard.
 */
UCLASS()
class GUIDINGLIGHT_API ASocketLampPost : public ARetainingLampPost
{
	GENERATED_BODY()
	
	ASocketLampPost();

public:
	/** Calls Parent Function to change Color based on Main Hand Color */
	virtual void ToggleColorChange() override;

	bool bOrbInSocket;

	/** Called when an instance of this class is placed (in editor) or spawned. */
	virtual void OnConstruction(const FTransform& Transform) override;
};
