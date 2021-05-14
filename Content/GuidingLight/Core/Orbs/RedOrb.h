// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Interactables/RockPillar/RockPillar.h"

#include "Math/UnrealMathUtility.h"

#include "CoreMinimal.h"
#include "OrbParent.h"
#include "RedOrb.generated.h"

UCLASS()
class GUIDINGLIGHT_API ARedOrb : public AOrbParent
{
	GENERATED_BODY()
		ARedOrb();
public:
	UFUNCTION(BlueprintCallable, Category = "AimActive")
		void AimActive();

	UFUNCTION(BlueprintCallable, Category = "FireActive")
		void FireActive();

	virtual void BeginPlay() override;

	void Tick(float DeltaTime);

	FHitResult* ActiveHitResult;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float ActiveMaxRange;

	TArray<ARockPillar*> RockPillars;
	UStaticMeshComponent* Reticle;
	UMaterial* ReticleMat;
	UStaticMesh* ReticleMesh;
	FVector* ReticleDestination;
	FRotator* ReticleRotation;
	bool bReticleFlag;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int MaxPillars;
};
