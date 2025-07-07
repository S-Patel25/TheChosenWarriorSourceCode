// All Rights Reserved


#include "Items/Weapons/WarriorWeaponBase.h"
#include "Components/BoxComponent.h"

AWarriorWeaponBase::AWarriorWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	weaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon Mesh Component"));
	SetRootComponent(weaponMesh);

	weaponCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Collision Box"));
	weaponCollisionBox->SetupAttachment(GetRootComponent());
	weaponCollisionBox->SetBoxExtent(FVector(20.f));
	weaponCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}
