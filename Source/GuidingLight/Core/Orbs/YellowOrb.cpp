// Fill out your copyright notice in the Description page of Project Settings.


#include "YellowOrb.h"
 
UYellowOrb::UYellowOrb()
{
	V_LOG("YELLOW ORB START");
	//Allows for orb to be edited from the editor
	//this->bEditable = true;

	//Creating the Tags for the Yellow Orb
	OrbLightCollisionSphere->ComponentTags.Add(FName("Yellow"));
	DummyRoot->ComponentTags.Add(FName("Yellow"));

	//Setting up the values for the Orb Point Light
	OrbLight->SetLightColor(FLinearColor(1, 0.752942, 0.040915, 1.0));
	OrbLight->bUseTemperature = false;
	OrbLight->SetLightFalloffExponent(10000);
	OrbLight->SetIntensity(5000);
	OrbLight->SetAttenuationRadius(1500);
	OrbLight->SetShadowBias(1);


	OrbLightCollisionSphere->SetSphereRadius(600);
	OrbLightCollisionSphere->SetVisibility(true);

	//Set Orb Mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MaterialSphere(TEXT("StaticMesh'/Game/GuidingLight/Environment/Props/Assets/SM_P_YellowOrb.SM_P_YellowOrb'"));
	if (MaterialSphere.Object)
	{
		OrbMesh->SetStaticMesh(MaterialSphere.Object);
		FVector Scale = FVector(.75);
		OrbMesh->SetWorldScale3D(Scale);
	}
	
	OrbLightCollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &UYellowOrb::OnOverlapBegin);
	OrbLightCollisionSphere->OnComponentEndOverlap.AddDynamic(this, &UYellowOrb::OnOverlapEnd);
	V_LOG("YELLOW ORB END");
}

void UYellowOrb::BeginPlay()
{
	Super::BeginPlay();
}

void UYellowOrb::TurnOffLight()
{
	OrbLight->bVisible = false;
}

void UYellowOrb::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Other Actor is the actor that triggered the event. Check that is not ourself.  
	if ((OtherActor != nullptr) && (OtherComp != nullptr))
	{
		TArray<UActorComponent*> OtherCollider = OtherActor->GetComponentsByTag(USceneComponent::StaticClass(), TEXT("Hiding"));
		if (OtherCollider.Num() > 0)
		{
			//SCREENMSG("SHOW");
			TArray<UActorComponent*> OtherMeshComponents = OtherActor->GetComponentsByClass(UMeshComponent::StaticClass());
			for (int i = 0; i < OtherMeshComponents.Num(); i++)
			{
				((UMeshComponent*)OtherMeshComponents[i])->BodyInstance.SetCollisionProfileName("BlockAllDynamic");
			}
			OtherActor->SetActorHiddenInGame(false);
		}
	}
}

void UYellowOrb::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		TArray<UActorComponent*> OtherCollider = OtherActor->GetComponentsByTag(USceneComponent::StaticClass(), TEXT("Hiding"));
		if (OtherCollider.Num() > 0)
		{
			TArray<AActor*> OverlappingActorList;
			OtherActor->GetOverlappingActors(OverlappingActorList);

			int count = 0;
			for (int i = 0; i < OverlappingActorList.Num(); i++)
				count += OverlappingActorList[i]->GetComponentsByTag(USphereComponent::StaticClass(), TEXT("Yellow")).Num();

			SCREENMSGF("COUNT: ", count);
			if (count <= 1)
			{
				TArray<UActorComponent*> OtherMeshComponents = OtherActor->GetComponentsByClass(UMeshComponent::StaticClass());
				for (int i = 0; i < OtherMeshComponents.Num(); i++)
				{
					((UMeshComponent*)OtherMeshComponents[i])->BodyInstance.SetCollisionProfileName("Hidden");
				}
				OtherActor->SetActorHiddenInGame(true);
			}

		}
	}
}
