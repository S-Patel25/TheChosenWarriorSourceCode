// All Rights Reserved


#include "AbilitySystem/HeroGameplayAbility_PickUpStones.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Characters/HeroCharacter.h"
#include "Items/PickUps/WarriorStoneBase.h"
#include "Components/UI/HeroUIComponent.h"

void UHeroGameplayAbility_PickUpStones::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	getHeroUIComponentFromActorInfo()->onStoneInteracted.Broadcast(true); //broadcast true when walk over stone

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UHeroGameplayAbility_PickUpStones::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	getHeroUIComponentFromActorInfo()->onStoneInteracted.Broadcast(false);

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UHeroGameplayAbility_PickUpStones::collectStones()
{
	collectedStones.Empty();

	TArray<FHitResult> traceHits;

	UKismetSystemLibrary::BoxTraceMultiForObjects(
		getHeroCharacterFromActorInfo(),
		getHeroCharacterFromActorInfo()->GetActorLocation(),
		getHeroCharacterFromActorInfo()->GetActorLocation() + -getHeroCharacterFromActorInfo()->GetActorUpVector() * boxTraceDistance,
		traceBoxSize / 2.f,
		(-getHeroCharacterFromActorInfo()->GetActorUpVector()).ToOrientationRotator(),
		stoneTraceChannel,
		false,
		TArray<AActor*>(),
		bDrawDebugShape ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None,
		traceHits,
		true
	);

	for (const FHitResult& traceHit : traceHits)
	{
		if (AWarriorStoneBase* foundStone = Cast<AWarriorStoneBase>(traceHit.GetActor()))
		{
			collectedStones.AddUnique(foundStone);
		}
	}

	if (collectedStones.IsEmpty())
	{
		CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
	}
}

void UHeroGameplayAbility_PickUpStones::consumeStones()
{
	if (collectedStones.IsEmpty())
	{
		CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
		return;
	}

	for (AWarriorStoneBase* collectedStone : collectedStones)
	{
		if (collectedStone)
		{
			collectedStone->Consume(getWarriorAbilitySystemComponentFromActorInfo(), GetAbilityLevel());
		}
	}

}
