// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ARotateHelper.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GUIDINGLIGHT_API UARotateHelper : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UARotateHelper();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/**
	* Calculate how much rotation should be done on one frame given the current timeline interval.
	* Note: a timeline element should be used in the blueprint for where this function is called. 
	* @param StartingRotation - Rotator value before event starts
	* @param RotateBy - How many degrees the StartingRotation should change by
	* @param CurrentTimelineValue - requires use of a timeline, the current value on the timeline curve
	* @return A rotator for the new world rotation for that particular tick in the timeline 
	*/
	UFUNCTION(BlueprintCallable)
	FRotator RotatorCalculate(FRotator StartingRotation, FRotator RotateBy, float CurrentTimelineValue);
		
};
