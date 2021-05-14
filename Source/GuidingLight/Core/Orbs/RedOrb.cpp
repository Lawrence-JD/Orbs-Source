// Fill out your copyright notice in the Description page of Project Settings.

#include "RedOrb.h"
#include "Core/Crystal/GuidingLightCharacter.h"
#include "HelperFiles/DefinedDebugHelpers.h"

// Sets default values
URedOrb::URedOrb()
{
	PrimaryComponentTick.bCanEverTick = true;

	V_LOG("RED ORB START");
	//Creating the Tags for the Red Orb
	OrbLightCollisionSphere->ComponentTags.Add(FName("Red"));
	DummyRoot->ComponentTags.Add(FName("Red"));

	//Setting up the values for the Orb Point Light
	OrbLight->SetLightColor(FLinearColor(0.72, 0.09, 0.07, 1.0));
	OrbLight->bUseTemperature = false;
	OrbLight->SetLightFalloffExponent(10000);
	OrbLight->SetIntensity(1700);
	OrbLight->SetAttenuationRadius(675);
	OrbLight->SetShadowBias(1);


	//Setting up collision radius
	OrbLightCollisionSphere->SetSphereRadius(550);
	OrbLightCollisionSphere->SetVisibility(true);

	//Set Orb Mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MaterialSphere(TEXT("StaticMesh'/Game/GuidingLight/Environment/Props/Assets/SM_P_RedOrb.SM_P_RedOrb'"));
	if (MaterialSphere.Object)
	{
		OrbMesh->SetStaticMesh(MaterialSphere.Object);
		FVector Scale = FVector(.75);
		OrbMesh->SetWorldScale3D(Scale);
	}

	IsAiming = EAimingState::NotAiming;
	ActiveHitResult = new FHitResult();
	ActiveMaxRange = 2000.f;

	MaxPillars = 10;

	Reticle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Reticle"), true);

	static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("Material'/Game/GuidingLight/Environment/Props/Materials/M_RockPillar_Reticle.M_RockPillar_Reticle'"));
	if (Material.Object)
	{
		ReticleMat = Material.Object;
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube(TEXT("StaticMesh'/Game/GuidingLight/Environment/Props/Assets/SM_P_Red_Orb_Pillar_A.SM_P_Red_Orb_Pillar_A'"));
	if (Cube.Object)
	{
		ReticleMesh = Cube.Object;

	}

	bReticleFlag = true;

		//static ConstructorHelpers::FObjectFinder<UClass> PillarSpawnEffectFinder(TEXT("Blueprint'/Game/GuidingLight/Core/Interactables/RockPillar/PillarSpawnEffect.PillarSpawnEffect_C'"));
	//if (PillarSpawnEffectFinder.Object)
	//{
	//	URedOrbClass = PillarSpawnEffectFinder.Object;
	//}

	ReticleOffset = 325;
	V_LOG("RED ORB END");
}

void URedOrb::AimActive()
{
	IsAiming = EAimingState::Aiming;
}

void URedOrb::FireActive()
{
	IsAiming = EAimingState::Firing;
}

