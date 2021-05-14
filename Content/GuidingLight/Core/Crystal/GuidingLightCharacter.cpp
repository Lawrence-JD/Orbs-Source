// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "GuidingLightCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Orbs/YellowOrb.h"
#include "Orbs/RedOrb.h"
#include "Orbs/BlueOrb.h"

#include "Interactables/OrbPickups/OrbPickupParent.h"
#include "Interactables/OrbPickups/RedOrbPickup.h"
#include "Interactables/OrbPickups/YellowOrbPickup.h"
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
	
	//Create the Player's Yellow Orb Component
	PlayerYellowOrb = CreateDefaultSubobject<UChildActorComponent>(TEXT("PlayerYellowOrb"));
	PlayerYellowOrb->SetupAttachment(RootComponent);
	PlayerYellowOrb->SetChildActorClass(AYellowOrb::StaticClass());
	PlayerYellowOrb->bIsActive = true;

	//Create the Player's Red Orb Component
	PlayerRedOrb = CreateDefaultSubobject<UChildActorComponent>(TEXT("PlayerRedOrb"));
	PlayerRedOrb->SetupAttachment(RootComponent);
	PlayerRedOrb->SetChildActorClass(ARedOrb::StaticClass());
	PlayerRedOrb->bIsActive = true;

	//Create the Player's Blue Orb Component
	PlayerBlueOrb = CreateDefaultSubobject<UChildActorComponent>(TEXT("PlayerBlueOrb"));
	PlayerBlueOrb->SetupAttachment(RootComponent);
	PlayerBlueOrb->SetChildActorClass(ABlueOrb::StaticClass());
	PlayerBlueOrb->bIsActive = true;
	

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	// Create Trigger Capsule for Interaction
	TriggerCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Trigger Capsule"));
	TriggerCapsule->InitCapsuleSize(55.f, 96.0f);;
	TriggerCapsule->SetCollisionProfileName(TEXT("Trigger"));
	TriggerCapsule->SetupAttachment(RootComponent);

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
	//PlayerInputComponent->BindAction("Hell", IE_Pressed, this, &AGuidingLightCharacter::OnHell);


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
	if (OtherActor && (OtherActor != this) && OtherComp)
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
			SCREENMSG2C("You've walked away from the orb pickup.", "Why would you do that?", Blue);
			CurrentOrbPickup = NULL;
		}

	}
}

void AGuidingLightCharacter::TimelineFloatReturn(float value)
{
}

void AGuidingLightCharacter::OnTimelineFinished()
{

}

void AGuidingLightCharacter::TogglePlayerYellowOrb()
{
	
	if (PlayerYellowOrb)
	{
		if (PlayerYellowOrb->bIsActive)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Deactivating Yellow Orb!")));
			//Need to figure out how to call Child Actor specifically/Cast to it

			
			//Keeping this around in case we need to use specific functions from a child actor in the future.
			//This is SUPER useful
			AYellowOrb* TempYActorOrb = Cast<AYellowOrb>(PlayerYellowOrb->GetChildActor());

			AYellowOrb* TempYOrb = Cast<AYellowOrb>(PlayerYellowOrb);

			if (TempYActorOrb && TempYActorOrb->IsA(AYellowOrb::StaticClass()))
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString::Printf(TEXT("GetChildActor is Yellow Orb")));
			}

			if (TempYOrb && TempYOrb->IsA(AYellowOrb::StaticClass())) //TempYOrb is NULL because you can't cast AChildActorComponent -> AYellowOrb
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString::Printf(TEXT("PlayerYellowOrb is Yellow Orb!")));
			}

			// THIS WILL WORK
			TempYActorOrb->OrbDeactivate();
			PlayerYellowOrb->bIsActive = false;
			
			

			// THIS WILL ALSO WORK
			/*
			PlayerYellowOrb->Deactivate();
			PlayerYellowOrb->SetVisibility(false);
			*/
			
        }
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Activating Yellow Orb!")));
			
			AYellowOrb* TempYActorOrb = Cast<AYellowOrb>(PlayerYellowOrb->GetChildActor());

			AYellowOrb* TempYOrb = Cast<AYellowOrb>(PlayerYellowOrb);

			if (TempYActorOrb && TempYActorOrb->IsA(AYellowOrb::StaticClass()))
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString::Printf(TEXT("GetChildActor is Yellow Orb")));
			}

			if (TempYOrb && TempYOrb->IsA(AYellowOrb::StaticClass())) //TempYOrb is NULL because you can't cast AChildActorComponent -> AYellowOrb
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString::Printf(TEXT("PlayerYellowOrb is Yellow Orb!")));
			}

			// THIS WILL WORK
			TempYActorOrb->OrbActivate();
			PlayerYellowOrb->bIsActive = true;

		}

	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Uhh... Fuck!")));
		}
	}
	

}

