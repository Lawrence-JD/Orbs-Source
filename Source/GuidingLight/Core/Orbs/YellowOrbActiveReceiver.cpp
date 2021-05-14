// Fill out your copyright notice in the Description page of Project Settings.

#include "YellowOrbActiveReceiver.h"
#include "GameFramework/Actor.h"


// Sets default values for this component's properties
UYellowOrbActiveReceiver::UYellowOrbActiveReceiver()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	ReceivingEnergy = false;
	EnergyTickToBeUsed = false;
}


// Called when the game starts
void UYellowOrbActiveReceiver::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UYellowOrbActiveReceiver::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UYellowOrbActiveReceiver::ReceiveTickOfEnergy()
{
	EnergyTickToBeUsed = true;
}

void UYellowOrbActiveReceiver::TurnEnergyReceiptOn()
{
	ReceivingEnergy = true;
}

void UYellowOrbActiveReceiver::TurnEnergyReceiptOff()
{
	ReceivingEnergy = false;
}

bool UYellowOrbActiveReceiver::IsReceivingEnergy()
{
	if (EnergyTickToBeUsed)
	{
		EnergyTickToBeUsed = false;
		return true;
	}
	else
	{
		return ReceivingEnergy;
	}
}

FVector UYellowOrbActiveReceiver::GiveTargetLocation()
{
	return GetOwner()->GetActorLocation();
}

