// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "GuidingLightCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Orbs/YellowOrb.h"
#include "Orbs/RedOrb.h"
#include "Orbs/BlueOrb.h"
#include "Orbs/YellowOrbActiveReceiver.h"

#include "Kismet/KismetMathLibrary.h"

#include "Interactables/OrbPickups/OrbPickupParent.h"
#include "Interactables/OrbPickups/RedOrbPickup.h"
#include "Interactables/OrbPickups/YellowOrbPickup.h"
#include "Interactables/OrbPickups/BlueOrbPickup.h"
#include "Interactables/LampPosts/LampPost.h"
#include "Interactables/LampPosts/AmplifierLampPost.h"
#include "Interactables/LampPosts/ChargingLampPost.h"
#include "HelperFiles/DefinedDebugHelpers.h"

#include "Core/DamageTypes/DamageTypeFall.h"
#include "Core/GameModes/GuidingLightGameMode.h"



//////////////////////////////////////////////////////////////////////////
// AGuidingLightCharacter

AGuidingLightCharacter::AGuidingLightCharacter()
{
	V_LOG("GLCHAR START");
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	
	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	// Create Trigger Capsule for Interaction
	TriggerCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Trigger Capsule"));
	TriggerCapsule->InitCapsuleSize(55.f, 96.0f);;
	TriggerCapsule->SetCollisionProfileName(TEXT("Trigger"));
	TriggerCapsule->SetupAttachment(RootComponent);

	// Active Yellow Orb Arc Radius Size
	ActiveYellowArcRadius = 1200;

	// Create Trigger Sphere for Checking for Yellow Orb Receivers
	YellowOrbSearchSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Yellow Orb Search Sphere"));
	YellowOrbSearchSphere->InitSphereRadius(ActiveYellowArcRadius);
	YellowOrbSearchSphere->SetCollisionProfileName(TEXT("YellowOrbSearch"));
	YellowOrbSearchSphere->SetupAttachment(RootComponent);

	//Sets the mesh's shadow to off. Gonna do this in Blueprints for now till we have a specific model for the character
	//Mesh->SetCastShadow(false);

	//Create Crystal Post Processing Component for the Vignette
	CrystalPostProcess = CreateDefaultSubobject<UPostProcessComponent>(TEXT("CrystalPostProcessing"));
	CrystalPostProcess->SetupAttachment(FollowCamera);

	//Sprint variables
	SprintSpeedMultiplier = 2.0f;
	JumpHeight = 600.0f;

	
	// Set current light switch to null at start
	CurrentLampPost = NULL;

	// Set current Orb Pickup to Null at start
	CurrentOrbPickup = NULL;

	// Set default Main Hand Color (FOR NOW)
	CurrentMainHand = EMainHandColorState::Empty;

	// Set the starting health to 100
	Health = 100;

	//The player shouldn't have any orbs when they first start playing
	bHasAnOrb = false;

	
	GainHealthRate = 0.5f;

	bLightningGeneratorActive = false;

	// Orb Setup
	//ActiveOrb = CreateDefaultSubobject<UOrbParent>(TEXT("PlayerOrb"));
	//ActiveOrb->SetupAttachment(RootComponent);
	//ActiveOrb->OrbDeactivate();
	
	Orbs.PRedOrb = CreateDefaultSubobject<URedOrb>(TEXT("RedOrb"));
	Orbs.PBlueOrb = CreateDefaultSubobject<UBlueOrb>(TEXT("BlueOrb"));
	Orbs.PYellowOrb = CreateDefaultSubobject<UYellowOrb>(TEXT("YellowOrb"));
	
	Orbs.PRedOrb->SetupAttachment(RootComponent);
	Orbs.PYellowOrb->SetupAttachment(RootComponent);
	Orbs.PBlueOrb->SetupAttachment(RootComponent);

	Orbs.PRedOrb->DummyRoot->SetupAttachment(Orbs.PRedOrb);
	Orbs.PYellowOrb->DummyRoot->SetupAttachment(Orbs.PYellowOrb);
	Orbs.PBlueOrb->DummyRoot->SetupAttachment(Orbs.PBlueOrb);

	bRedOrbInInventory = false;
	bYellowOrbInInventory = false;
	bBlueOrbInInventory = false;

	bGrabbingOrb = false;
	bPullingLever = false;
	bRotatingWheel = false;
	bAiming = false;
	bOrangeJustice = false;
	bSwitchingOrb = false;

	AYellowOrbPickup* YOrbPickup = NULL;
	ARedOrbPickup* ROrbPickup = NULL;
	ABlueOrbPickup* BOrbPickup = NULL;

	OriginMovementMode = MOVE_Walking;


	V_LOG("GLCHAR END");
}


