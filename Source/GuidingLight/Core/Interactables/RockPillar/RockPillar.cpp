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

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MaterialCube(TEXT("StaticMesh'/Game/GuidingLight/Environment/Props/Assets/SM_P_Red_Orb_Pillar_A.SM_P_Red_Orb_Pillar_A'"));
	if (MaterialCube.Object)
	{
		Pillar->SetStaticMesh(MaterialCube.Object);
	}

	PillarOverlapOutliner->SetWorldScale3D(FVector(1.75, 1.75, 1.55));
	PillarOverlapOutliner->SetRelativeLocation(FVector(0, 0, 50));
	
	//Pillar->SetWorldScale3D(FVector(1, 1, 3));
	FormingDistance = 300;

	Pillar->SetNotifyRigidBodyCollision(true);
	Pillar->BodyInstance.SetCollisionProfileName("BlockAllDynamic");

	PillarOverlapOutliner->OnComponentBeginOverlap.AddDynamic(this, &ARockPillar::OnPillarOverlapBegin);

	Pillar->OnComponentBeginOverlap.AddDynamic(this, &ARockPillar::OnOverlapBegin);
	Pillar->OnComponentEndOverlap.AddDynamic(this, &ARockPillar::OnOverlapEnd);

	this->Tags.Add(TEXT("RockPillar"));
	Pillar->ComponentTags.Add(TEXT("RockPillar"));
	PillarOverlapOutliner->ComponentTags.Add(TEXT("RockPillar"));

	State = EPillarState::Decaying;

	FVector Direction = GetActorUpVector();
	FVector Location = GetActorLocation() - (Direction * 320);
	SetActorLocation(Location);

	Dirty = 0;
}

// Called when the game starts or when spawned
void ARockPillar::BeginPlay()
{
	Super::BeginPlay();

	Shake_x = FMath::RandRange(-2, 2);
	Shake_y = FMath::RandRange(-2, 2);
	ShakeFactor = -1;
}

// Called every frame
void ARockPillar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (FormingDistance > 0)
	{
		FVector Direction = GetActorUpVector();
		float dist = 100 * DeltaTime;

		FVector Location = GetActorLocation() + (Direction * dist);
		Location.Set(Location.X + (Shake_x * ShakeFactor), Location.Y + (Shake_y * ShakeFactor), Location.Z);
		FormingDistance -= dist;
		SetActorLocation(Location);
		ShakeFactor *= -1;

		if (((int)FormingDistance % 2)== 0)
		{
			Shake_x = FMath::RandRange(-2, 2);
			Shake_y = FMath::RandRange(-2, 2);
		}
	}

	if (Dirty == 1)
	{
		TArray<AActor*> OverlappingActorList;
		this->GetOverlappingActors(OverlappingActorList);

		int count = 0;
		int num = OverlappingActorList.Num();
		for (int i = 0; i < num; i++)
			count += OverlappingActorList[i]->GetComponentsByTag(USphereComponent::StaticClass(), TEXT("Red")).Num();

		if (count < 1)
			State = EPillarState::Decaying;

		Dirty = 0;
	}

	if (--Dirty < 0)
		Dirty = 0;

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
		if (PillarOverlapCheck.Num() > 0)// && State == EPillarState::Forming)
		{
			//SCREENMSG("DIE");
			State = EPillarState::Dead;

			return;
		}
		//else if (PillarOverlapCheck.Num() <= 0)
		//{
		//	bDirty = false;
		//	Pillar->SetVisibility(true);
		//}
	}
}

void ARockPillar::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this))
	{
		TArray<UActorComponent*> RedOrbCheck = OtherActor->GetComponentsByTag(USphereComponent::StaticClass(), TEXT("Red"));
		if (RedOrbCheck.Num() > 0)
		{
			Dirty = 10;
		}
	}
}

