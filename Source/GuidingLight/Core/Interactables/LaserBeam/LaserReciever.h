// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LaserReciever.generated.h"

UCLASS()
class GUIDINGLIGHT_API ALaserReciever : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALaserReciever();
	bool IsHit = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
		void SetHitState(bool State); //sets the hitstate to true, used in blueprints for activated logic
	UFUNCTION(BlueprintCallable)
		bool GetHitState(); //used via blueprints to get the hitstate to determine logic path
};
