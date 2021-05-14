// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "LampPost.generated.h"

/*
* The Parent class for all Lamp Post Actors.
*/
UENUM(BlueprintType)
enum class EColorState : uint8
{
	Empty	UMETA(DisplayName = "Empty"),
	Yellow	UMETA(DisplayName = "Yellow"),
	Blue		UMETA(DisplayName = "Blue"),
	Red		UMETA(DisplayName = "Red")
};

class UCapsuleComponent;
class AGuidingLightCharacter;

UCLASS()
class GUIDINGLIGHT_API ALampPost : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALampPost();

	/** Current Lamp Color */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "A\|State")
	EColorState CurrentLightState;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "A\|State")
	UStaticMeshComponent* MeshComp;

	/** Capsule that detects overlap for Interaction */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "A\|State")
	UCapsuleComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Light, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* ColorEffectCollisionSphere;


	bool bIsLightOn;
	bool bPlayerInRange;

	/** Pointer to Player Character */
	AGuidingLightCharacter* PlayerCharacter;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** When Player is in Range*/
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** When Player is not in Range*/
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintImplementableEvent)
	void ColorChangedStart();

	UFUNCTION(BlueprintImplementableEvent)
	void ColorChangedEnd();

	//UFUNCTION(BlueprintCallable)
	/** Changes the Color to Current Main Hand Orb Color*/
	virtual void ToggleColorChange();

	void ChangeColor(EColorState NewColor);

	UFUNCTION()
		void OnColorOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnColorOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
