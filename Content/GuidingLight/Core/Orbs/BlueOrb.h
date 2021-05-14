// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OrbParent.h"
#include "BlueOrb.generated.h"

UCLASS()
class GUIDINGLIGHT_API ABlueOrb : public AOrbParent
{
	GENERATED_BODY()
	ABlueOrb();

public:
	UFUNCTION(BlueprintCallable, Category = "AimActive")
		void AimActive();

	UFUNCTION(BlueprintCallable, Category = "FireActive")
		void FireActive();

	virtual void BeginPlay() override;

	void Tick(float DeltaTime);

	FHitResult* ActiveHitResult;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Blink, meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* BlinkTargetParticle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Blink)
	bool bIsTargettingBlink;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Blink)
	FVector BlinkEndLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Blink)
	class UTimelineComponent* BlinkFOVTimeline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Blink)
	class UCurveFloat* BlinkFOVCurve;

	UPROPERTY(VisibleAnywhere, Category = "Blink")
	float FOVOffset;

	UFUNCTION(BluePrintCallable, Category = "Blink")
	void SpawnBlinkTarget();

	UFUNCTION(BluePrintCallable, Category = "Blink")
	void Blink();

	/** Declare our delegate function to be binded with TImelineFloatReturn */
	FOnTimelineFloat InterpFunction{};

	/** Declare our delegate function to be binded with OnTimelineFinished */
	FOnTimelineEvent TimelineFinished{};

	UWorld* World;


};
