// Fill out your copyright notice in the Description page of Project Settings.


#include "BlueOrb.h"
#include "Core/Crystal/GuidingLightCharacter.h"
#include "HelperFiles/DefinedDebugHelpers.h"
#include "Math/Vector.h"
#include "Math/Rotator.h"

// Sets default values
UBlueOrb::UBlueOrb()
{
	V_LOG("BLUE ORB START");
	BlueOrbCharge = 1.0;

	PrimaryComponentTick.bCanEverTick = true;
	bTickInEditor = true;

	//Creating the Tags for the Red Orb
	OrbLightCollisionSphere->ComponentTags.Add(FName("Blue"));
	DummyRoot->ComponentTags.Add(FName("Blue"));

	TargetMeshRoot = CreateDefaultSubobject<USceneComponent>(TEXT("TargetMeshRoot"));

	BlueOrbBaseIntensity = 1700;
	SetValue = 0;

	//Setting up the values for the Orb Point Light
	OrbLight->SetLightColor(FLinearColor(0.5, 1, 0.99, 1.0));
	OrbLight->bUseTemperature = false;
	OrbLight->SetLightFalloffExponent(10000);
	OrbLight->SetIntensity(BlueOrbBaseIntensity);
	OrbLight->SetAttenuationRadius(750);
	OrbLight->SetShadowBias(1);
	

	//Setting up collision radius
	OrbLightCollisionSphere->SetSphereRadius(550);
	OrbLightCollisionSphere->SetVisibility(true);

	//Set Orb Mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MaterialSphere(TEXT("StaticMesh'/Game/GuidingLight/Environment/Props/Assets/SM_P_BlueOrb.SM_P_BlueOrb'"));
	if (MaterialSphere.Object)
	{
		OrbMesh->SetStaticMesh(MaterialSphere.Object);
		FVector Scale = FVector(.75);
		OrbMesh->SetWorldScale3D(Scale);
	}


	bDoppelgangerIsMoving = true;

	FireFalseMat = CreateDefaultSubobject<UMaterial>(TEXT("FireFalseMat"));
	FireTrueMat = CreateDefaultSubobject<UMaterial>(TEXT("FireTrueMat"));

	IsAiming = EAimingState::NotAiming;
	ActiveHitResult = new FHitResult();

	//Create the Blink Target Particle System
	BlinkTargetParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("BlinkTargetParticle"));


	static ConstructorHelpers::FObjectFinder<UParticleSystem> Particle(TEXT("ParticleSystem'/Game/GuidingLight/Effects/Orb/P_BlueOrb_Sparks.P_BlueOrb_Sparks'"));
	if (Particle.Object)
	{
		//UE_LOG(LogTemp, Warning, TEXT("I'm IN YOUR AREA"));
		BlinkTargetParticle->Template = Particle.Object;

	}
	

	FOVOffset = 25.0f;

	//Create the Timeline Component for the FOV Interpolation during "Blink"
	BlinkFOVTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("BlinkFOVTimeline"));
	FOVInterpFunction.BindUFunction(this, FName("FOVTimelineFloatReturn"));
	FOVTimelineFinished.BindUFunction(this, FName("FOVOnTimelineFinished"));

	//Create the Timeline Component for the Movement during "Blink"
	BlinkMovementTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("BlinkMovementTimeline"));
	MovementInterpFunction.BindUFunction(this, FName("MovementTimelineFloatReturn"));
	MovementTimelineFinished.BindUFunction(this, FName("MovementTimelineFinished"));
	
	//Create the Timeline Component for the Recharge after throwing the Doppelganger
	BlueOrbRechargeTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("BlueOrbRechargeTimeline"));
	RechargeInterpFunction.BindUFunction(this, FName("RechargeTimelineFloatReturn"));
	RechargeTimelineFinished.BindUFunction(this, FName("RechargeTimelineFinished"));

	OrbLightCollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &UBlueOrb::OnOverlapBegin);
	OrbLightCollisionSphere->OnComponentEndOverlap.AddDynamic(this, &UBlueOrb::OnOverlapEnd);

	ADoppelganger* DoppelTemp = NULL;

	V_LOG("BLUE ORB END");
}


void UBlueOrb::AimActive()
{
	IsAiming = EAimingState::Aiming;
	//SCREENMSG("AIM");
}

void UBlueOrb::FireActive()
{
	IsAiming = EAimingState::Firing;
	//SCREENMSG("FIRE");
}

