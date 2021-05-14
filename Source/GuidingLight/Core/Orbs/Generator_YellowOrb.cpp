// Fill out your copyright notice in the Description page of Project Settings.


#include "Generator_YellowOrb.h"
#include <string>
#include "HelperFiles/DefinedDebugHelpers.h"
#include "YellowOrbActiveReceiver.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
UGenerator_YellowOrb::UGenerator_YellowOrb()
{
	
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DummyRoot"));

	PrimaryComponentTick.bCanEverTick = true;

	GeneratorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GeneratorMesh"));
	
	GeneratorMesh->SetupAttachment(DummyRoot);
	
	
	MyReciever = CreateDefaultSubobject<UYellowOrbActiveReceiver>(TEXT("MyReciever"));
	YellowLightCollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("YellowLightCollisionSphere"));
	YellowLightCollisionSphere->SetupAttachment(GeneratorMesh);

	YellowLightCollisionSphere->SetSphereRadius(CollisionSphereSize);
	YellowLightCollisionSphere->SetVisibility(true);

	YellowLightCollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &UGenerator_YellowOrb::OnOverlapBegin);
	YellowLightCollisionSphere->OnComponentEndOverlap.AddDynamic(this, &UGenerator_YellowOrb::OnOverlapEnd);

	//static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("StaticMesh'/Game/Shapes/Shape_Cube.Shape_Cube'"));
	//if (Mesh.Object)
	//{
	//	GeneratorMesh->SetStaticMesh(Mesh.Object);
	//}
	yOrbCollision = false;

	AmountOfCharge = 0;
	PowerIncrement = 1;
	PowerDecrease = 1;

}

// Called when the game starts or when spawned
void UGenerator_YellowOrb::BeginPlay()
{
	Super::BeginPlay();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("GeginPlay has been called - Yellow_Generator"));
}


// Called every frame
void UGenerator_YellowOrb::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(AmountOfCharge));

	if (MyReciever->IsReceivingEnergy()) {
		AmountOfCharge += PowerIncrement * DeltaTime;
	}
	else
	{
		if (AmountOfCharge >= 1)
		{
			if (yOrbCollision!=true)
			{
				AmountOfCharge -= PowerDecrease * DeltaTime;
			}
		}
	}
}

bool UGenerator_YellowOrb::hasPower() {
	if (AmountOfCharge > 0) {
		return true;
	}
	else
	{
		return false;
	}
}
void UGenerator_YellowOrb::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Other Actor is the actor that triggered the event. Check that is not ourself.  
	if ((OtherActor != nullptr) && (OtherComp != nullptr))
	{
		TArray<UActorComponent*> OtherCollider = OtherActor->GetComponentsByTag(USceneComponent::StaticClass(), TEXT("Yellow"));
		
		if (OtherCollider.Num() > 0)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("overlap begin"));
			yOrbCollision = true;
		}
	}
}
void UGenerator_YellowOrb::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		
		TArray<UActorComponent*> OtherCollider = OtherActor->GetComponentsByTag(USceneComponent::StaticClass(), TEXT("Yellow"));
		if (OtherCollider.Num() > 0)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("overlap end"));
			yOrbCollision = false;
		}
	}
}