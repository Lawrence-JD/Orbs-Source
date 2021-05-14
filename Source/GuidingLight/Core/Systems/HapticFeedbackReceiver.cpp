// Fill out your copyright notice in the Description page of Project Settings.


#include "HapticFeedbackReceiver.h"

// Sets default values for this component's properties
UHapticFeedbackReceiver::UHapticFeedbackReceiver()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHapticFeedbackReceiver::BeginPlay()
{
	Super::BeginPlay();
	// ...
	
}


// Called every frame
void UHapticFeedbackReceiver::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

void UHapticFeedbackReceiver::ReceiveHapticFeedback(ERumbleTypeEnum Type, float Intensity)
{
	RumbleTypeEnum = Type;
	CurrentIntensity = Intensity;
}