void UBlueOrb::BeginPlay()
{
	Super::BeginPlay();

	World = GetWorld();

	//See if the curve exists

	//FOVCurve is set in Crystal's Blueprint in the BlueOrbChildComponent
	if (BlinkFOVCurve)
	{
		//Add the float curve to the timeline
		BlinkFOVTimeline->AddInterpFloat(BlinkFOVCurve, FOVInterpFunction, FName("Alpha"));
		// Add OnTimelineFinished Function
		BlinkFOVTimeline->SetTimelineFinishedFunc(FOVTimelineFinished);
	}

	//MovementCurve is set in Crystal's Blueprint in the BlueOrbChildComponent
	if (BlinkMovementCurve)
	{
		BlinkMovementTimeline->AddInterpFloat(BlinkMovementCurve, MovementInterpFunction, FName("Alpha"));
		BlinkMovementTimeline->SetTimelineFinishedFunc(MovementTimelineFinished);
	}

	if (BlueOrbRechargeCurve)
	{
		BlueOrbRechargeTimeline->AddInterpFloat(BlueOrbRechargeCurve, RechargeInterpFunction, FName("Alpha"));
		BlueOrbRechargeTimeline->SetTimelineFinishedFunc(RechargeTimelineFinished);
	}

	//Turn off the reticle for Blink
	if (BlinkTargetParticle)
	{
		BlinkTargetParticle->SetVisibility(false);
	}

	BlinkFOVTimeline->SetLooping(false);
	BlinkFOVTimeline->SetIgnoreTimeDilation(false);

	BlinkMovementTimeline->SetLooping(false);
	BlinkMovementTimeline->SetIgnoreTimeDilation(false);

	if (Crystal && Crystal->FollowCamera)
	{
		CrystalFOV = Crystal->FollowCamera->FieldOfView;
	}

	if (Crystal)
	{
		Crystal->bBlinkTrailActivate = false;
	}
	
	
}

void UBlueOrb::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	if(Crystal)
	{
	}
	if (IsAiming == EAimingState::Aiming)
	{
		SpawnBlinkTarget();
		if (BlinkTargetParticle)
		{
			BlinkTargetParticle->SetVisibility(true);
		}
		
		float CameraYaw = Crystal->GetFollowCamera()->GetComponentRotation().Yaw;
		Crystal->SetActorRotation(FRotator(Crystal->GetMesh()->GetComponentRotation().Pitch, CameraYaw, Crystal->GetMesh()->GetComponentRotation().Roll));
	}
	else if (IsAiming == EAimingState::Firing)
	{

		BlinkTargetParticle->SetVisibility(false);

		SpawnDoppelganger();
		if (BlueOrbCharge >= 1)
		{

			SpawnDoppelganger();
		}
			
		IsAiming = EAimingState::NotAiming;

		float CameraYaw = Crystal->GetFollowCamera()->GetComponentRotation().Yaw;

		Crystal->SetActorRotation(FRotator(Crystal->GetMesh()->GetComponentRotation().Pitch, CameraYaw, Crystal->GetMesh()->GetComponentRotation().Roll));

	}
	
	if (DoppelTemp != NULL)
	{
		if (DoppelTemp->IsActorBeingDestroyed())
		{
			DoppelTemp = NULL;
		}
	}

	
}

void UBlueOrb::SpawnDoppelganger()
{


	if (DoppelgangerProjectile != NULL)
	{
		GLog->Log("WE ARE HERE");
		if (DoppelTemp != NULL)
		{
			DoppelTemp->Destroy(true);
			DoppelTemp = NULL;
		}

		if (DoppelTemp == NULL)
		{
			FVector SpawnLocation;
			FRotator CameraRotation;

			SpawnLocation = Crystal->GetActorLocation();
			CameraRotation = Crystal->GetFollowCamera()->GetComponentRotation();

			FVector DoppelSpawnLocation = SpawnLocation + FTransform(CameraRotation).TransformVector(FVector(80.0f, 0.0f, -30.0f));

			CameraRotation.Pitch += 10.0f;

			if (World != NULL)
			{
				FActorSpawnParameters ActorSpawnParams;

				DoppelTemp = World->SpawnActor<ADoppelganger>(DoppelgangerProjectile, DoppelSpawnLocation, CameraRotation, ActorSpawnParams);

				FRotator CrystRot = Crystal->GetActorRotation();
				CrystRot.Yaw -= 90;
				DoppelTemp->SetActorRotation(CrystRot);
				BlueOrbRechargeTimeline->PlayFromStart();
			}
		}

		
	}

}

void UBlueOrb::DestroyDoppelganger()
{
	if (DoppelTemp != NULL)
	{
		DoppelTemp->Destroy(true);
		
	}
}

