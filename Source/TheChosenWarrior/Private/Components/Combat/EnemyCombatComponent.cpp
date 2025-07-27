// All Rights Reserved


#include "Components/Combat/EnemyCombatComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "ChosenWarriorGameplayTags.h"

#include "WarriorDebugHelpers.h"

void UEnemyCombatComponent::OnHitTargetActor(AActor* hitActor)
{
	if (overlappedActors.Contains(hitActor))
	{
		return;
	}

	overlappedActors.AddUnique(hitActor);

	//TODO: implement block check, parry, etc.
	bool bIsValidBlock = false;

	const bool bIsPlayerBlocking = false;
	const bool bIsMyAttackUnblockable = false;

	if (bIsPlayerBlocking && !bIsMyAttackUnblockable)
	{
		//TODO: check if block is valid
	}

	FGameplayEventData eventData;
	eventData.Instigator = getOwningPawn();
	eventData.Target = hitActor;

	if (bIsValidBlock)
	{
		//TODO: handle successful block
	}
	else
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			getOwningPawn(),
			ChosenWarriorGameplayTags::Shared_Event_MeleeHit,
			eventData
		);
	}
}
