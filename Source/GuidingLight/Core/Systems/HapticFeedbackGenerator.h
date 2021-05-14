// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Core/Crystal/GuidingLightCharacter.h"
#include "Math/Vector.h"
#include "GuidingLightEnums.h"
#include "HapticFeedbackGenerator.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GUIDINGLIGHT_API UHapticFeedbackGenerator : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHapticFeedbackGenerator();
	UHapticFeedbackGenerator(float Radius, bool FallsOff, ERumbleTypeEnum RumbleType, bool IsActive);

protected:
	UPROPERTY(EditAnywhere, Category = Enum)
		ERumbleTypeEnum RumbleTypeEnum;

	// Distance in game units that the rumble can be felt
	UPROPERTY(EditAnywhere)
		float EffectRaidus;
	// Does the rumble effect lessen the further away you get from the generator
	UPROPERTY(EditAnywhere)
		bool bDoesRumbleFallOff;
	// Is generator currently aqtive
	UPROPERTY(EditAnywhere)
		bool bGeneratorIsActive;
	// Pointer to the player that will be used to call rumble receiving function when
	// player is close enough and the generator is active
	ACharacter* PlayerCharacter;
	// Called when the game starts
	// Looks through all the actors in the world to try and find a GuidingLightCharacter Actor
	// And then points PlayerActor to it
	void AttemptToFindPlayer();
	virtual void BeginPlay() override;

public:
	// Lets caller know if the generator is active
	bool IsGeneratorActive();
	// Lets caller turn the rumble on or off
	void SetRumbleActive(bool IsActive);

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
