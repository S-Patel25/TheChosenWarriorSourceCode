// All Rights Reserved


#include "Items/PickUps/WarriorPickUpBase.h"
#include "Components/SphereComponent.h"

AWarriorPickUpBase::AWarriorPickUpBase()
{
	PrimaryActorTick.bCanEverTick = false;

	pickUpCollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Pick up collision sphere"));
	SetRootComponent(pickUpCollisionSphere);
	pickUpCollisionSphere->InitSphereRadius(50.f);
	pickUpCollisionSphere->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::onPickUpCollisionSphereBeginOverlap);
}

void AWarriorPickUpBase::onPickUpCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}
