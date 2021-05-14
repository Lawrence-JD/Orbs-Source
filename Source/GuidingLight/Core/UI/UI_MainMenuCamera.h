// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/InputComponent.h"
#include "UI_MainMenuCamera.generated.h"

UCLASS()
class GUIDINGLIGHT_API AUI_MainMenuCamera : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AUI_MainMenuCamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(EditAnywhere)
	USpringArmComponent* OurCameraSpringArm;
	UCameraComponent* OurCamera;

	// Input variables
	FVector2D MovementInput;
	FVector2D CameraInput;
	float ZoomFactor;
	
	// Input functions
	void Zoom(float Axisvalue);
	void MoveRight(float AxisValue);
	void YawCamera(float AxisValue);
	
};
