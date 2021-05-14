// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "LaserBeam.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/World.h"
#include "Core/Interactables/LampPosts/AmplifierLampPost.h"
#include <Core\Interactables\LaserBeam\LaserReciever.h>

#include "LaserEmitterComponent.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GUIDINGLIGHT_API ULaserEmitterComponent : public UActorComponent
{
	GENERATED_BODY()
protected:
	TArray<ALaserBeam*> Beams; //list of laser beam class pointers
	UPhysicalMaterial* MirrorMaterial; //physical material for mirror logic
	UPhysicalMaterial* RecieverMaterial; //physical material for the reciever
	UBlueprint* SpawnType; //The blueprint associated with the ALaserBeam class <Blueprint must implement ALaserBeam>
	UMaterialInterface* RedMat;
	UMaterialInterface* YellowMat;
	UMaterialInterface* BlueMat;
	AAmplifierLampPost* Amplifier;
	ECollisionChannel TraceChannel;

public:	
	// Sets default values for this component's properties
	ULaserEmitterComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	EPhysicalSurface MirrorSurfaceEnum;
	EPhysicalSurface RecieverSurfaceEnum;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintCallable)
		void CastRay(FVector Origin, FVector Forward, float Dist, int MaxReflections); //Linetraces by the Light channel 
	UFUNCTION(BlueprintCallable)
		void ClearRay();
	UFUNCTION(BlueprintCallable)
		void InitAll(UMaterialInterface* Red, UMaterialInterface* Yellow, UMaterialInterface* Blue, UPhysicalMaterial* MirrorMat, UPhysicalMaterial* RecieveMat, 
			AAmplifierLampPost* Amp, UBlueprint* BeamBP, ECollisionChannel TraceChannel);
		void SetLaserMats(UMaterialInterface* Red, UMaterialInterface* Yellow, UMaterialInterface* Blue);
		void SetSpawnType(UBlueprint* Blueprint); //Sets the blueprint class that is to be spawned in SpawnBeam
		void SetAmplifier(AAmplifierLampPost* Amp);
	void SpawnBeam(FVector Origin, FVector Endpoint, UMaterialInterface* LaserMaterial, FVector Normal); //Spawns a ALaserBeam class and adds it to the mBeams TArray
		
};
