// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/ConstructorHelpers.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "YellowOrbActiveReceiver.h"
#include "Generator_YellowOrb.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GUIDINGLIGHT_API UGenerator_YellowOrb : public USceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	UGenerator_YellowOrb();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class USceneComponent* DummyRoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CollisionSphereSize=600;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float AmountOfCharge;
	float PowerIncrement;
	bool yOrbCollision;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Light, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* YellowLightCollisionSphere;
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite)
		float PowerDecrease;

	// The Orb's Mesh Component that physically represents the orb
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* GeneratorMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable, Category = "Generator")
		bool hasPower();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UYellowOrbActiveReceiver* MyReciever;
	

};
