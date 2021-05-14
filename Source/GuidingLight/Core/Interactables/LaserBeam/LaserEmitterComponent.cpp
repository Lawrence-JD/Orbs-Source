// Fill out your copyright notice in the Description page of Project Settings.


#include "LaserEmitterComponent.h"
#include "Engine/World.h"
#include "UObject/WeakObjectPtrTemplates.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/KismetMathLibrary.h"
#include <iostream>
#include "LaserBeam.h"

// Sets default values for this component's properties
ULaserEmitterComponent::ULaserEmitterComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void ULaserEmitterComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...
	
}


// Called every frame
void ULaserEmitterComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void ULaserEmitterComponent::CastRay(FVector InputOrigin, FVector InputForward, float Dist, int MaxReflects)
{
	//Takes an origin point and a forward vector * distance to draw a line trace between the two points
	FVector BeamOrigin; // origin used for SetEnds of the beam mesh
	FVector BeamEnd; // endpoint used for SetEnds on the beam mesh
	FVector Origin = InputOrigin; // origin point of the beam
	FVector Forward = InputForward; // forward vector of the beam
	bool bContinue = true; //true if beam has not been blocked, false otherwise
	int CurReflects = 0; // num of times the beam has reflected off of a mirror material
	ClearRay(); // clears ray

			//Spawn beam section
	UMaterialInterface* Mat1 = RedMat;
	switch (Amplifier->CurrentLightState) //switches based on the attached amplifier light state
	{
	case EColorState::Red:
		Mat1 = RedMat;
		bIsActive = true;
		break;
	case EColorState::Yellow:
		Mat1 = YellowMat;
		bIsActive = true;
		break;
	case EColorState::Blue:
		Mat1 = BlueMat;
		bIsActive = true;
		break;
	default:
		bIsActive = false;
		break;
	}



	while (CurReflects < MaxReflects && bContinue && bIsActive)
	{
		//Line trace section
		FVector OldForward = Forward;
		FVector EndPoint = (Forward * Dist) + Origin;
		FCollisionQueryParams TraceParams; //TraceParams sets the additional commands for the LineTrace to follow
		TraceParams.bTraceComplex = true; // Allows collisions with complex meshes
		TraceParams.bReturnPhysicalMaterial = true; // Sets the linetrace to return phys material of impacted mesh face
		TraceParams.AddIgnoredActor(this->GetOwner()); //Sets the linetrace to ignore the parent actor.
		FHitResult HitDetails = FHitResult(ForceInit); //Inits the HitDetails so it can be used as a output buffer
		bool bIsHit = GetWorld()->LineTraceSingleByChannel(
			HitDetails,      // FHitResult object that will be populated with hit info
			Origin,      // starting positions
			EndPoint,        // end position
			TraceChannel,  // the custom trace channel (light)
			TraceParams      // additional trace settings
		);

		//HitDetails result section
		BeamOrigin = Origin; //The beam origin is always the origin of the linetrace at either original origin, or point of reflection
		if (bIsHit) // If trace hit a object in game
		{
			BeamEnd = HitDetails.ImpactPoint; //Sets the beam endpoint to be at the location of impact if it hits something.
			if (HitDetails.PhysMaterial.IsValid()) // checks if the physmaterial ref is valid
			{
				UPhysicalMaterial* Mat = HitDetails.PhysMaterial.Get();
				EPhysicalSurface SurfHit = HitDetails.PhysMaterial->DetermineSurfaceType(Mat);
				if (SurfHit == MirrorSurfaceEnum)
				{
					Origin = HitDetails.ImpactPoint; //Sets the new temp origin to the point of reflection
					//mirrored vector = R = 2*(V dot N)*N - V  (V is incoming vector, N is surface normal)
					FVector V = Forward.GetSafeNormal();
					FVector N = HitDetails.ImpactNormal;
					Forward = UKismetMathLibrary::MirrorVectorByNormal(Forward, HitDetails.Normal); //uses 3d math to set new mirrored forward vec.
					CurReflects++; //adds to the curReflection count, will end the loop if curReflects >= maxReflects
				}
				else if (SurfHit == RecieverSurfaceEnum)
				{
					ALaserReciever* Reciever = Cast<ALaserReciever>(HitDetails.GetActor());
					if (Reciever != nullptr)
					{
						Reciever->SetHitState(true);
						bContinue = false;
					}
					bContinue = false;
				}
				else
				{
					bContinue = false; //if the beam did not hit a mirror material, (or any other future material logic) then it will simply end.
				}
			}
			
			
		}
		else // If trace did not hit an object in game, then the beam will just be a straight line up to dist specified
		{
			BeamEnd = HitDetails.TraceEnd;
			bContinue = false;
		}

		SpawnBeam(BeamOrigin, BeamEnd, Mat1, OldForward); //spawns the beam and adds it to mBeams
	}
}

