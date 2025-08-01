// All Rights Reserved


#include "AbilitySystem/HeroGameplayAbility_TargetLock.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Characters/HeroCharacter.h"
#include "Kismet/GameplayStatics.h"

#include "WarriorDebugHelpers.h"
//doing most of the target lock functionality in C++ as its more performant considering this will be handled frame by frame (switching targets, animation poses, etc.)
void UHeroGameplayAbility_TargetLock::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	tryLockOnTarget();

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UHeroGameplayAbility_TargetLock::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	cleanUp();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UHeroGameplayAbility_TargetLock::tryLockOnTarget()
{
	getAvailableActorsToLock();

	if (availableActorsToLock.IsEmpty())
	{
		cancelTargetLockAbility(); //if no targets found then we cancel
		return;
	}

	currentLockedActor = getNearestTargetFromAvailableActors(availableActorsToLock); //has a valid lock on target at this point

	if (currentLockedActor)
	{
		Debug::Print(currentLockedActor->GetActorNameOrLabel());
	}
	else
	{
		cancelTargetLockAbility();
	}
}

void UHeroGameplayAbility_TargetLock::getAvailableActorsToLock()
{
	TArray<FHitResult> boxTraceHits; //will have the enemies we can target lock
	 
	UKismetSystemLibrary::BoxTraceMultiForObjects( //box trace mulitple to get enemies
		getHeroCharacterFromActorInfo(),
		getHeroCharacterFromActorInfo()->GetActorLocation(),
		getHeroCharacterFromActorInfo()->GetActorLocation() + getHeroCharacterFromActorInfo()->GetActorForwardVector() * boxTraceDistance,
		traceBoxSize / 2.f, //divide as its half size
		getHeroCharacterFromActorInfo()->GetActorForwardVector().ToOrientationRotator(), //where box is facing
		boxTraceChannel,
		false,
		TArray<AActor*>(),
		bShowPersistentShape ? EDrawDebugTrace::Persistent : EDrawDebugTrace::None, //should we see shape?
		boxTraceHits,
		true
	);

	for (const FHitResult& traceHit : boxTraceHits) //loop through results
	{
		if (AActor* hitActor = traceHit.GetActor())
		{
			if (hitActor != getHeroCharacterFromActorInfo()) //means its can be locked on
			{
				availableActorsToLock.AddUnique(hitActor);

				Debug::Print(hitActor->GetActorNameOrLabel());
			}
		}
	}
}

AActor* UHeroGameplayAbility_TargetLock::getNearestTargetFromAvailableActors(const TArray<AActor*>& inAvailableActors)
{
	float closestDistance = 0.f;
	return UGameplayStatics::FindNearestActor(getHeroCharacterFromActorInfo()->GetActorLocation(), inAvailableActors, closestDistance); //get nearest actor to target lock onto
}

void UHeroGameplayAbility_TargetLock::cancelTargetLockAbility()
{
	CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
}

void UHeroGameplayAbility_TargetLock::cleanUp()
{
	availableActorsToLock.Empty(); //empty array (perfomant)

	currentLockedActor = nullptr; //good to have this
}