void AGuidingLightCharacter::TogglePlayerRedOrb()
{
	if (PlayerRedOrb)
	{
		if (PlayerRedOrb->bIsActive)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Deactivating Red!")));

			ARedOrb* TempRActorOrb = Cast<ARedOrb>(PlayerRedOrb->GetChildActor());

			ARedOrb* TempROrb = Cast<ARedOrb>(PlayerRedOrb);

			if (TempRActorOrb && TempRActorOrb->IsA(ARedOrb::StaticClass()))
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString::Printf(TEXT("GetChildActor is Red Orb")));
			}

			if (TempROrb && TempROrb->IsA(ARedOrb::StaticClass())) //TempYOrb is NULL because you can't cast AChildActorComponent -> AYellowOrb
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString::Printf(TEXT("PlayerRedOrb is Red Orb!")));
			}

			// THIS WILL WORK
			TempRActorOrb->OrbDeactivate();
			PlayerRedOrb->bIsActive = false;
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Activating Red!")));

			ARedOrb* TempRActorOrb = Cast<ARedOrb>(PlayerRedOrb->GetChildActor());

			ARedOrb* TempROrb = Cast<ARedOrb>(PlayerRedOrb);

			if (TempRActorOrb && TempRActorOrb->IsA(ARedOrb::StaticClass()))
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString::Printf(TEXT("GetChildActor is Red Orb")));
			}

			if (TempROrb && TempROrb->IsA(ARedOrb::StaticClass())) //TempYOrb is NULL because you can't cast AChildActorComponent -> AYellowOrb
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString::Printf(TEXT("PlayerYellowOrb is Red Orb!")));
			}

			// THIS WILL WORK
			TempRActorOrb->OrbActivate();
			PlayerRedOrb->bIsActive = true;
		}

	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Uhh... Fuck! There is no Red Orb!")));
		}
	}
}

void AGuidingLightCharacter::TogglePlayerBlueOrb()
{
	if (PlayerBlueOrb)
	{
		if (PlayerBlueOrb->bIsActive)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Deactivating Blue!")));

			ABlueOrb* TempBActorOrb = Cast<ABlueOrb>(PlayerBlueOrb->GetChildActor());

			ABlueOrb* TempBOrb = Cast<ABlueOrb>(PlayerBlueOrb);

			if (TempBActorOrb && TempBActorOrb->IsA(ABlueOrb::StaticClass()))
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString::Printf(TEXT("GetChildActor is Blue Orb")));
			}

			if (TempBOrb && TempBOrb->IsA(ABlueOrb::StaticClass())) //TempYOrb is NULL because you can't cast AChildActorComponent -> AYellowOrb
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString::Printf(TEXT("PlayerBlueOrb is Blue Orb!")));
			}

			// THIS WILL WORK
			TempBActorOrb->OrbDeactivate();
			PlayerBlueOrb->bIsActive = false;
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Activating Blue!")));

			ABlueOrb* TempBActorOrb = Cast<ABlueOrb>(PlayerBlueOrb->GetChildActor());

			ABlueOrb* TempBOrb = Cast<ABlueOrb>(PlayerBlueOrb);

			if (TempBActorOrb && TempBActorOrb->IsA(ABlueOrb::StaticClass()))
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString::Printf(TEXT("GetChildActor is Blue Orb")));
			}

			if (TempBOrb && TempBOrb->IsA(ABlueOrb::StaticClass())) //TempYOrb is NULL because you can't cast AChildActorComponent -> AYellowOrb
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString::Printf(TEXT("PlayerBlueOrb is Blue Orb!")));
			}

			// THIS WILL WORK
			TempBActorOrb->OrbActivate();
			PlayerBlueOrb->bIsActive = true;


		}

	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Uhh... Fuck! There is no Blue Orb!")));
		}
	}
}

void AGuidingLightCharacter::SwapToPlayerYellowOrb()
{
	if (PlayerYellowOrb && bYellowOrbInInventory)
	{
		if (!(PlayerYellowOrb->bIsActive))
		{
			if (PlayerRedOrb->bIsActive)
			{
				TogglePlayerRedOrb();
			}

			if (PlayerBlueOrb->bIsActive)
			{
				StopSprinting();
				TogglePlayerBlueOrb();
			}

			TogglePlayerYellowOrb();
			CurrentMainHand = EMainHandColorState::Yellow;
			bHasAnOrb = true;
		}

	}
}

void AGuidingLightCharacter::SwapToPlayerRedOrb()
{
	if (PlayerRedOrb && bRedOrbInInventory)
	{
		if (!(PlayerRedOrb->bIsActive))
		{
			if (PlayerYellowOrb->bIsActive)
			{
				TogglePlayerYellowOrb();
			}

			if (PlayerBlueOrb->bIsActive)
			{
				StopSprinting();
				TogglePlayerBlueOrb();
			}

			TogglePlayerRedOrb();
			CurrentMainHand = EMainHandColorState::Red;
			bHasAnOrb = true;
		}

	}
}

