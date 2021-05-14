// Fill out your copyright notice in the Description page of Project Settings.


#include "LaserBeam.h"
#include "Kismet/KismetMathLibrary.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
ALaserBeam::ALaserBeam()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DummyRoot"));
	RootComponent = DummyRoot;

	LaserMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PillarMesh"));
	LaserMesh->SetupAttachment(DummyRoot);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cylinder.Shape_Cylinder'"));
	if (Cube.Object)
	{
		LaserMesh->SetStaticMesh(Cube.Object);
		LaserMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

// Called when the game starts or when spawned
void ALaserBeam::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALaserBeam::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALaserBeam::SetMeshComponent(UStaticMeshComponent* Mesh)
{
	;// LaserMesh = Mesh;
}

void ALaserBeam::SetEnds(FVector Origin, FVector End, FVector Normal)
{
	//Set ends takes two vectors, and origin and an end point vector, and it make stretch a 1M cylinder to the correct size
	//Note: The 1M cylinder is important for the scale since the origin is at the center-bottom, avoiding problems with the laser
	//going behind the origin point.

	FVector DeltaVec = End - Origin; //gets the delta vector between the start and endpoints - used for direction/distance
	this->SetActorLocationAndRotation(Origin, UKismetMathLibrary::MakeRotFromZ(Normal)); //sets location/rotation of the root component
	FVector ScaleVec(0.05, 0.05, DeltaVec.Size() / 100.0);  //scale of the laserbeam (in centimeters)
	SetActorScale3D(ScaleVec);
	SetActorHiddenInGame(false);
	//SetMeshVisible(true); //Sets the visibility of the laser mesh to true (avoids graphical issues to keep it hidden until now)
}

void ALaserBeam::SetMeshVisible(bool Visibility)
{
	LaserMesh->SetVisibility(Visibility, true);
}

void ALaserBeam::SetMaterial(UMaterialInterface* Mat)
{
	LaserMesh->SetMaterial(0, Mat);
}