void URedOrb::BeginPlay()
{
	Super::BeginPlay();

	if (ReticleMesh)
	{
		Reticle->SetStaticMesh(ReticleMesh);
	}
	if (ReticleMat)
	{
		Reticle->SetMaterial(0, ReticleMat);
	}

	//Reticle->SetWorldScale3D(FVector(1, 1, 3));
	Reticle->SetVisibility(false);
	Reticle->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Reticle->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

void URedOrb::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	bActiveUsed = false;
	
	if (ReticleOffset > 0)
	{
		float dist = 100 * DeltaTime;
		ReticleOffset -= dist;
	}
	else
	{
		ReticleOffset = 325;
	}

	if (Crystal)
	{
		FVector StartTrace = Crystal->FollowCamera->GetComponentLocation();
		FVector ForwardVector = Crystal->FollowCamera->GetForwardVector();
		StartTrace += ForwardVector * 350;
		FVector EndTrace = ((ForwardVector * 1500.f) + StartTrace);
		FCollisionQueryParams* TraceParams = new FCollisionQueryParams();

		if (!(IsAiming == EAimingState::NotAiming))
		{
			float CameraYaw = Crystal->GetFollowCamera()->GetComponentRotation().Yaw;
			Crystal->SetActorRotation(FRotator(Crystal->GetMesh()->GetComponentRotation().Pitch, CameraYaw, Crystal->GetMesh()->GetComponentRotation().Roll));

			if (GetWorld()->LineTraceSingleByChannel(*ActiveHitResult, StartTrace, EndTrace,ECC_Visibility, *TraceParams))
			{
				//SCREENMSG(FString(ActiveHitResult->Actor->GetName()));
				if (!(ActiveHitResult->Actor == NULL))
				{
					TArray<UActorComponent*> OtherCollider = ActiveHitResult->Actor->GetComponentsByTag(UStaticMeshComponent::StaticClass(), TEXT("PillarSpawnZone"));
					if (OtherCollider.Num() > 0)
					{
						FVector* Location = new FVector(ActiveHitResult->Location.X, ActiveHitResult->Location.Y, ActiveHitResult->Location.Z);
						FRotator OtherUp = ActiveHitResult->Normal.Rotation();
						FRotator Rotation = FRotator(OtherUp.Pitch - 90, OtherUp.Yaw, OtherUp.Roll);

						if ((*Location - Crystal->GetActorLocation()).Size() < ActiveMaxRange)
						{
							if (IsAiming == EAimingState::Aiming)
							{
								//DEBUGL(StartTrace, EndTrace, true);

								Reticle->SetVisibility(true);
								
								*Location -= ActiveHitResult->Normal * ReticleOffset;
								if (bReticleFlag)
								{
									Reticle->SetWorldLocationAndRotation(*Location, Rotation);
									bReticleFlag = false;
								}
								else
								{
									Reticle->SetWorldLocationAndRotation(FMath::Lerp(Reticle->GetComponentLocation(), *Location, DeltaTime * 20), Rotation);
								}
							}
							else if (IsAiming == EAimingState::Firing)
							{

								SCREENMSG("SPAWN PILLAR");
								bActiveUsed = true;
								//URedOrb* NewSpawnEffect = (URedOrb*)GetWorld()->SpawnActor(URedOrbClass, Location, &Rotation);
								ARockPillar* temp = (ARockPillar*)GetWorld()->SpawnActor(ARockPillar::StaticClass(), Location, &Rotation);
								RockPillars.Add(temp);
								IsAiming = EAimingState::NotAiming;
							}
						}
					}
					else
					{
						Reticle->SetVisibility(false);
						bReticleFlag = true;
						if (IsAiming == EAimingState::Firing)
							IsAiming = EAimingState::NotAiming;
					}
				}
				else
				{
					Reticle->SetVisibility(false);
					bReticleFlag = true;
					if (IsAiming == EAimingState::Firing)
						IsAiming = EAimingState::NotAiming;
				}
			}
			else
			{
				Reticle->SetVisibility(false);
				bReticleFlag = true;
				if (IsAiming == EAimingState::Firing)
					IsAiming = EAimingState::NotAiming;
			}
		}
		else
		{
			Reticle->SetVisibility(false);
			bReticleFlag = true;
			if (IsAiming == EAimingState::Firing)
				IsAiming = EAimingState::NotAiming;
		}
	}

	if (RockPillars.Num() > MaxPillars)
	{
		//SCREENMSG(TEXT("REMOVE"));
		if (RockPillars[0])
			RockPillars[0]->Destroy();
	
		RockPillars.RemoveAt(0);
	}

	int cap = RockPillars.Num();
	for (int i = 0; i < cap; i++)
	{
		if (RockPillars[i] && RockPillars[i]->IsPendingKill())
		{
			RockPillars.RemoveAt(i);
			cap--;
			i--;
		}
	}
}

void URedOrb::OrbDeactivate()
{
	Reticle->SetVisibility(false);
	bReticleFlag = true;
	Super::OrbDeactivate();
}
