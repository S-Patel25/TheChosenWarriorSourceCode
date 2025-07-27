// All Rights Reserved


#include "Items/Weapons/WarriorWeaponBase.h"
#include "Components/BoxComponent.h"
#include "WarriorFunctionLibrary.h"

#include "WarriorDebugHelpers.h"

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
	weaponCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnCollisionBoxBeginOverlap); //binding collision event
	weaponCollisionBox->OnComponentEndOverlap.AddUniqueDynamic(this, &ThisClass::OnCollisionBoxEndOverlap);

}

void AWarriorWeaponBase::OnCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APawn* weaponOwningPawn = GetInstigator<APawn>();

	checkf(weaponOwningPawn, TEXT("Forgot to assign instigator as the owning pawn for the weapon!"));

	if (APawn* hitPawn = Cast<APawn>(OtherActor))
	{
		if (UWarriorFunctionLibrary::isTargetPawnHostile(weaponOwningPawn, hitPawn))
		{
			OnWeaponHitTarget.ExecuteIfBound(OtherActor);
		}
	}
}

void AWarriorWeaponBase::OnCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APawn* weaponOwningPawn = GetInstigator<APawn>();

	checkf(weaponOwningPawn, TEXT("Forgot to assign instigator as the owning pawn for the weapon!"));

	if (APawn* hitPawn = Cast<APawn>(OtherActor))
	{
		if (UWarriorFunctionLibrary::isTargetPawnHostile(weaponOwningPawn, hitPawn))
		{
			OnWeaponPulledFromTarget.ExecuteIfBound(OtherActor);
		}
	}
}
