// All Rights Reserved


#include "Items/WarriorProjectileBase.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "WarriorFunctionLibrary.h"
#include "ChosenWarriorGameplayTags.h"
#include "AbilitySystemBlueprintLibrary.h"

#include "WarriorDebugHelpers.h"

AWarriorProjectileBase::AWarriorProjectileBase()
{
	PrimaryActorTick.bCanEverTick = false;

	projectileCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("projectileCollisionBox"));
	SetRootComponent(projectileCollisionBox); //needs to be root!
	projectileCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	projectileCollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	projectileCollisionBox->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	projectileCollisionBox->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	projectileCollisionBox->OnComponentHit.AddUniqueDynamic(this, &ThisClass::OnProjectileHit);
	projectileCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnProjectileBeginOverlap);

	projectileNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("projectileNiagaraComponent"));
	projectileNiagaraComponent->SetupAttachment(GetRootComponent());

	projectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("projectileMovementComponent"));
	projectileMovementComp->InitialSpeed = 700.f;
	projectileMovementComp->MaxSpeed = 900.f;
	projectileMovementComp->Velocity = FVector(1.f, 0.f, 0.f);
	projectileMovementComp->ProjectileGravityScale = 0.f; //always flying

	InitialLifeSpan = 4.f; //will be destroyed after 4 seconds

}

void AWarriorProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	if (projectileDamagePolicy == EProjectileDamagePolicy::onBeginOverlap)
	{
		projectileCollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); //need this as everything is set to block by default
	}
	
}

void AWarriorProjectileBase::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	BP_OnSpawnProjectileFX(Hit.ImpactPoint);

	APawn* hitPawn = Cast<APawn>(OtherActor);

	if (!hitPawn || !UWarriorFunctionLibrary::isTargetPawnHostile(GetInstigator(), hitPawn))
	{
		Destroy();
		return;
	}

	bool bIsValidBlock = false;
	const bool bIsPlayerBlocking = UWarriorFunctionLibrary::NativeDoesActorHaveTag(hitPawn, ChosenWarriorGameplayTags::Player_Status_Blocking);

	if (bIsPlayerBlocking)
	{
		bIsValidBlock = UWarriorFunctionLibrary::isValidBlock(this, hitPawn); //check for block first
	}

	FGameplayEventData Data;
	Data.Instigator = this;
	Data.Target = hitPawn;

	if (bIsValidBlock)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			hitPawn,
			ChosenWarriorGameplayTags::Player_Event_SuccessfulBlock,
			Data
		);
	}
	else
	{
		HandleApplyProjectileDamage(hitPawn, Data);
	}

	Destroy();
}

void AWarriorProjectileBase::OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void AWarriorProjectileBase::HandleApplyProjectileDamage(APawn* inHitPawn, const FGameplayEventData& inPayload)
{
	checkf(projectileDamageEffectSpecHandle.IsValid(), TEXT("Forgot to assign valid spec handle!"));
	const bool bWasApplied = UWarriorFunctionLibrary::applyGameplayEffectSpecHandleToTargetActor(GetInstigator(), inHitPawn, projectileDamageEffectSpecHandle);

	if (bWasApplied)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			inHitPawn,
			ChosenWarriorGameplayTags::Shared_Event_SpawnProjectile,
			inPayload
		);
	}
}


