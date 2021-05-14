// Fill out your copyright notice in the Description page of Project Settings.


#include "LaserReciever.h"

// Sets default values
ALaserReciever::ALaserReciever()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALaserReciever::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALaserReciever::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALaserReciever::SetHitState(bool State)
{
	IsHit = State;
}

bool ALaserReciever::GetHitState()
{
	return IsHit;
}