void AGuidingLightCharacter::SwapToPlayerBlueOrb()
{
	if (PlayerBlueOrb && bBlueOrbInInventory)
	{
		if (!(PlayerBlueOrb->bIsActive))
		{
			if (PlayerYellowOrb->bIsActive)
			{
				TogglePlayerYellowOrb();
			}
			if (PlayerRedOrb->bIsActive)
			{
				TogglePlayerRedOrb();
			}

			Sprint();
			TogglePlayerBlueOrb();
			CurrentMainHand = EMainHandColorState::Blue;
			bHasAnOrb = true;
		}

	}
}

/** Function that is called to apply damage to this Actor */
float AGuidingLightCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	SCREENMSG("TAKING DMG");
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
	SCREENMSG("HELP!");
	if (CurrentLampPost)
	{
		if (CurrentLampPost->IsA(AAmplifierLampPost::StaticClass()))
		{
			AAmplifierLampPost* AMP = Cast<AAmplifierLampPost>(CurrentLampPost);
			AMP->TurnAmp();
		}
	}

	/* Figure out what kind of orb the pickup is and set its "InInventory" Boolean to true */
	if (CurrentOrbPickup)
	{
		if (CurrentOrbPickup->IsA(AYellowOrbPickup::StaticClass()))
		{
			AYellowOrbPickup* YOrbPickup = Cast<AYellowOrbPickup>(CurrentOrbPickup);
			YOrbPickup->PutOrbInInventory();
		}
		else if (CurrentOrbPickup->IsA(ARedOrbPickup::StaticClass()))
		{
			ARedOrbPickup* ROrbPickup = Cast<ARedOrbPickup>(CurrentOrbPickup);
			ROrbPickup->PutOrbInInventory();
		}
		else if (CurrentOrbPickup->IsA(AOrbPickupParent::StaticClass()))
		{
			AOrbPickupParent* POrbPickup = Cast<AOrbPickupParent>(CurrentOrbPickup);
			POrbPickup->PutOrbInInventory();
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

	if (PlayerRedOrb->bIsActive)
	{
		ARedOrb* TempRActorOrb = Cast<ARedOrb>(PlayerRedOrb->GetChildActor());
		TempRActorOrb->AimActive();
	}

	else if (PlayerBlueOrb->bIsActive)
	{
		ABlueOrb* TempBActorOrb = Cast<ABlueOrb>(PlayerBlueOrb->GetChildActor());
		TempBActorOrb->AimActive();
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
	if (PlayerRedOrb->bIsActive)
	{
		ARedOrb* TempRActorOrb = Cast<ARedOrb>(PlayerRedOrb->GetChildActor());
		TempRActorOrb->FireActive();
	}
	else if (PlayerBlueOrb->bIsActive)
	{
		ABlueOrb* TempBActorOrb = Cast<ABlueOrb>(PlayerBlueOrb->GetChildActor());
		TempBActorOrb->FireActive();
	}
}


// Switch Main Hand Orb Color
bool AGuidingLightCharacter::SwitchOrb(FString Orb)
{
	if (Orb == "Blue")
	{
		CurrentMainHand = EMainHandColorState::Blue;
	}
	else if (Orb == "Yellow")
	{
		CurrentMainHand = EMainHandColorState::Yellow;
	}
	else if (Orb == "Red")
	{
		CurrentMainHand = EMainHandColorState::Red;
	}
	else if (Orb == "Empty")
	{
		CurrentMainHand = EMainHandColorState::Empty;
	}
	else
	{
		return false;
	}

	return true;
}

void AGuidingLightCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerYellowOrb->RegisterComponent();
	PlayerYellowOrb->InitializeComponent();

	PlayerRedOrb->RegisterComponent();
	PlayerRedOrb->InitializeComponent();

	PlayerBlueOrb->RegisterComponent();
	PlayerBlueOrb->InitializeComponent();

	TogglePlayerRedOrb();
	TogglePlayerBlueOrb();
	TogglePlayerYellowOrb();

	if (GetWorld())
	{
		MyGameMode = Cast<AGuidingLightGameMode>(GetWorld()->GetAuthGameMode());
	}

	SCREENMSG2C("Current Orb Color is:  ", GETENUMSTRING("EMainHandColorState", CurrentMainHand), Red);

	// Bind trigger events
	TriggerCapsule->OnComponentBeginOverlap.AddDynamic(this, &AGuidingLightCharacter::OnOverlapBegin);
	TriggerCapsule->OnComponentEndOverlap.AddDynamic(this, &AGuidingLightCharacter::OnOverlapEnd);

	Health = 100;
	RegainHealth = 0;
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