void ULaserEmitterComponent::ClearRay()
{
	//Goes through beam list and destroys actors, and then empties out the pointers from the list.
	for (ALaserBeam* Beam : Beams)
	{
		Beam->SetActorHiddenInGame(true);
		if(Beam)
			Beam->Destroy();
	}
	Beams.Empty();
}

void ULaserEmitterComponent::InitAll(UMaterialInterface* Red, UMaterialInterface* Yellow, UMaterialInterface* Blue, UPhysicalMaterial* MirrorMat, UPhysicalMaterial* RecieveMat,
	AAmplifierLampPost* Amp, UBlueprint* BeamBP, ECollisionChannel TraceChannel)
{
	//Simplified init function allowing end user to set everything needed at once (acts as psuedo constructor)
	SetLaserMats(Red, Yellow, Blue);
	SetAmplifier(Amp);
	SetSpawnType(BeamBP);
	MirrorSurfaceEnum = MirrorMat->DetermineSurfaceType(MirrorMat);
	RecieverSurfaceEnum = RecieveMat->DetermineSurfaceType(RecieveMat);
	TraceChannel = TraceChannel;
}

void ULaserEmitterComponent::SetLaserMats(UMaterialInterface* Red, UMaterialInterface* Yellow, UMaterialInterface* Blue)
{
	//materials for the red yellow and blue variant lasers
	RedMat = Red;
	YellowMat = Yellow;
	BlueMat = Blue;
}

void ULaserEmitterComponent::SetSpawnType(UBlueprint* Blueprint)
{
	//blueprint ref for the laser beam, allows cpp to spawn blueprint only types
	SpawnType = Blueprint;
}

void ULaserEmitterComponent::SetAmplifier(AAmplifierLampPost* Amp)
{
	//Init for amplifier ref
	Amplifier = Amp;
}

void ULaserEmitterComponent::SpawnBeam(FVector Origin, FVector Endpoint, UMaterialInterface* LaserMat, FVector Normal)
{
		UWorld* World = GetWorld(); //Gets current active level
		if (World) // if world is valid
		{
			FActorSpawnParameters Params; //Sets the spawn parameters for the beam, namely that it should ignore collision when spawning
			Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			FRotator Rotator(0, 0, 0); //initial spawned rotation, the beam will change direction once SetEnds is called
			FVector SpawnLoc = Origin; //initial spawned position, SetEnds also sets it to be origin as a failsafe.
			ALaserBeam* Beam = (ALaserBeam*)World->SpawnActor<ALaserBeam>(ALaserBeam::StaticClass(), SpawnLoc, Rotator, Params);
			if (Beam)
			{
				Beam->SetEnds(Origin, Endpoint, Normal); //Sets the final rotation/location of the beam.
				Beam->SetMaterial(LaserMat); // calls the setmaterial of the laser beam, sets its apparent material to match the orb
				Beams.Add(Beam); //adds the beam to the list of beams to be culled next time castRay is called.
			}
		}
}



