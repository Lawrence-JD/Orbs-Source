// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OrbParent.h"
#include "YellowOrb.generated.h"

/**
 * 
 */
UCLASS()
class GUIDINGLIGHT_API UYellowOrb : public UOrbParent
{
public:
	GENERATED_BODY()
		UYellowOrb();

	UFUNCTION(BlueprintCallable, Category = "Activate")
	void TurnOffLight();

	virtual void BeginPlay() override;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
