// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/ConstructorHelpers.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RockPillarSpawningBox.generated.h"

UCLASS()
class GUIDINGLIGHT_API ARockPillarSpawningBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARockPillarSpawningBox();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class USceneComponent* DummyRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* SpawnZoneCollider;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* SpawnZoneOutliner; 

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
