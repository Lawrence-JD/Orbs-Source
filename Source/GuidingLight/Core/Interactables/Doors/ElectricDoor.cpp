// Fill out your copyright notice in the Description page of Project Settings.


#include "ElectricDoor.h"

// Sets default values
AElectricDoor::AElectricDoor()
{
	PrimaryActorTick.bCanEverTick = true;

	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DummyRoot"));

	RootComponent = DummyRoot;

	DoorFrame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorWay"));
	DoorFrame->SetupAttachment(DummyRoot);

	Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	Door->SetupAttachment(DummyRoot);

	Generator = CreateDefaultSubobject<UGenerator_YellowOrb>(TEXT("Generator"));
	//Generator->SetupAttachment(DummyRoot);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MaterialDoorFrame(TEXT("StaticMesh'/Game/GuidingLight/Environment/Props/Assets/P_SM_Door_OT_Frame_A.P_SM_Door_OT_Frame_A'"));
	if (MaterialDoorFrame.Object)
	{
		DoorFrame->SetStaticMesh(MaterialDoorFrame.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MaterialDoor(TEXT("StaticMesh'/Game/GuidingLight/Environment/Props/Assets/SM_P_Door_OT_A.SM_P_Door_OT_A'"));
	if (MaterialDoor.Object)
	{
		Door->SetStaticMesh(MaterialDoor.Object);
	}

	DoorCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("DoorCollider"));
	DoorCollider->SetupAttachment(DummyRoot);
	DoorCollider->SetWorldScale3D(FVector(1.75, 5, 6));
	DoorCollider->SetRelativeLocation(FVector(0, 0, 190));
	DoorCollider->SetRelativeRotation(FRotator(0, 90, 0));

	DoorCollider->OnComponentBeginOverlap.AddDynamic(this, &AElectricDoor::OnOverlapBegin);
	DoorCollider->OnComponentEndOverlap.AddDynamic(this, &AElectricDoor::OnOverlapEnd);

	OpenSpeed = 66.f;

	ChargeCap = 5;
}

	// Called when the game starts or when spawned
void AElectricDoor::BeginPlay()
{
	Super::BeginPlay();
	
	MaxDoorHeight = Door->GetRelativeTransform().GetTranslation();
	MinDoorHeight = MaxDoorHeight - (330.0f * GetActorUpVector());
}

void AElectricDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (State == EDoorState::Open || State == EDoorState::Closed)
	//	return;
	//
	//FVector Direction = Door->GetUpVector();
	//FVector Location = Door->GetRelativeTransform().GetTranslation();
	//
	//if (State == EDoorState::Opening)
	//{
	//	Location += (Direction * OpenSpeed * DeltaTime);
	//
	//	if (Location.Z >= MaxDoorHeight)
	//	{
	//		Location.Z = MaxDoorHeight;
	//		State = EDoorState::Open;
	//	}
	//}
	//else if (State == EDoorState::Closing)
	//{
	//	Location -= (Direction * OpenSpeed * DeltaTime);
	//
	//	if (Location.Z <= MinDoorHeight)
	//	{
	//		Location.Z = MinDoorHeight;
	//		State = EDoorState::Closed;
	//	}
	//}
	//
	//Door->SetRelativeLocation(Location);

	if (Generator->AmountOfCharge > ChargeCap)
	{
		Generator->AmountOfCharge = ChargeCap;
	}

	float OpenPercent = Generator->AmountOfCharge / ChargeCap;

	SCREENMSGF("OPEN PERCENT: ", OpenPercent);

	Door->SetRelativeLocation(FMath::Lerp(MaxDoorHeight, MinDoorHeight, OpenPercent));
}

void AElectricDoor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Other Actor is the actor that triggered the event. Check that is not ourself.  
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		
		TArray<UActorComponent*> OtherCollider = OtherActor->GetComponentsByTag(USphereComponent::StaticClass(), TEXT("Yellow"));
		if (OtherCollider.Num() > 0)
		{
			//if(State != EDoorState::Open)
			//	State = EDoorState::Opening;
		}
	}
}

UFUNCTION()
void AElectricDoor::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this))
	{
		
		TArray<UActorComponent*> OtherCollider = OtherActor->GetComponentsByTag(USphereComponent::StaticClass(), TEXT("Yellow"));
		if (OtherCollider.Num() > 0)
		{
			//TArray<AActor*> OverlappingActorList;
			//this->GetOverlappingActors(OverlappingActorList);
			//
			//int count = 0;
			//for (int i = 0; i < OverlappingActorList.Num(); i++)
			//	count += OverlappingActorList[i]->GetComponentsByTag(USphereComponent::StaticClass(), TEXT("Yellow")).Num();
			//
			//if (State != EDoorState::Closed && count <= 1)
			//	State = EDoorState::Closing;
		}
	}
}