//////////////////////////////////////////////////////////////////////////
// Input

void AGuidingLightCharacter::SetCouldRegainHealth()
{
	bCouldRegainHealth = true;
}

void AGuidingLightCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

	// Main Button Active Binding (Left Mouse Click)
	PlayerInputComponent->BindAction("UseMainActive", IE_Pressed, this, &AGuidingLightCharacter::OnMainActive);
	PlayerInputComponent->BindAction("UseMainActive", IE_Released, this, &AGuidingLightCharacter::OnReleaseMainActive);

	// Interact Button Binding 'E'
	PlayerInputComponent->BindAction("Hell", IE_Pressed, this, &AGuidingLightCharacter::OnHell);


	PlayerInputComponent->BindAxis("MoveForward", this, &AGuidingLightCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGuidingLightCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AGuidingLightCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AGuidingLightCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AGuidingLightCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AGuidingLightCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AGuidingLightCharacter::OnResetVR);
	
}

void AGuidingLightCharacter::DoneGrabbingOrb()
{
	bGrabbingOrb = false;
	GetCharacterMovement()->MovementMode = MOVE_Walking;

}

void AGuidingLightCharacter::PutYellowPickupInHand()
{
	YOrbPickup->PutOrbInInventory();
}

void AGuidingLightCharacter::PutRedPickupInHand()
{
	ROrbPickup->PutOrbInInventory();
}

void AGuidingLightCharacter::PutBluePickupInHand()
{
	BOrbPickup->PutOrbInInventory();
}

void AGuidingLightCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// If Player is in range of LampPost, grab pointer to that Lamp
	if (OtherActor && (OtherActor != this) && OtherComp && OtherActor->GetClass()->IsChildOf(ALampPost::StaticClass()))
	{
		CurrentLampPost = Cast<ALampPost>(OtherActor);
	}

	// If Player is in range of Orb Pickup, grab point to that Pickup
	if (OtherActor && (OtherActor != this) && OtherComp && OtherActor->GetClass()->IsChildOf(AOrbPickupParent::StaticClass()))
	{
		CurrentOrbPickup = Cast<AOrbPickupParent>(OtherActor);
	}
}

void AGuidingLightCharacter::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// If Player is not in range of LampPost, set pointer to NULL
	if (OtherActor && (OtherActor != this) && OtherComp && OtherActor->GetClass()->IsChildOf(ALampPost::StaticClass()))
	{
		if (CurrentLampPost)
		{
			CurrentLampPost = NULL;
		}
		
	}

	// If Player is not in range of Orb Pickup, set pointer to NULL
	if (OtherActor && (OtherActor != this) && OtherComp && OtherActor->GetClass()->IsChildOf(AOrbPickupParent::StaticClass()))
	{
		if (CurrentOrbPickup)
		{
			//SCREENMSG2C("You've walked away from the orb pickup.", "Why would you do that?", Blue);
			CurrentOrbPickup = NULL;
		}

	}
}

void AGuidingLightCharacter::OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UYellowOrbActiveReceiver* ReceiverCheck = OtherActor->FindComponentByClass<UYellowOrbActiveReceiver>();
	if (ReceiverCheck != nullptr)
	{
		YellowOrbReceiversInRange.Add(OtherActor);
		//SCREENMSG2C("Yellow Orb Receiver in Range: ", OtherActor->GetActorLabel(), Yellow);
	}
}

void AGuidingLightCharacter::OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UYellowOrbActiveReceiver* ReceiverCheck = OtherActor->FindComponentByClass<UYellowOrbActiveReceiver>();
	if (ReceiverCheck != nullptr)
	{
		YellowOrbReceiversInRange.Remove(OtherActor);
		//SCREENMSG2C("Yellow Orb Receiver moved out of Range: ", OtherActor->GetActorLabel(), Yellow);
	}
}

void AGuidingLightCharacter::TimelineFloatReturn(float value)
{
}

void AGuidingLightCharacter::OnTimelineFinished()
{

}

void AGuidingLightCharacter::SwapToPlayerYellowOrb()
{
	if (Orbs.PYellowOrb && bYellowOrbInInventory && CurrentMainHand != EMainHandColorState::Yellow)
	{
		SwitchOrb("Yellow");
		bHasAnOrb = true;
	}
}

void AGuidingLightCharacter::SwapToPlayerRedOrb()
{
	if (Orbs.PRedOrb && bRedOrbInInventory && CurrentMainHand != EMainHandColorState::Red)
	{
		SwitchOrb("Red");
		bHasAnOrb = true;
	}
}

