// Fill out your copyright notice in the Description page of Project Settings.


#include "Generator_YellowOrb.h"

// Sets default values
AGenerator_YellowOrb::AGenerator_YellowOrb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GeneratorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GeneratorMesh"));

}

// Called when the game starts or when spawned
void AGenerator_YellowOrb::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGenerator_YellowOrb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

