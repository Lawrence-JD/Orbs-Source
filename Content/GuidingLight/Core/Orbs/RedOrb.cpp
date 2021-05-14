// Fill out your copyright notice in the Description page of Project Settings.

#include "RedOrb.h"
#include "Core/Crystal/GuidingLightCharacter.h"
#include "HelperFiles/DefinedDebugHelpers.h"

// Sets default values
ARedOrb::ARedOrb()
{
	PrimaryActorTick.bCanEverTick = true;

	//Creating the Tags for the Red Orb
	this->Tags.Add(FName("Red"));
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

	//Set Orb Mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MaterialSphere(TEXT("StaticMesh'/Game/StarterContent/Props/MaterialSphere.MaterialSphere'"));
	if (MaterialSphere.Object)
	{
		OrbMesh->SetStaticMesh(MaterialSphere.Object);
		FVector Scale = FVector(.25);
		OrbMesh->SetWorldScale3D(Scale);
	}

	IsAiming = EAimingState::NotAiming;
	ActiveHitResult = new FHitResult();
	ActiveMaxRange = 2000.f;

	MaxPillars = 4;

	Reticle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Reticle"), true);

	static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("Material'/Game/GuidingLight/Environment/Props/Materials/M_RockPillar_Reticle.M_RockPillar_Reticle'"));
	if (Material.Object)
	{
		ReticleMat = Material.Object;
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	if (Cube.Object)
	{
		ReticleMesh = Cube.Object;
		
	}

	bReticleFlag = true;
}

void ARedOrb::AimActive()
{
	IsAiming = EAimingState::Aiming;
}

void ARedOrb::FireActive()
{
	IsAiming = EAimingState::Firing;
}

void ARedOrb::BeginPlay()
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

	Reticle->SetWorldScale3D(FVector(1, 1, 3));
	Reticle->SetVisibility(false);
	Reticle->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Reticle->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

void ARedOrb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Crystal)
	{
		FVector StartTrace = Crystal->FollowCamera->GetComponentLocation();
		FVector ForwardVector = Crystal->FollowCamera->GetForwardVector();
		StartTrace += ForwardVector * 350;
		FVector EndTrace = ((ForwardVector * 1500.f) + StartTrace);
		FCollisionQueryParams* TraceParams = new FCollisionQueryParams();

		if (!(IsAiming == EAimingState::NotAiming))
		{
			if (GetWorld()->LineTraceSingleByChannel(*ActiveHitResult, StartTrace, EndTrace, ECC_Visibility, *TraceParams))
			{
				//SCREENMSG(FString(ActiveHitResult->Actor->GetName()));
				if (!(ActiveHitResult->Actor == NULL))
				{
					TArray<UActorComponent*> OtherCollider = ActiveHitResult->Actor->GetComponentsByTag(UStaticMeshComponent::StaticClass(), TEXT("PillarSpawnZone"));
					if (OtherCollider.Num() > 0)
					{
						//SCREENMSG("SPAWN");
						FVector* Location = new FVector(ActiveHitResult->Location.X, ActiveHitResult->Location.Y, ActiveHitResult->Location.Z);
						FRotator Rotation = ActiveHitResult->Actor->GetActorRotation();

						if ((*Location - Crystal->GetActorLocation()).Size() < ActiveMaxRange)
						{
							if (IsAiming == EAimingState::Aiming)
							{
								//DEBUGL(StartTrace, EndTrace, true);

								Reticle->SetVisibility(true);

								if (bReticleFlag)
								{
									Reticle->SetWorldLocationAndRotation(*Location, Rotation);
									bReticleFlag = false;
								}
								else
									Reticle->SetWorldLocationAndRotation(FMath::Lerp(Reticle->GetComponentLocation(), *Location, DeltaTime * 20), Rotation);
							}
							else if (IsAiming == EAimingState::Firing)
							{
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
					}
				}
				else
				{
					Reticle->SetVisibility(false);
					bReticleFlag = true;
				}
			}
			else
			{
				Reticle->SetVisibility(false);
				bReticleFlag = true;
			}
		}
		else
		{
			Reticle->SetVisibility(false);
			bReticleFlag = true;
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