void AGuidingLightCharacter::SwapToPlayerBlueOrb()
{
	if (Orbs.PBlueOrb && bBlueOrbInInventory && CurrentMainHand != EMainHandColorState::Blue)
	{
		SwitchOrb("Blue");
		bHasAnOrb = true;
	}
}

// Switch Main Hand Orb Color
bool AGuidingLightCharacter::SwitchOrb(FString Orb)
{
	SCREENMSG(Orb);
	if (Orb == "Blue")
	{
		if (CurrentMainHand == EMainHandColorState::Yellow)
		{
			Orbs.PYellowOrb->OrbDeactivate();
		}
		else if (CurrentMainHand == EMainHandColorState::Red)
		{
			Orbs.PRedOrb->OrbDeactivate();
		}

		CurrentMainHand = EMainHandColorState::Blue;
		Orbs.PBlueOrb->OrbActivate();
	}
	else if (Orb == "Yellow")
	{
		if (CurrentMainHand == EMainHandColorState::Blue)
		{
			Orbs.PBlueOrb->OrbDeactivate();
		}
		else if (CurrentMainHand == EMainHandColorState::Red)
		{
			Orbs.PRedOrb->OrbDeactivate();
		}
		
		CurrentMainHand = EMainHandColorState::Yellow;
		Orbs.PYellowOrb->OrbActivate();
	}
	else if (Orb == "Red")
	{
		if (CurrentMainHand == EMainHandColorState::Blue)
		{
			Orbs.PBlueOrb->OrbDeactivate();
		}
		else if (CurrentMainHand == EMainHandColorState::Yellow)
		{
			Orbs.PYellowOrb->OrbDeactivate();
		}

		CurrentMainHand = EMainHandColorState::Red;
		Orbs.PRedOrb->OrbActivate();
	}
	else if (Orb == "Empty")
	{
		SCREENMSG("EMPTY");
		//ToggleOrb();
		CurrentMainHand = EMainHandColorState::Empty;
		//ActiveOrb = nullptr;
	}
	else
	{
		return false;
	}
	return true;
}

/** Function that is called to apply damage to this Actor */
float AGuidingLightCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// Call the base class - this will tell us how much damage to apply  
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (ActualDamage > 0.f)
	{
		
		Health -= ActualDamage;
		bCouldRegainHealth = false;
		if (GetWorld())
		{
			
			GetWorld()->GetTimerManager().SetTimer(WaitForHealthRegainHandle, this, &AGuidingLightCharacter::SetCouldRegainHealth, 2.f, false);
		}

		// If the damage depletes our health set our lifespan to zero - which will destroy the actor  
		if (Health <= 0)
		{
			//MyGameMode->ResetLevel();
			// Fucntion Reset
			// PLayer location
			// player health
			// Go to default orb
			Health = 0;
			SetLifeSpan(2.f);

			auto BRow = this->GetMovementComponent()->MovementState;
			
			this->GetCharacterMovement()->DisableMovement();
		}
	}
	return ActualDamage;
}

/** OnLandedEvent gets triggered once this Character hits or lands on the Ground */
void AGuidingLightCharacter::Landed(const FHitResult& Hit)
{
	/*
	float DownwardsVel = this->GetCharacterMovement()->Velocity.Z;
	SCREENMSG2C("Down Vec: ", FString::SanitizeFloat(DownwardsVel), Red);

	// If Velocity is less than -500 (More than a simple Jump) Apply a Falling Damage to the Character 
	if (DownwardsVel < -650)
	{
		// Creates a Damage Event with the Damage Type of Fall
		TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(UDamageTypeFall::StaticClass());
		FDamageEvent DamageEvent(ValidDamageTypeClass);
		
		
		if (GetWorld())
		{
			// Apply Damage to Character based on how much the Downwards Vel is
			if (DownwardsVel > -1600)
			{
				const float ActualDamage = this->TakeDamage(((DownwardsVel + 650) / -1600) * 100, DamageEvent, GetWorld()->GetFirstPlayerController(), this);
			}
			else
			{
				const float ActualDamage = this->TakeDamage( 100, DamageEvent, GetWorld()->GetFirstPlayerController(), this);
			}
		}
		SCREENMSG2C("HEALTH: ", FString::FromInt(Health), Green);

	}
	*/
}

void AGuidingLightCharacter::FellOutOfWorld(const UDamageType& dmgType)
{
	//Super::FellOutOfWorld(dmgType);

	TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(dmgType.StaticClass());
	FDamageEvent DamageEvent(ValidDamageTypeClass);


	if (GetWorld())
	{
		// Apply Damage to Character when KillZVolume is hit
		const float ActualDamage = this->TakeDamage(100, DamageEvent, GetWorld()->GetFirstPlayerController(), this);

	}

}


void AGuidingLightCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AGuidingLightCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AGuidingLightCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AGuidingLightCharacter::OnHell()
{
	//SCREENMSG("HELP!");
	if (CurrentLampPost)
	{
		if (CurrentLampPost->IsA(AAmplifierLampPost::StaticClass()))
		{
			AAmplifierLampPost* AMP = Cast<AAmplifierLampPost>(CurrentLampPost);
			AMP->TurnAmp();
		}
		//SCREENMSG("COLOR CHANGE");
		CurrentLampPost->ToggleColorChange();
	}

	/* Figure out what kind of orb the pickup is and set its "InInventory" Boolean to true */
	if (CurrentOrbPickup)
	{
		if (CurrentOrbPickup->bIsActive)
		{
			if (CurrentOrbPickup->IsA(AYellowOrbPickup::StaticClass()))
			{
				YOrbPickup = Cast<AYellowOrbPickup>(CurrentOrbPickup);

				bGrabbingOrb = true;

				FRotator PickupRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), CurrentOrbPickup->GetActorLocation());
				SetActorRotation(FRotator(GetMesh()->GetComponentRotation().Pitch, PickupRotation.Yaw, GetMesh()->GetComponentRotation().Roll));

				GetCharacterMovement()->DisableMovement();


				GetWorld()->GetTimerManager().SetTimer(GrabbingOrbDelayTimerHandle, this, &AGuidingLightCharacter::DoneGrabbingOrb, 2.0f, false);
				GetWorld()->GetTimerManager().SetTimer(PickupOrbToHandDelayTimerHandle, this, &AGuidingLightCharacter::PutYellowPickupInHand, 1.5f, false);


			}
			else if (CurrentOrbPickup->IsA(ARedOrbPickup::StaticClass()))
			{
				ROrbPickup = Cast<ARedOrbPickup>(CurrentOrbPickup);
				bGrabbingOrb = true;

				FRotator PickupRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), CurrentOrbPickup->GetActorLocation());
				SetActorRotation(FRotator(GetMesh()->GetComponentRotation().Pitch, PickupRotation.Yaw, GetMesh()->GetComponentRotation().Roll));

				GetCharacterMovement()->DisableMovement();


				GetWorld()->GetTimerManager().SetTimer(GrabbingOrbDelayTimerHandle, this, &AGuidingLightCharacter::DoneGrabbingOrb, 2.0f, false);
				GetWorld()->GetTimerManager().SetTimer(PickupOrbToHandDelayTimerHandle, this, &AGuidingLightCharacter::PutRedPickupInHand, 1.5f, false);
			}
			else if (CurrentOrbPickup->IsA(ABlueOrbPickup::StaticClass()))
			{
				BOrbPickup = Cast<ABlueOrbPickup>(CurrentOrbPickup);
				bGrabbingOrb = true;

				FRotator PickupRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), CurrentOrbPickup->GetActorLocation());
				SetActorRotation(FRotator(GetMesh()->GetComponentRotation().Pitch, PickupRotation.Yaw, GetMesh()->GetComponentRotation().Roll));

				GetCharacterMovement()->DisableMovement();


				GetWorld()->GetTimerManager().SetTimer(GrabbingOrbDelayTimerHandle, this, &AGuidingLightCharacter::DoneGrabbingOrb, 2.0f, false);
				GetWorld()->GetTimerManager().SetTimer(PickupOrbToHandDelayTimerHandle, this, &AGuidingLightCharacter::PutBluePickupInHand, 1.5f, false);
			}
			else if (CurrentOrbPickup->IsA(AOrbPickupParent::StaticClass()))
			{
				AOrbPickupParent* POrbPickup = Cast<AOrbPickupParent>(CurrentOrbPickup);
				POrbPickup->PutOrbInInventory();
			}
		}
		
	}
}

void AGuidingLightCharacter::OnMainActive()
{

	if (CurrentLampPost)
	{
		// Only Activate if LampPost is not Retaining
		if (!CurrentLampPost->IsA(ARetainingLampPost::StaticClass()))
		{
			V_LOG("Is Not Retaining");
			// Interaction for Lamp Post
			CurrentLampPost->ToggleColorChange();
		}
		else
		{
			if (CurrentLampPost->IsA(AChargingLampPost::StaticClass()))
			{
				V_LOG("Is Not Retaining");
				CurrentLampPost->ToggleColorChange();
			}
			else
			{
				V_LOG("Is Retaining");
			}
		}
	}

	if (Orbs.PRedOrb->bIsActive)
	{
		Orbs.PRedOrb->AimActive();
	}
	
	else if (Orbs.PBlueOrb->bIsActive)
	{
		Orbs.PBlueOrb->AimActive();
	}
	else if (Orbs.PYellowOrb->bIsActive)
	{
		ActivateActiveYellowOrbGenerator();
		bLightningGeneratorActive = true;
	}
}

