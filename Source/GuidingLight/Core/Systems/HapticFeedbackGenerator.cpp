// Fill out your copyright notice in the Description page of Project Settings.

#include "HapticFeedbackGenerator.h"

// Sets default values for this component's properties
UHapticFeedbackGenerator::UHapticFeedbackGenerator()
{
	PlayerCharacter = NULL;
	EffectRaidus = 50;
	bDoesRumbleFallOff = false;
	RumbleTypeEnum = ERumbleTypeEnum::RTE_High;
	bGeneratorIsActive = false;
	AttemptToFindPlayer();
}

// Sets non-default values for component variables
UHapticFeedbackGenerator::UHapticFeedbackGenerator(float Radius, bool FallsOff, ERumbleTypeEnum RumbleType, bool IsActive) :
	EffectRaidus(Radius), bDoesRumbleFallOff(FallsOff), RumbleTypeEnum(RumbleType), bGeneratorIsActive(IsActive)
{
	PrimaryComponentTick.bCanEverTick = true;
	PlayerCharacter = NULL;
	AttemptToFindPlayer();
}

// Lets caller know if the generator is active
bool UHapticFeedbackGenerator::IsGeneratorActive()
{
	return bIsActive;
}

// Lets caller turn the rumble generator on or off
void UHapticFeedbackGenerator::SetRumbleActive(bool IsActive)
{
	bGeneratorIsActive = IsActive;
}


// Called when the game starts
void UHapticFeedbackGenerator::BeginPlay()
{
	Super::BeginPlay();


}

// Looks through all the actors in the world to try and find a GuidingLightCharacter Actor
// And then points PlayerActor to it
void UHapticFeedbackGenerator::AttemptToFindPlayer()
{
	PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
}


// Called every frame
void UHapticFeedbackGenerator::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PlayerCharacter == NULL)
	{
		// If the player wasn't found when running the constructor,
		// Generator tries every tick to find them again
		// Until they're found
		AttemptToFindPlayer();
	}
	else
	{
		// Only sends signal if the generator is active
		if (IsGeneratorActive())
		{
			float DistanceFromPlayer = FVector::Dist(GetOwner()->GetActorLocation(), PlayerCharacter->GetActorLocation());
			if (DistanceFromPlayer <= EffectRaidus)
			{
				// Set intensity for non-falling-off generation
				float intensity = 1;
				if (bDoesRumbleFallOff)
				{
					// TODO: (Possibly) Give fall-off options that aren't linear
					// Changes intensity for falling off generator rumble based
					// on distance of player from generator
					intensity = (EffectRaidus - DistanceFromPlayer) / DistanceFromPlayer;
				}
				// Make a Call to HapticFeedbackReceiver in player
				// (AGuidingLightCharacter *)PlayerActor.PlayRumble(RumbleType, intensity);
			}
		}
	}
}