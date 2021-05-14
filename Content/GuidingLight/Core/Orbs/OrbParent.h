// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Classes/Components/DirectionalLightComponent.h"
#include "Classes/Components/PointLightComponent.h"
#include "GameplayTagContainer.h"
#include "Particles/ParticleSystemComponent.h"

#include "Engine.h"
#include "Engine/World.h"
#include "Engine/TriggerVolume.h"
#include "Engine/PointLight.h"
#include "Engine/DirectionalLight.h"
#include "Engine/StaticMesh.h"

#include "UObject/ConstructorHelpers.h"

#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/Material.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OrbParent.generated.h"

struct FGameplayTagContainer;
class AGuidingLightCharacter;

UENUM()
enum class EAimingState
{
	Aiming	UMETA(DisplayName = "Aiming"),
	Firing UMETA(DisplayName = "Firing"),
	NotAiming	UMETA(DisplayName = "NotAiming"),
};

UCLASS()
class GUIDINGLIGHT_API AOrbParent : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOrbParent();

	/* DEPRACATED!!!!!!!
		Only kept in for now due to child code currently using it. Will be removed soon.
		Orb Tag (Ex: Blue, Yellow, Red - Reflects the color the orb represents)
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FGameplayTagContainer OrbTags;

	//Do we have this orb/have direct access to this orb?
	bool bIsInInventory;
	//Is the orb currently broken?
	bool bIsBroken;
	//Is this orb currently equipped?
	bool bIsEquipped;
	//Is it main hand 
	bool bMainHand;
	//Is orb in off hand
	bool bOffHand;

	EAimingState IsAiming;

	/*
	The radius of the orbs light/where it affects
	This value may be tweaked/split into two later on
	if it makes the gameplay feel better.
	*/
	float OrbRadius;

	AGuidingLightCharacter* Crystal;

	// The Orb's Mesh Component that physically represents the orb
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* OrbMesh;

	/* The Orb's Light Component (Point Light) */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Light, meta = (AllowPrivateAccess = "true"))
	class UPointLightComponent* OrbLight;

	/*
		The Orb's main Particle System
		May add another one in the future if it's deemed necessary
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Light, meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* OrbParticles;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Light, meta = (AllowPrivateAccess = "true"))
	class UMaterial* OrbMaterial;


	// The root component that all other components are attached to
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USceneComponent* DummyRoot;


	/*
	The collision sphere that dictates what the light is actively
	interacting with
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Light, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* OrbLightCollisionSphere;


	/** Activates the YellowOrb if it's deactivated, and activates it if it is deactivated */
	UFUNCTION(BlueprintCallable, Category = "Activate")
	void OrbActivate();

	UFUNCTION(BlueprintCallable, Category = "Activate")
	void OrbDeactivate();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