void AGuidingLightCharacter::OnReleaseMainActive()
{
	if (CurrentLampPost)
	{
		if (CurrentLampPost->IsA(AChargingLampPost::StaticClass()))
		{
			AChargingLampPost* ChargingLP = Cast<AChargingLampPost>(CurrentLampPost);
			ChargingLP->ReleaseCharge();
		}
	}
	if (Orbs.PRedOrb->bIsActive)
	{
		Orbs.PRedOrb->FireActive();
	}
	else if (Orbs.PBlueOrb->bIsActive)
	{
		Orbs.PBlueOrb->FireActive();
	}
	else if (Orbs.PYellowOrb->bIsActive)
	{
		DeactivateActiveYellowOrbGenerator();
		bLightningGeneratorActive = false;
	}
	
}

void AGuidingLightCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld())
	{
		MyGameMode = Cast<AGuidingLightGameMode>(GetWorld()->GetAuthGameMode());
	}

	SCREENMSG2C("Current Orb Color is:  ", GETENUMSTRING("EMainHandColorState", CurrentMainHand), Red);

	// Bind trigger events
	TriggerCapsule->OnComponentBeginOverlap.AddDynamic(this, &AGuidingLightCharacter::OnOverlapBegin);
	TriggerCapsule->OnComponentEndOverlap.AddDynamic(this, &AGuidingLightCharacter::OnOverlapEnd);

	YellowOrbSearchSphere->OnComponentBeginOverlap.AddDynamic(this, &AGuidingLightCharacter::OnSphereOverlapBegin);
	YellowOrbSearchSphere->OnComponentEndOverlap.AddDynamic(this, &AGuidingLightCharacter::OnSphereOverlapEnd);

	Health = 100;
	RegainHealth = 0;

	Orbs.PRedOrb->OrbDeactivate();
	Orbs.PBlueOrb->OrbDeactivate();
	Orbs.PYellowOrb->OrbDeactivate();

	//GetWorld()->SpawnActor<TSubclassOf<AActor>>(YellowOrbLightningGenerator, GetActorLocation(), GetActorRotation());
}

void AGuidingLightCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bCouldRegainHealth)
	{
		if (Health > 100)
		{
			Health = 100;
		}
		if (Health == 100)
		{
			bCouldRegainHealth = false;
		}
		if (RegainHealth > 1)
		{
			Health += 5;
			SCREENMSG2C("HEALTH: ", FString::FromInt(Health), Green);
			RegainHealth = 0;
		}
		RegainHealth += DeltaSeconds * GainHealthRate;
		V_LOGF("RegainHealth", RegainHealth);
	}
	if (bLightningGeneratorActive)
	{
		if (YellowOrbReceiversInRange.Num() > 0)
		{
			FVector SetLightningTargetTo;
			AActor* FinalReceiver = nullptr;
			UYellowOrbActiveReceiver* RecComponent = nullptr;
			float MaxDistance = 99999.99;
			for (AActor* Receiver : YellowOrbReceiversInRange)
			{
				float Distance = Receiver->GetDistanceTo(this);
				if (Distance < MaxDistance)
				{
					MaxDistance = Receiver->GetDistanceTo(this);
					RecComponent = Receiver->FindComponentByClass<UYellowOrbActiveReceiver>();
					SetLightningTargetTo = Receiver->GetTargetLocation();
					V_LOG("Should Be Setting Location: " + SetLightningTargetTo.ToString());
					FinalReceiver = Receiver;
				}
			}
			SetActiveYellowOrbGeneratorToDirectedMode(FinalReceiver);
			RecComponent->ReceiveTickOfEnergy();
		}
		else
		{
			SetActiveYellowOrbGeneratorToUndirectedMode();
		}
	}
}



void AGuidingLightCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AGuidingLightCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AGuidingLightCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AGuidingLightCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AGuidingLightCharacter::Sprint()
{
	GetCharacterMovement()->MaxWalkSpeed *= SprintSpeedMultiplier;

}

void AGuidingLightCharacter::StopSprinting()
{
	GetCharacterMovement()->MaxWalkSpeed /= SprintSpeedMultiplier;
}

int AGuidingLightCharacter::GetHealth()
{
	return Health;
}

