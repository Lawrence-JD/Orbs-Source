// Fill out your copyright notice in the Description page of Project Settings.


#include "RockPillarSpawningBox.h"
// Sets default values
ARockPillarSpawningBox::ARockPillarSpawningBox()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DummyRoot"));

	RootComponent = DummyRoot;

	SpawnZoneCollider = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpawnZoneCollider"));
	SpawnZoneCollider->SetupAttachment(DummyRoot);
	SpawnZoneCollider->ComponentTags.Add(FName("PillarSpawnZone"));

	SpawnZoneOutliner = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnZoneOutline"));
	SpawnZoneOutliner->SetupAttachment(SpawnZoneCollider);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MaterialCube(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	if (MaterialCube.Object)
	{
		SpawnZoneCollider->SetStaticMesh(MaterialCube.Object);
		SpawnZoneCollider->SetVisibility(false);
		SpawnZoneCollider->BodyInstance.SetCollisionProfileName("PillarSpawner");
	}

	SpawnZoneCollider->SetGenerateOverlapEvents(false);
	SpawnZoneOutliner->SetGenerateOverlapEvents(false);
	SpawnZoneCollider->BodyInstance.SetCollisionProfileName("PillarSpawnBox");


	SpawnZoneOutliner->SetWorldScale3D(FVector(1.56, 1.56, 1.56));
	SpawnZoneOutliner->AddRelativeLocation(FVector(0, 0, 50));
}

// Called when the game starts or when spawned
void ARockPillarSpawningBox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARockPillarSpawningBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

