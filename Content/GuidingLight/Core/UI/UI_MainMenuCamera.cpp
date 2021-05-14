// Fill out your copyright notice in the Description page of Project Settings.

#include "UI_MainMenuCamera.h"

// Sets default values
AUI_MainMenuCamera::AUI_MainMenuCamera()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create components
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	OurCameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	OurCameraSpringArm->SetupAttachment(RootComponent);
	OurCameraSpringArm->TargetArmLength = 400.f;
	OurCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MenuCamera"));
	OurCamera->SetupAttachment(OurCameraSpringArm, USpringArmComponent::SocketName);

	// Take control of camera
	AutoPossessPlayer = EAutoReceiveInput::Player0;

}

// Called when the game starts or when spawned
void AUI_MainMenuCamera::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUI_MainMenuCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	{
		// Min and Max zoom
		ZoomFactor = FMath::Clamp<float>(ZoomFactor, -0.3f, 0.3f);
		//Blend our camera's FOV and our SpringArm's length based on ZoomFactor
		OurCamera->FieldOfView = FMath::Lerp<float>(90.0f, 60.0f, ZoomFactor);
		OurCameraSpringArm->TargetArmLength = FMath::Lerp<float>(400.0f, 300.0f, ZoomFactor); 
		
	}

	// Rotate our actor's Yaw
	{
		FRotator NewRotation = GetActorRotation();
		NewRotation.Yaw += CameraInput.X;
		SetActorRotation(NewRotation);

	}

	// Handle Movement based on our "MoveY" axes
	{
		if (!MovementInput.IsZero())
		{
			// Scale our movement input axis by 650 units per second
			MovementInput = MovementInput.GetSafeNormal() * 650.0f;
			FVector NewLocation = GetActorLocation();
			NewLocation += GetActorRightVector() * MovementInput.Y * DeltaTime;
			SetActorLocation(NewLocation);

		}
	}
}

// Called to bind functionality to input
void AUI_MainMenuCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Hook up every-frame handling for our axes
	InputComponent->BindAxis("CameraZoom", this, &AUI_MainMenuCamera::Zoom);
	InputComponent->BindAxis("CameraMoveRight", this, &AUI_MainMenuCamera::MoveRight);
	InputComponent->BindAxis("CameraYaw", this, &AUI_MainMenuCamera::YawCamera);
	
}

// Input function
void AUI_MainMenuCamera::MoveRight(float AxisValue)
{
	// A and D key
	MovementInput.Y = AxisValue;

}

void AUI_MainMenuCamera::YawCamera(float AxisValue)
{
	// A and D key
	CameraInput.X = AxisValue;

}

void AUI_MainMenuCamera::Zoom(float AxisValue)
{
	// W and S key
	ZoomFactor += AxisValue;

}



