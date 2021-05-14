// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "LaserBeam.generated.h"


UCLASS()
class GUIDINGLIGHT_API ALaserBeam : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALaserBeam();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UStaticMeshComponent* LaserMesh; //The laser beam mesh associated with this laser beam blueprint

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
		void SetMeshComponent(UStaticMeshComponent* Mesh); //Called in begin play, sets the current laser mesh
	UFUNCTION(BlueprintCallable)
		void SetMaterial(UMaterialInterface* Mat); // sets the laser material for the laser beam
	void SetEnds(FVector Origin, FVector End, FVector Normal); // Sets the ends of the laser mesh so that it spans the delta vector between origin and end.
	void SetMeshVisible(bool Visibility);

};
