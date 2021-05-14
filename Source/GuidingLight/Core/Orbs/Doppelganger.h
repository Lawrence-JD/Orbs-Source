// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Materials/Material.h"
#include "Classes/Components/PointLightComponent.h"
#include "Engine/PointLight.h"
#include "UObject/ConstructorHelpers.h"
#include "Doppelganger.generated.h"

UCLASS()
class GUIDINGLIGHT_API ADoppelganger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoppelganger();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Doppelganger, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* DoppelgangerMovementComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Blink, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* DoppelgangerMesh;

	/*
	The collision sphere that dictates what the blue "aura" off the doppelganger is interacting with
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Light, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* BlueSphere;

	/* The Doppelganger's Light Component (Point Light) */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Light, meta = (AllowPrivateAccess = "true"))
	class UPointLightComponent* DoppelgangerLight;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
