// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"
#include "YellowOrbActiveReceiver.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GUIDINGLIGHT_API UYellowOrbActiveReceiver : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UYellowOrbActiveReceiver();
	bool ReceivingEnergy;
	bool EnergyTickToBeUsed;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void ReceiveTickOfEnergy();
	void TurnEnergyReceiptOn();
	void TurnEnergyReceiptOff();
	UFUNCTION(BlueprintCallable, Category = "Active Yellow Orb")
	bool IsReceivingEnergy();
	FVector GiveTargetLocation();
};
