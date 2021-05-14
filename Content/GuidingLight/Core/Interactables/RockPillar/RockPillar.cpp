// Fill out your copyright notice in the Description page of Project Settings.


#include "RockPillar.h"

// Sets default values
ARockPillar::ARockPillar()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MaxDeathTimer = 3.f;
	CurDeathTimer = MaxDeathTimer;

	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DummyRoot"));
	RootComponent = DummyRoot;

	Pillar = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PillarMesh"));
	Pillar->SetupAttachment(DummyRoot);

	PillarOverlapOutliner = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnZoneOutline"));
	PillarOverlapOutliner->SetupAttachment(Pillar);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MaterialCube(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	if (MaterialCube.Object)
	{
		Pillar->SetStaticMesh(MaterialCube.Object);
		Pillar->SetVisibility(false);
	}

	PillarOverlapOutliner->SetWorldScale3D(FVector(1.75, 1.75, 1.55));
	PillarOverlapOutliner->SetRelativeLocation(FVector(0, 0, 50));
	Pillar->SetWorldScale3D(FVector(1, 1, 3));

	Pillar->SetNotifyRigidBodyCollision(true);
	Pillar->BodyInstance.SetCollisionProfileName("BlockAllDynamic");

	PillarOverlapOutliner->OnComponentBeginOverlap.AddDynamic(this, &ARockPillar::OnPillarOverlapBegin);

	Pillar->OnComponentBeginOverlap.AddDynamic(this, &ARockPillar::OnOverlapBegin);
	Pillar->OnComponentEndOverlap.AddDynamic(this, &ARockPillar::OnOverlapEnd);

	this->Tags.Add(TEXT("RockPillar"));
	Pillar->ComponentTags.Add(TEXT("RockPillar"));
	PillarOverlapOutliner->ComponentTags.Add(TEXT("RockPillar"));

	State = EPillarState::Decaying;
	bDirty = true;
}

// Called when the game starts or when spawned
void ARockPillar::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ARockPillar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bDirty)
	{
		FVector Direction = GetActorUpVector();
		FVector Location = GetActorLocation() - (Direction * 5000 * DeltaTime);
		SetActorLocation(Location);
	}

	if (State == EPillarState::Living)
		CurDeathTimer = MaxDeathTimer;

	else if (State == EPillarState::Decaying)
	{
		CurDeathTimer -= DeltaTime;

		if (CurDeathTimer <= 0)
			State = EPillarState::Dead;
	}

	if (State == EPillarState::Dead)
		Destroy();

}

void ARockPillar::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Other Actor is the actor that triggered the event. Check that is not ourself.  
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{

		//SCREENMSG("OVERLAP BEGIN");		
		TArray<UActorComponent*> RedOrbCheck = OtherActor->GetComponentsByTag(USphereComponent::StaticClass(), TEXT("Red"));

		if (RedOrbCheck.Num() > 0)
		{
			State = EPillarState::Living;
		}
	}
}

void ARockPillar::OnPillarOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Other Actor is the actor that triggered the event. Check that is not ourself.  
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		//SCREENMSG("DYING");

		TArray<UActorComponent*> RedOrbCheck = OtherActor->GetComponentsByTag(USphereComponent::StaticClass(), TEXT("Red"));
		if (RedOrbCheck.Num() > 0)
		{
			return;
		}

		TArray<UActorComponent*> PillarOverlapCheck = OtherActor->GetComponentsByTag(UBoxComponent::StaticClass(), TEXT("RockPillar"));
		if (PillarOverlapCheck.Num() > 0 && !bDirty)// && State == EPillarState::Forming)
		{
			//SCREENMSG("DIE");
			State = EPillarState::Dead;

			return;
		}
		else if (PillarOverlapCheck.Num() <= 0)
		{
			bDirty = false;
			Pillar->SetVisibility(true);
		}
	}
}

UFUNCTION()
void ARockPillar::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this))
	{
		TArray<UActorComponent*> RedOrbCheck = OtherActor->GetComponentsByTag(USphereComponent::StaticClass(), TEXT("Red"));
		if (RedOrbCheck.Num() > 0)
		{
			State = EPillarState::Decaying;
		}
	}
}

