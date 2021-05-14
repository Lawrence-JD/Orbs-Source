// Fill out your copyright notice in the Description page of Project Settings.


#include "Doppelganger.h"

// Sets default values
ADoppelganger::ADoppelganger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DoppelgangerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoppelgangerMesh"));
	DoppelgangerMesh->SetGenerateOverlapEvents(true);
	DoppelgangerMesh->SetMobility(EComponentMobility::Movable);

	//Set Doppelganger Mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> DoppelgangerMaterialMesh(TEXT("StaticMesh'/Game/GuidingLight/TestStuff/ShaderStuff/ParticleEffects/TeleportEffect/SM_BlinkReticleCanFire.SM_BlinkReticleCanFire'"));
	if (DoppelgangerMaterialMesh.Object)
	{
		DoppelgangerMesh->SetStaticMesh(DoppelgangerMaterialMesh.Object);
	}

	RootComponent = DoppelgangerMesh;

	BlueSphere = CreateDefaultSubobject<USphereComponent>(TEXT("BlueSphere"));
	BlueSphere->SetupAttachment(DoppelgangerMesh);

	BlueSphere->SetSphereRadius(550);
	BlueSphere->ComponentTags.Add(FName("Blue"));
	BlueSphere->SetVisibility(true);
	BlueSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BlueSphere->SetGenerateOverlapEvents(true);

	DoppelgangerMesh->SetCollisionObjectType(ECC_PhysicsBody);
	BlueSphere->SetCollisionObjectType(ECC_Visibility);

	DoppelgangerLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("DoppelgangerLight"));
	DoppelgangerLight->SetupAttachment(DoppelgangerMesh);

	//Setting up the values for the Orb Point Light (Copied from Blue Orb)
	DoppelgangerLight->SetLightColor(FLinearColor(0.5, 1, 0.99, 1.0));
	DoppelgangerLight->bUseTemperature = false;
	DoppelgangerLight->SetLightFalloffExponent(10000);
	DoppelgangerLight->SetIntensity(17000);
	DoppelgangerLight->SetAttenuationRadius(750);
	DoppelgangerLight->SetShadowBias(1);


	DoppelgangerMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("DoppelgangerMovementComp"));
	DoppelgangerMovementComp->bShouldBounce = true;
	DoppelgangerMovementComp->Bounciness = 0.3;
	DoppelgangerMovementComp->ProjectileGravityScale = 1.2;
	DoppelgangerMovementComp->Friction = 0.9;
	DoppelgangerMovementComp->UpdatedComponent = DoppelgangerMesh;
	DoppelgangerMovementComp->InitialSpeed = 1250.0f;
	DoppelgangerMovementComp->MaxSpeed = 1250.0f;
	DoppelgangerMovementComp->bRotationFollowsVelocity = false;

	InitialLifeSpan = 3.0f;

	BlueSphere->OnComponentBeginOverlap.AddDynamic(this, &ADoppelganger::OnOverlapBegin);
	BlueSphere->OnComponentEndOverlap.AddDynamic(this, &ADoppelganger::OnOverlapEnd);

}

// Called when the game starts or when spawned
void ADoppelganger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADoppelganger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADoppelganger::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GLog->Log(OtherActor->GetName());
	// Other Actor is the actor that triggered the event. Check that is not ourself.  
	if ((OtherActor != nullptr) && (OtherComp != nullptr))
	{
		TArray<UActorComponent*> OtherCollider = OtherActor->GetComponentsByTag(USceneComponent::StaticClass(), TEXT("Spoopy"));
		if (OtherCollider.Num() > 0)
		{
			TArray<UActorComponent*> OtherMeshComponents = OtherActor->GetComponentsByClass(UMeshComponent::StaticClass());
			for (int i = 0; i < OtherMeshComponents.Num(); i++)
			{
				((UMeshComponent*)OtherMeshComponents[i])->BodyInstance.SetCollisionProfileName("Ghosted");
				((UMeshComponent*)OtherMeshComponents[i])->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Ignore);
			}
		}
	}
}

void ADoppelganger::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		TArray<UActorComponent*> OtherCollider = OtherActor->GetComponentsByTag(USceneComponent::StaticClass(), TEXT("Spoopy"));
		if (OtherCollider.Num() > 0)
		{
			TArray<AActor*> OverlappingActorList;
			OtherActor->GetOverlappingActors(OverlappingActorList);

			int count = 0;
			for (int i = 0; i < OverlappingActorList.Num(); i++)
				count += OverlappingActorList[i]->GetComponentsByTag(USphereComponent::StaticClass(), TEXT("Blue")).Num();

			if (count < 1)
			{
				TArray<UActorComponent*> OtherMeshComponents = OtherActor->GetComponentsByClass(UMeshComponent::StaticClass());
				for (int i = 0; i < OtherMeshComponents.Num(); i++)
				{
					((UMeshComponent*)OtherMeshComponents[i])->BodyInstance.SetCollisionProfileName("BlockAllDynamic");
					((UMeshComponent*)OtherMeshComponents[i])->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Block);
				}
			}

		}
	}
}