// Fill out your copyright notice in the Description page of Project Settings.


#include "ARotateHelper.h"

// Sets default values for this component's properties
UARotateHelper::UARotateHelper()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UARotateHelper::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UARotateHelper::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

FRotator UARotateHelper::RotatorCalculate(FRotator StartingRotation, FRotator RotateBy, float CurrentTimelineValue)
{
	float NewRoll = StartingRotation.Roll + (RotateBy.Roll * CurrentTimelineValue);
	float NewPitch = StartingRotation.Pitch + (RotateBy.Pitch * CurrentTimelineValue);
	float NewYaw = StartingRotation.Yaw + (RotateBy.Yaw * CurrentTimelineValue);

	FRotator NewRotation = FRotator(NewPitch, NewYaw, NewRoll);
	return NewRotation;
}