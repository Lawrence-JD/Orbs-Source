// Fill out your copyright notice in the Description page of Project Settings.


#include "BlueOrb.h"
#include "Core/Crystal/GuidingLightCharacter.h"
#include "HelperFiles/DefinedDebugHelpers.h"

// Sets default values
ABlueOrb::ABlueOrb()
{
	PrimaryActorTick.bCanEverTick = true;

	//Creating the Tags for the Red Orb
	this->Tags.Add(FName("Blue"));
	OrbLightCollisionSphere->ComponentTags.Add(FName("Blue"));
	DummyRoot->ComponentTags.Add(FName("Blue"));

	//Setting up the values for the Orb Point Light
	OrbLight->SetLightColor(FLinearColor(0.5, 1, 0.99, 1.0));
	OrbLight->bUseTemperature = false;
	OrbLight->SetLightFalloffExponent(10000);
	OrbLight->SetIntensity(1700);
	OrbLight->SetAttenuationRadius(750);
	OrbLight->SetShadowBias(1);


	//Setting up collision radius
	OrbLightCollisionSphere->SetSphereRadius(550);

	//Set Orb Mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MaterialSphere(TEXT("StaticMesh'/Game/StarterContent/Props/SM_BlueOrb.SM_BlueOrb'"));
	if (MaterialSphere.Object)
	{
		OrbMesh->SetStaticMesh(MaterialSphere.Object);
		FVector Scale = FVector(.25);
		OrbMesh->SetWorldScale3D(Scale);
	}

	IsAiming = EAimingState::NotAiming;
	ActiveHitResult = new FHitResult();

	//Create the Blink Target Particle System
	BlinkTargetParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("BlinkTargetParticle"));


	static ConstructorHelpers::FObjectFinder<UParticleSystem> Particle(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Sparks.P_Sparks'"));
	if (Particle.Object)
	{
		UE_LOG(LogTemp, Warning, TEXT("I'm IN YOUR AREA"));
		BlinkTargetParticle->Template = Particle.Object;

	}
	

	FOVOffset = 50.0f;

	//Create the Timeline Component for the FOV Interpolation during "Blink"
	BlinkFOVTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("BlinkFOVTimeline"));

	InterpFunction.BindUFunction(this, FName("TimelineFloatReturn"));
	TimelineFinished.BindUFunction(this, FName("OnTimelineFinished"));
}


void ABlueOrb::AimActive()
{
	IsAiming = EAimingState::Aiming;
	//SCREENMSG("AIM");
}

void ABlueOrb::FireActive()
{
	IsAiming = EAimingState::Firing;
	//SCREENMSG("FIRE");
}

void ABlueOrb::BeginPlay()
{
	Super::BeginPlay();

	World = GetWorld();

	//Check to see if our curve asset is valid
	if (BlinkFOVCurve)
	{
		BlinkFOVTimeline->AddInterpFloat(BlinkFOVCurve, InterpFunction, FName("Alpha"));
	}

	//Turn off the reticle for Blink
	if (BlinkTargetParticle)
		BlinkTargetParticle->SetVisibility(false);
}

void ABlueOrb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(Crystal)
	{
	}
	if (IsAiming == EAimingState::Aiming)
	{
		SpawnBlinkTarget();
		if(BlinkTargetParticle)
			BlinkTargetParticle->SetVisibility(true);
	}

	if (IsAiming == EAimingState::Firing)
	{
		Blink();
		if (BlinkTargetParticle)
			BlinkTargetParticle->SetVisibility(false);
		IsAiming = EAimingState::NotAiming;
	}
}

void ABlueOrb::SpawnBlinkTarget()
{

	
	FHitResult HitResult;
	FVector PlayerCharacterForwardVec = Crystal->FollowCamera->GetForwardVector();
	FVector PlayerCameraLocation = Crystal->FollowCamera->GetComponentLocation();
	FVector EndLocation = (PlayerCharacterForwardVec * 1000) + PlayerCameraLocation;

	if (World->LineTraceSingleByChannel(
		HitResult,
		PlayerCameraLocation,
		EndLocation,
		ECollisionChannel::ECC_Visibility))
	{
		FVector HitLocation = HitResult.Location;
		BlinkEndLocation = HitLocation;
		BlinkTargetParticle->SetWorldLocation(BlinkEndLocation);
	}
	else
	{
		BlinkEndLocation = EndLocation;
		BlinkTargetParticle->SetWorldLocation(BlinkEndLocation);
	}

}

void ABlueOrb::Blink()
{

	float CrystalXPos = Crystal->GetActorLocation().X;
	float CrystalYPos = Crystal->GetActorLocation().Y;
	float CrystalZPos = Crystal->GetActorLocation().Z + 100;
	Crystal->SetActorLocation(FVector(CrystalXPos,CrystalYPos,CrystalZPos), true);
	Crystal->SetActorLocation(BlinkEndLocation, true);
}

