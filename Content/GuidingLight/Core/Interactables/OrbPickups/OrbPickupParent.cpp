// Fill out your copyright notice in the Description page of Project Settings.


#include "OrbPickupParent.h"


AOrbPickupParent::AOrbPickupParent()
{
	PickupCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("PickupCollisionBox"));
	PickupCollisionBox->SetGenerateOverlapEvents(true);
	PickupCollisionBox->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	PickupCollisionBox->SetRelativeScale3D(FVector(2.75f, 2.75f, 2.0f));
	PickupCollisionBox->SetupAttachment(DummyRoot);
	OrbLightCollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void AOrbPickupParent::PutOrbInInventory()
{
	Destroy();
}
