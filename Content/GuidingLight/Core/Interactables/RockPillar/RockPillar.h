// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HelperFiles/DefinedDebugHelpers.h"

#include "UObject/ConstructorHelpers.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"

#include "GameplayTagContainer.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RockPillar.generated.h"

UENUM()
enum class EPillarState
{
	Forming UMETA(DisplayName = "Forming"),
	Living	UMETA(DisplayName = "Living"),
	Decaying UMETA(DisplayName = "Decaying"),
	Dead	UMETA(DisplayName = "Dead"),
};


UCLASS()
class GUIDINGLIGHT_API ARockPillar : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARockPillar();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class USceneComponent* DummyRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* Pillar;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* PillarOverlapOutliner;

	float CurDeathTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float MaxDeathTimer;

	EPillarState State;
	float SpawnBias;
	bool bDirty;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE bool operator<(const ARockPillar& Other) const
	{
		if (this && &Other)
			return CurDeathTimer < Other.CurDeathTimer;
		else
			return false;
	}


	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnPillarOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
