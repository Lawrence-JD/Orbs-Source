// Fill out your copyright notice in the Description page of Project Settings.

#include "LampPost.h"
#include "Core/Crystal/GuidingLightCharacter.h"
#include "HelperFiles/DefinedDebugHelpers.h"


// Sets default values
ALampPost::ALampPost()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	CollisionComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision Comp"));
	CollisionComp->InitCapsuleSize(55.f, 96.f);
	CollisionComp->SetCollisionProfileName(TEXT("Trigger"));
	CollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	RootComponent = MeshComp;
	CollisionComp->SetupAttachment(MeshComp);

	ColorEffectCollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ColorEffectCollisionSphere"));
	ColorEffectCollisionSphere->SetupAttachment(MeshComp);
	ColorEffectCollisionSphere->SetSphereRadius(550);
	CurrentLightState = EColorState::Empty;
	ColorEffectCollisionSphere->ComponentTags.Add(FName(*GETENUMSTRING("EColorState", CurrentLightState)));

	ColorEffectCollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ALampPost::OnColorOverlapBegin);
	ColorEffectCollisionSphere->OnComponentEndOverlap.AddDynamic(this, &ALampPost::OnColorOverlapEnd);
}


// Called when the game starts or when spawned
void ALampPost::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ALampPost::OnOverlapBegin);
	CollisionComp->OnComponentEndOverlap.AddDynamic(this, &ALampPost::OnOverlapEnd);

	PlayerCharacter = Cast<AGuidingLightCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

}

// Called every frame
void ALampPost::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ALampPost::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{	
	if (OtherActor && (OtherActor != this) && OtherComp && OtherActor->GetClass()->IsChildOf(AGuidingLightCharacter::StaticClass()))
	{
	//	PlayerController = Cast<ALampPost>(OtherActor);
	}
	bPlayerInRange = true;
}

void ALampPost::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bPlayerInRange = false;

}

// Changes the Color to Current Main Hand Orb Color
void ALampPost::ToggleColorChange()
{
	
	if (bPlayerInRange)
	{
		// If Main Hand Light does not equal Lamp Post Light State
		if (GETENUMSTRING("EMainHandColorState", PlayerCharacter->CurrentMainHand) != GETENUMSTRING("EColorState", CurrentLightState))
		{	
			ColorChangedStart();
			ColorEffectCollisionSphere->ComponentTags.Remove(FName(*GETENUMSTRING("EColorState", CurrentLightState)));
			//ColorEffectCollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			// Change Lamp Post Light to Main Hand Light
			switch (PlayerCharacter->CurrentMainHand)
			{
				case EMainHandColorState::Yellow:
					CurrentLightState = EColorState::Yellow;
					bIsLightOn = true;
					break;
				case EMainHandColorState::Blue:
					CurrentLightState = EColorState::Blue;
					bIsLightOn = true;
					break;
				case EMainHandColorState::Red:
					CurrentLightState = EColorState::Red;
					bIsLightOn = true;
					break;
				case EMainHandColorState::Empty:
					CurrentLightState = EColorState::Empty;
					bIsLightOn = false;
					break;
			}
			//ColorEffectCollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			ColorEffectCollisionSphere->ComponentTags.Add(FName(*GETENUMSTRING("EColorState", CurrentLightState)));
			ColorChangedEnd();
		}
		
		
	}
	
}

void ALampPost::ChangeColor(EColorState NewColor)
{
	CurrentLightState = NewColor;
}

void ALampPost::OnColorOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Other Actor is the actor that triggered the event. Check that is not ourself.  
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		TArray<UActorComponent*> OtherCollider = OtherActor->GetComponentsByTag(USceneComponent::StaticClass(), TEXT("Spoopy"));
		if (OtherCollider.Num() && CurrentLightState == EColorState::Blue)
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

UFUNCTION()
void ALampPost::OnColorOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this))
	{
		TArray<UActorComponent*> OtherCollider = OtherActor->GetComponentsByTag(USceneComponent::StaticClass(), TEXT("Spoopy"));
		if (OtherCollider.Num() && CurrentLightState != EColorState::Blue)
		{
			TArray<AActor*> OverlappingActorList;
			OtherActor->GetOverlappingActors(OverlappingActorList);
			int count = 0;
			for (int i = 0; i < OverlappingActorList.Num(); i++)
				count += OverlappingActorList[i]->GetComponentsByTag(USphereComponent::StaticClass(), TEXT("Blue")).Num();
			if (count <= 1)
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
