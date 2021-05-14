// Fill out your copyright notice in the Description page of Project Settings.


#include "OrbParent.h"
#include "Core/Crystal/GuidingLightCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AOrbParent::AOrbParent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// Setting up all of the components and where they're attached to
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DummyRoot"));

	RootComponent = DummyRoot;

	OrbMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OrbMesh"));
	OrbMesh->SetupAttachment(DummyRoot);
	OrbMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	

	OrbLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("OrbLight"));
	OrbLight->SetupAttachment(DummyRoot);
	
	OrbParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("OrbParticles"));
	OrbParticles->SetupAttachment(DummyRoot);

	OrbLightCollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("OrbLightCollisionSphere"));
	OrbLightCollisionSphere->SetupAttachment(OrbLight);

	OrbMaterial = CreateDefaultSubobject<UMaterial>(TEXT("OrbMaterial"));
}

// Called when the game starts or when spawned
void AOrbParent::BeginPlay()
{
	Super::BeginPlay();
	if (GetWorld())
	{
		Crystal = Cast<AGuidingLightCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	}
}
// Called every frame
void AOrbParent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AOrbParent::OrbActivate()
{
	if(OrbMesh)
		OrbMesh->SetHiddenInGame(false);
	OrbLight->SetHiddenInGame(false);
	OrbLightCollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	
	//this->SetActorHiddenInGame(false);
	//this->SetActorEnableCollision(true);
}

void AOrbParent::OrbDeactivate()
{
	if(OrbMesh)
		OrbMesh->SetHiddenInGame(true);
	OrbLight->SetHiddenInGame(true);
	OrbLightCollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//this->SetActorHiddenInGame(true);
	//this->SetActorEnableCollision(false);
}

