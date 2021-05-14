// Fill out your copyright notice in the Description page of Project Settings.


#include "OrbParent.h"
#include "Core/Crystal/GuidingLightCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
UOrbParent::UOrbParent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//V_LOG("ORB PARENT START");
	// Setting up all of the components and where they're attached to
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DummyRoot"));
	
	OrbMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OrbMesh"));
	OrbMesh->SetupAttachment(DummyRoot);
	OrbMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	OrbMesh->SetCastShadow(false);
	
	
	OrbLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("OrbLight"));
	OrbLight->SetupAttachment(DummyRoot);
	
	OrbParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("OrbParticles"));
	OrbParticles->SetupAttachment(DummyRoot);
	
	OrbLightCollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("OrbLightCollisionSphere"));
	OrbLightCollisionSphere->SetupAttachment(OrbLight);
	
	OrbMaterial = CreateDefaultSubobject<UMaterial>(TEXT("OrbMaterial"));
	
	bActiveUsed = false;
	//V_LOG("ORB PARENT END");
}

// Called when the game starts or when spawned
void UOrbParent::BeginPlay()
{
	Super::BeginPlay();
	if (GetWorld())
	{
		Crystal = Cast<AGuidingLightCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	}
}
// Called every frame
void UOrbParent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UOrbParent::OrbActivate()
{
	if(OrbMesh)
		OrbMesh->SetHiddenInGame(false);
	OrbLight->SetHiddenInGame(false);
	OrbLightCollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	bIsActive = true;
	//this->SetHiddenInGame(false);
	//this->SetActorHiddenInGame(false);
	//this->SetActorEnableCollision(true);
}

void UOrbParent::OrbDeactivate()
{
	IsAiming = EAimingState::NotAiming;
	//this->SetHiddenInGame(true);
	if(OrbMesh)
		OrbMesh->SetHiddenInGame(true);
	OrbLight->SetHiddenInGame(true);
	OrbLightCollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bIsActive = false;
	//this->SetActorHiddenInGame(true);
	//this->SetActorEnableCollision(false);
}

