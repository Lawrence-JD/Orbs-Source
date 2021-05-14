// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HelperFiles/DefinedDebugHelpers.h"

#include "CrackedWall.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ElectricDoor.generated.h"

UCLASS()
class GUIDINGLIGHT_API AElectricDoor : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class USceneComponent* DummyRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* DoorFrame;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* Door;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* DoorCollider;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float MaxDoorHeight;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float MinDoorHeight;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float OpenSpeed;

	EDoorState State;

	// Sets default values for this actor's properties
	AElectricDoor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};

