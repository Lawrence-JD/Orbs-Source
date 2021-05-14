// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "Engine/World.h"
#include "Math/Vector.h"
#include "GuidingLight/Core/Crystal/GuidingLightCharacter.h"
#include "HelperFiles/DefinedDebugHelpers.h"
#include "YellowOrbRevealShaderDriver.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GUIDINGLIGHT_API UYellowOrbRevealShaderDriver : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UYellowOrbRevealShaderDriver();
	// Holds a pointer to the Material Parameter Collection
	// That drives the shader for the hint statue
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShaderCollections)
		UMaterialParameterCollection* MaterialParameterCollectionAsset;
	// Holds player's position to update MPC fiel with
	FVector PlayerPosition;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	// Holds an instance of the MPC
	UMaterialParameterCollectionInstance* MaterialPCInstance;
	// Pointer to the player
	AGuidingLightCharacter* ThePlayer;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
