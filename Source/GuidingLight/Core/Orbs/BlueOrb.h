// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OrbParent.h"
#include "Orbs/Doppelganger.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "BlueOrb.generated.h"

UCLASS()
class GUIDINGLIGHT_API UBlueOrb : public UOrbParent
{
public:
	GENERATED_BODY()
	UBlueOrb();

	UFUNCTION(BlueprintCallable, Category = "AimActive")
		void AimActive();

	UFUNCTION(BlueprintCallable, Category = "FireActive")
		void FireActive();

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FHitResult* ActiveHitResult;

	UPROPERTY()
	float CrystalFOV;

	UPROPERTY()
	float BlueOrbBaseIntensity;

	UPROPERTY()
	float BlueOrbCharge;

	UPROPERTY()
	float SetValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Doppelganger, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ADoppelganger> DoppelgangerProjectile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Blink, meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* BlinkTargetParticle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Blink)
	bool bIsTargettingBlink;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Blink)
	FVector BlinkEndLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Blink)
	FVector FinalDestination;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Blink)
	FVector BeginningDestination;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Blink)
	class UTimelineComponent* BlinkFOVTimeline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Blink)
	class UCurveFloat* BlinkFOVCurve;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Blink)
	class UTimelineComponent* BlinkMovementTimeline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Blink)
	class UCurveFloat* BlinkMovementCurve;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Blink)
	class UTimelineComponent* BlueOrbRechargeTimeline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Blink)
	class UCurveFloat* BlueOrbRechargeCurve;

	UPROPERTY(VisibleAnywhere, Category = "Blink")
	float FOVOffset;

	UPROPERTY(EditAnywhere, Category = "Blink")
	class UMaterialInterface* FireTrueMat;
	
	UPROPERTY(EditAnywhere, Category = "Blink")
	class UMaterialInterface* FireFalseMat;

	UFUNCTION(BluePrintCallable, Category = "Doppelganger")
	void SpawnDoppelganger();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Doppelganger")
	bool bDoppelgangerIsMoving;

	/*
	Reference to the Doppelganger and whether or not it's been deployed.
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Doppelganger")
	ADoppelganger* DoppelTemp;

	UFUNCTION(BluePrintCallable, Category = "Blink")
	void SpawnBlinkTarget();

	UFUNCTION(BluePrintCallable, Category = "Blink")
	void Blink();

	UFUNCTION()
	void FOVTimelineFloatReturn(float Value);

	UFUNCTION()
	void FOVOnTimelineFinished();

	UFUNCTION()
	void MovementTimelineFloatReturn(float Value);

	UFUNCTION()
	void MovementOnTimelineFinished();

	UFUNCTION()
	void RechargeTimelineFloatReturn(float Value);

	UFUNCTION()
	void RechargeOnTimelineFinished();

	/** Declare our delegate function to be binded with FOVTimelineFloatReturn */
	FOnTimelineFloat FOVInterpFunction{};

	/** Declare our delegate function to be binded with FOVOnTimelineFinished */
	FOnTimelineEvent FOVTimelineFinished{};

	/** Declare our delegate function to be binded with MovementTimelineFloatReturn */
	FOnTimelineFloat MovementInterpFunction{};

	/** Declare our delegate function to be binded with MovementOnTimelineFinished */
	FOnTimelineEvent MovementTimelineFinished{};

	/** Declare our delegate function to be binded with RechargeTimelineFloatReturn */
	FOnTimelineFloat RechargeInterpFunction{};

	/** Declare our delegate function to be binded with RechargeOnTimelineFinished */
	FOnTimelineEvent RechargeTimelineFinished{};

	void OrbDeactivate();

	UPROPERTY()
	FVector CrystalStartLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USceneComponent* TargetMeshRoot;

	UWorld* World;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable, Category = "Doppelganger")
	void DestroyDoppelganger();

};
