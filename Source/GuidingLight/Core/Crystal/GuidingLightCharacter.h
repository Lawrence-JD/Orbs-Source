// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/TimelineComponent.h"
#include "Components/PostProcessComponent.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "Core/Interactables/LampPosts/LampPost.h"
#include "Core/Interactables/OrbPickups/OrbPickupParent.h"
#include "Orbs/YellowOrbActiveReceiver.h"
#include "Orbs/OrbParent.h"
#include "Orbs/YellowOrb.h"
#include "Orbs/RedOrb.h"
#include "Orbs/BlueOrb.h"
#include "Orbs/Doppelganger.h"
#include "GuidingLightCharacter.generated.h"

UENUM(BlueprintType)
enum class EMainHandColorState : uint8
{
	Empty	UMETA(DisplayName = "Empty"),
	Yellow	UMETA(DisplayName = "Yellow"),
	Blue		UMETA(DisplayName = "Blue"),
	Red		UMETA(DisplayName = "Red"),
};

UENUM()
enum class EOffHandColorState
{
	Empty	UMETA(DisplayName = "Empty"),
	Yellow	UMETA(DisplayName = "Yellow"),
	Blue		UMETA(DisplayName = "Blue"),
	Red		UMETA(DisplayName = "Red"),
};

USTRUCT(BlueprintType)
struct FOrbStruct
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class URedOrb* PRedOrb;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UYellowOrb* PYellowOrb;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBlueOrb* PBlueOrb;
};

class ALampPost;
class AGuidingLightGameMode;

UCLASS(config=Game)
class AGuidingLightCharacter : public ACharacter
{
	GENERATED_BODY()
public:

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UPostProcessComponent* CrystalPostProcess;

	AGuidingLightCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category =
		Action)
	TEnumAsByte<EMovementMode> OriginMovementMode;

	//The Yellow Orb Child Actor
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Orb, meta = (AllowPrivateAccess = "true"))
	//class UChildActorComponent* PlayerYellowOrb;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Orb, meta = (AllowPrivateAccess = "true"))
	//class UChildActorComponent* PlayerRedOrb;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Orb, meta = (AllowPrivateAccess = "true"))
	//class UChildActorComponent* PlayerBlueOrb;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;
	
	//Is the Yellow Orb in the Player Inventory?
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Orb)
	bool bYellowOrbInInventory;
	
	//Is the Blue Orb in the Player Inventory?
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Orb)
	bool bBlueOrbInInventory; 
	
	//Is the Red Orb in the Player Inventory?
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Orb)
	bool bRedOrbInInventory;

	//Is the Red Orb in the Player Inventory?
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Orb)
	bool bGrabbingOrb;

	//Is the Red Orb in the Player Inventory?
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Orb)
	bool bSwitchingOrb;

	//Whether the player has at least one orb in their inventory or not
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = 
		Orb)
	bool bHasAnOrb;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category =
		Action)
	bool bPullingLever;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category =
		Action)
	bool bRotatingWheel;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category =
		Action)
	bool bOrangeJustice;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category =
		Action)
	bool bAiming;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Orb")
	class AYellowOrbPickup* YOrbPickup;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Orb")
	class ARedOrbPickup* ROrbPickup;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Orb")
	class ABlueOrbPickup* BOrbPickup;

	UFUNCTION()
	void DoneGrabbingOrb();

	UFUNCTION()
	void PutYellowPickupInHand();

	UFUNCTION()
	void PutRedPickupInHand();

	UFUNCTION()
	void PutBluePickupInHand();

	/** Creates the delay, so that the player can't move while grabbing an orb s*/
	FTimerHandle GrabbingOrbDelayTimerHandle;

	/** Creates the delay, so that the player doesn't automatically have the orb in hand*/
	FTimerHandle PickupOrbToHandDelayTimerHandle;

	//Actor for the active yellow orb lightning generator
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Active Yellow Orb")
	class AActor* YellowOrbLightningGenerator;

	UPROPERTY(VisibleAnywhere, Category = "Active Yellow Orb")
	class USphereComponent* YellowOrbSearchSphere;

	// Blueprint Event For Activating the Lightning Generator
	UFUNCTION(Category = "Active Yellow Orb", BlueprintImplementableEvent, BlueprintCallable)
	void ActivateActiveYellowOrbGenerator();

	// Blueprint Event For Deactivating the Lightning Generator
	UFUNCTION(Category = "Active Yellow Orb", BlueprintImplementableEvent, BlueprintCallable)
	void DeactivateActiveYellowOrbGenerator();

	// Blueprint Event for Setting the Lightning Generator to Directed
	UFUNCTION(Category = "Active Yellow Orb", BlueprintImplementableEvent, BlueprintCallable)
	void SetActiveYellowOrbGeneratorToDirectedMode(AActor * TargetActor);

	// Blueprint Event for Setting the Lightning Generator to Undirected
	UFUNCTION(Category = "Active Yellow Orb", BlueprintImplementableEvent, BlueprintCallable)
	void SetActiveYellowOrbGeneratorToUndirectedMode();

	// Blueprint Event for Setting the Lightning Generator's Target In Directed Mode
	UFUNCTION(Category = "Active Yellow Orb", BlueprintImplementableEvent, BlueprintCallable)
	void SetActiveYellowOrbGeneratorTarget(FVector TargetLocation);

	// Radius of Active Yellow Orb Arc Finding
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Active Yellow Orb")
	float ActiveYellowArcRadius;

	// Keep Track of if the Lightning Generation Effect is Turned On
	UPROPERTY(VisibleAnywhere, Category = "Active Yellow Orb")
	bool bLightningGeneratorActive;

	/** Character health */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	int Health;
	
	float RegainHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float PriorHealth = 100;

	/** Character health */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float GainHealthRate;

	/** Sets whether or not we should have the trail behind the player when blink happens **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blink")
	bool bBlinkTrailActivate;


	/** Declare overlap Begin function for Interaction*/
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** Declare overlap End function for Interaction*/
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/** Declare overlap Begin function for Finding Chargeable Objects*/
	UFUNCTION()
	void OnSphereOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** Declare overlap End function for Finding Chargeable Objects*/
	UFUNCTION()
	void OnSphereOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void TimelineFloatReturn(float value);

	UFUNCTION()
	void OnTimelineFinished();
	
	AOrbPickupParent* CurrentOrbPickup;

	//UPROPERTY(BlueprintReadOnly)
	//UOrbParent* ActiveOrb;

	

	/** Pointer to Current Lamp Post if in Range */
	ALampPost* CurrentLampPost;

	/** What Color Orb is currently being held*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EMainHandColorState CurrentMainHand;

	/** Capsule that detects overlap for Interaction */
	UPROPERTY(VisibleAnywhere, Category = "Trigger Capsule")
	class UCapsuleComponent* TriggerCapsule;

	/** Switch to "Orb" Color
		Ex. SwitchOrb("Blue")
	*/
	bool SwitchOrb(FString Orb);

	/**Character movement speed*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Movement: Walking")
	float SprintSpeedMultiplier;

	/** Function that is called to apply damage to this Actor */
	UFUNCTION(BlueprintCallable, Category = "Damage")
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	/** OnLandedEvent gets triggered once this Character hits or lands on the Ground */
	virtual void Landed(const FHitResult& Hit) override;

	virtual void FellOutOfWorld(const class UDamageType& dmgType);

	UFUNCTION(BlueprintCallable, Category = "Swap")
	void SwapToPlayerYellowOrb();

	UFUNCTION(BlueprintCallable, Category = "Swap")
	void SwapToPlayerRedOrb();

	UFUNCTION(BlueprintCallable, Category = "Swap")
	void SwapToPlayerBlueOrb();


	AGuidingLightGameMode* MyGameMode;

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	UFUNCTION(BlueprintCallable, Category = "Health")
	int GetHealth();

	


	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);
	
	/**Multiples maxwalkspeed by sprintspeedmultiplier*/
	void Sprint();
	/**Divides maxwalkspeed by sprintspeedmultiplier*/
	void StopSprinting();
	/** General On Action function*/
	UFUNCTION(BlueprintCallable, Category = "OnHell")
	void OnHell();

	/** On Main Hand Active function*/
	void OnMainActive();

	/** On Main Hand Active Button Release function*/
	void OnReleaseMainActive();

	FTimerHandle WaitForHealthRegainHandle;

	UFUNCTION()
		void SetCouldRegainHealth();

	bool bCouldRegainHealth;

	// Array that holds the current group of yellow orb receivers in range of the player
	TArray<AActor*> YellowOrbReceiversInRange;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	/** This is by default 600.0f **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float JumpHeight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FOrbStruct Orbs;

	
};

