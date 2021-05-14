// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/TimelineComponent.h"
#include "Components/PostProcessComponent.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Core/Interactables/LampPosts/LampPost.h"
#include "Core/Interactables/OrbPickups/OrbPickupParent.h"
#include "GuidingLightCharacter.generated.h"

UENUM()
enum class EMainHandColorState
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

class ALampPost;
class AGuidingLightGameMode;

UCLASS(config=Game)
class AGuidingLightCharacter : public ACharacter
{
	GENERATED_BODY()
public:
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Orb", meta = (AllowPrivateAccess = "true"));
	class AYellowOrb* PYellowOrb;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UPostProcessComponent* CrystalPostProcess;

	AGuidingLightCharacter();

	//The Yellow Orb Child Actor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Orb, meta = (AllowPrivateAccess = "true"))
	class UChildActorComponent* PlayerYellowOrb;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Orb, meta = (AllowPrivateAccess = "true"))
	class UChildActorComponent* PlayerRedOrb;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Orb, meta = (AllowPrivateAccess = "true"))
	class UChildActorComponent* PlayerBlueOrb;

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

	//Whether the player has at least one orb in their inventory or not
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = 
		Orb)
	bool bHasAnOrb;

	/** Character health */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	int Health;
	
	float RegainHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float PriorHealth = 100;

	/** Character health */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float GainHealthRate;


	/** Declare overlap Begin function for Interaction*/
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** Declare overlap End function for Interaction*/
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void TimelineFloatReturn(float value);

	UFUNCTION()
	void OnTimelineFinished();
	
	AOrbPickupParent* CurrentOrbPickup;

	/** Pointer to Current Lamp Post if in Range */
	ALampPost* CurrentLampPost;

	/** What Color Orb is currently being held*/
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

	/** Activates the YellowOrb if it's deactivated, and activates it if it is deactivated */
	UFUNCTION(BlueprintCallable, Category = "Activate")
	void TogglePlayerYellowOrb();

	UFUNCTION(BlueprintCallable, Category = "Activate")
	void TogglePlayerRedOrb();

	UFUNCTION(BlueprintCallable, Category = "Activate")
	void TogglePlayerBlueOrb();

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
};

