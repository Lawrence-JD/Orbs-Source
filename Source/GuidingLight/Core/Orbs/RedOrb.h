// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Interactables/RockPillar/RockPillar.h"

#include "Math/UnrealMathUtility.h"

#include "CoreMinimal.h"
#include "OrbParent.h"
#include "RedOrb.generated.h"

UCLASS()
class GUIDINGLIGHT_API URedOrb : public UOrbParent
{
public:
	GENERATED_BODY()
		URedOrb();

	UFUNCTION(BlueprintCallable, Category = "AimActive")
		void AimActive();

	UFUNCTION(BlueprintCallable, Category = "FireActive")
		void FireActive();

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FHitResult* ActiveHitResult;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float ActiveMaxRange;

	void OrbDeactivate();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<ARockPillar*> RockPillars;
	
	UStaticMeshComponent* Reticle;
	UMaterial* ReticleMat;
	UStaticMesh* ReticleMesh;
	FVector* ReticleDestination;
	FRotator* ReticleRotation;
	bool bReticleFlag;
	int ReticleOffset;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int MaxPillars;
};