void UBlueOrb::SpawnBlinkTarget()
{

	
	FHitResult HitResult;
	FVector PlayerCharacterForwardVec = Crystal->FollowCamera->GetForwardVector();
	FVector PlayerCameraLocation = Crystal->FollowCamera->GetComponentLocation();
	FVector EndLocation = (PlayerCharacterForwardVec * 1250) + PlayerCameraLocation;

	if (World->LineTraceSingleByChannel(
		HitResult,
		PlayerCameraLocation,
		EndLocation,
		ECollisionChannel::ECC_Visibility))
	{
		FVector HitLocation = HitResult.Location;
		BlinkEndLocation = HitLocation;
	}
	else
	{
		BlinkEndLocation = EndLocation;
	}

	BlinkTargetParticle->SetWorldLocation(BlinkEndLocation);
	//BlinkTargetMesh->SetWorldLocation(BlinkEndLocation);
	
	

	//SCREENMSG2C("BlinkTargetMesh Rotation:  ", (BlinkTargetMesh->GetComponentRotation().ToString()), Red);

}

void UBlueOrb::Blink()
{

	Crystal->bBlinkTrailActivate = true;

	float CrystalXPos = Crystal->GetActorLocation().X;
	float CrystalYPos = Crystal->GetActorLocation().Y;
	float CrystalZPos = Crystal->GetActorLocation().Z + 100;


	CrystalStartLocation.GetSafeNormal();

	Crystal->SetActorLocation(FVector(CrystalXPos, CrystalYPos, CrystalZPos), true);

	BlinkFOVTimeline->Play();

	CrystalStartLocation = Crystal->GetActorLocation();

	Crystal->GetCharacterMovement()->Velocity = FVector(0, 0, 0);

	BlinkMovementTimeline->PlayFromStart();

	BlueOrbRechargeTimeline->PlayFromStart();

}

void UBlueOrb::FOVTimelineFloatReturn(float Value)
{
	//GLog->Log("PLAY");
	Crystal->FollowCamera->SetFieldOfView(FMath::Lerp(CrystalFOV, CrystalFOV - FOVOffset, Value));
}

void UBlueOrb::FOVOnTimelineFinished()
{
	BlinkFOVTimeline->Reverse();
}

void UBlueOrb::MovementTimelineFloatReturn(float Value)
{
	Crystal->SetActorLocation(FMath::Lerp(CrystalStartLocation, FinalDestination, Value), true);

}

void UBlueOrb::MovementOnTimelineFinished()
{
	BlinkMovementTimeline->Stop();

}

void UBlueOrb::RechargeTimelineFloatReturn(float Value)
{
	BlueOrbCharge = FMath::Lerp(0.0, 1.0, Value);
	//FString SVString = FString::SanitizeFloat(SetValue);

}

void UBlueOrb::RechargeOnTimelineFinished()
{
	BlueOrbRechargeTimeline->Stop();
}

void UBlueOrb::OrbDeactivate()
{
	BlinkTargetParticle->SetVisibility(false);
	Super::OrbDeactivate();

}

void UBlueOrb::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Other Actor is the actor that triggered the event. Check that is not ourself.  
	if ((OtherActor != nullptr) && (OtherComp != nullptr))
	{
		TArray<UActorComponent*> OtherCollider = OtherActor->GetComponentsByTag(USceneComponent::StaticClass(), TEXT("Spoopy"));
		if (OtherCollider.Num() > 0)
		{
			TArray<UActorComponent*> OtherMeshComponents = OtherActor->GetComponentsByClass(UMeshComponent::StaticClass());
			for (int i = 0; i < OtherMeshComponents.Num(); i++)
			{
				((UMeshComponent*)OtherMeshComponents[i])->BodyInstance.SetCollisionProfileName("Ghosted");
				((UMeshComponent*)OtherMeshComponents[i])->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Ignore);
			}
		}
	}
}

UFUNCTION()
void UBlueOrb::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		TArray<UActorComponent*> OtherCollider = OtherActor->GetComponentsByTag(USceneComponent::StaticClass(), TEXT("Spoopy"));
		if (OtherCollider.Num() > 0)
		{
			TArray<AActor*> OverlappingActorList;
			OtherActor->GetOverlappingActors(OverlappingActorList);

			int count = 0;
			for (int i = 0; i < OverlappingActorList.Num(); i++)
				count += OverlappingActorList[i]->GetComponentsByTag(USphereComponent::StaticClass(), TEXT("Blue")).Num();

			if (count < 1)
			{
				TArray<UActorComponent*> OtherMeshComponents = OtherActor->GetComponentsByClass(UMeshComponent::StaticClass());
				for (int i = 0; i < OtherMeshComponents.Num(); i++)
				{
					((UMeshComponent*)OtherMeshComponents[i])->BodyInstance.SetCollisionProfileName("BlockAllDynamic");
					((UMeshComponent*)OtherMeshComponents[i])->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Block);
				}
			}

		}
	}
}

