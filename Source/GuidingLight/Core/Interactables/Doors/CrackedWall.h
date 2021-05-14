// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/StaticMesh.h"

#include "UObject/ConstructorHelpers.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CrackedWall.generated.h"

UENUM(BlueprintType)
enum class EDoorState : uint8
{
	Closed UMETA(DisplayName = "Closed"),
	Closing	UMETA(DisplayName = "Closing"),
	Opening UMETA(DisplayName = "Opening"),
	Open	UMETA(DisplayName = "Open"),
};

UCLASS()
class GUIDINGLIGHT_API ACrackedWall : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class USceneComponent* DummyRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* Frame;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* LeftWall;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* RightWall;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* WallCollider;

	// Sets default values for this actor's properties
	ACrackedWall();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FName TagToLookFor;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float MaxDoorValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float MinDoorValue;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float OpenSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	EDoorState State;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
