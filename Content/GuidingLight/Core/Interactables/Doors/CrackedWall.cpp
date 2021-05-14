// Fill out your copyright notice in the Description page of Project Settings.


#include "CrackedWall.h"

// Sets default values
ACrackedWall::ACrackedWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DummyRoot"));

	RootComponent = DummyRoot;
	
	Frame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Frame"));
	Frame->SetupAttachment(DummyRoot);

	LeftWall = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftWall"));
	LeftWall->SetupAttachment(DummyRoot);

	RightWall = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightWall"));
	RightWall->SetupAttachment(DummyRoot);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Material(TEXT("StaticMesh'/Game/GuidingLight/Environment/Props/Assets/SM_P_Rock_Door_Frame_A.SM_P_Rock_Door_Frame_A'"));
	if (Material.Object)
	{
		Frame->SetStaticMesh(Material.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MaterialCube(TEXT("StaticMesh'/Game/GuidingLight/Environment/Props/Assets/SM_P_Rock_Door_Slab_A.SM_P_Rock_Door_Slab_A'"));
	if (MaterialCube.Object)
	{
		LeftWall->SetStaticMesh(MaterialCube.Object);
		LeftWall->SetRelativeLocation(FVector(0, 0, -5));

		RightWall->SetStaticMesh(MaterialCube.Object);
		RightWall->SetRelativeLocation(FVector(0, 0, -5));
		RightWall->SetRelativeRotation(FRotator(0, 180, 0));

	}

	WallCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("WallCollider"));
	WallCollider->SetupAttachment(DummyRoot);
	WallCollider->SetWorldScale3D(FVector(1, 6.5, 6.5));
	WallCollider->SetRelativeLocation(FVector(0, 0, 200));
	WallCollider->SetRelativeRotation(FRotator(0, 90, 0));

	WallCollider->OnComponentBeginOverlap.AddDynamic(this, &ACrackedWall::OnOverlapBegin);
	WallCollider->OnComponentEndOverlap.AddDynamic(this, &ACrackedWall::OnOverlapEnd);

	TagToLookFor = TEXT("Red");

	OpenSpeed = 40.f;
}

void ACrackedWall::BeginPlay()
{
	Super::BeginPlay();
	MinDoorValue = Frame->GetComponentLocation().X;
	MaxDoorValue = MinDoorValue + 150.f;
}

void ACrackedWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (State == EDoorState::Open || State == EDoorState::Closed)
		return;

	FVector Direction = Frame->GetForwardVector();
	FVector LocationL = LeftWall->GetComponentLocation();
	FVector LocationR = RightWall->GetComponentLocation();

	if (State == EDoorState::Opening)
	{
		LocationL += (Direction * OpenSpeed * DeltaTime);
		LocationR -= (Direction * OpenSpeed * DeltaTime);

		if (LocationL.X >= MaxDoorValue)
		{
			State = EDoorState::Open;
		}
	}
	else if (State == EDoorState::Closing)
	{
		LocationL -= (Direction * OpenSpeed * DeltaTime);
		LocationR += (Direction * OpenSpeed * DeltaTime);

		if (LocationL.X <= MinDoorValue)
		{
			LocationL.X = MinDoorValue;
			LocationR.X = MinDoorValue;
			State = EDoorState::Closed;
		}
	}

	LeftWall->SetWorldLocation(LocationL);
	RightWall->SetWorldLocation(LocationR);
}

void ACrackedWall::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Other Actor is the actor that triggered the event. Check that is not ourself.  
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		TArray<UActorComponent*> OtherCollider = OtherActor->GetComponentsByTag(USphereComponent::StaticClass(), TagToLookFor);
		if (OtherCollider.Num() > 0)
		{
			if (State != EDoorState::Open)
				State = EDoorState::Opening;
		}
	}
}

UFUNCTION()
void ACrackedWall::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this))// && (OtherComp->ComponentTags.Find(FName("Red"))))
	{
		TArray<UActorComponent*> OtherCollider = OtherActor->GetComponentsByTag(USphereComponent::StaticClass(), TagToLookFor);
		if (OtherCollider.Num() > 0)
		{
			if (State != EDoorState::Closed)
				State = EDoorState::Closing;
		}
	}
}
