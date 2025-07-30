// All Rights Reserved


#include "Components/Combat/EnemyCombatComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "ChosenWarriorGameplayTags.h"
#include "WarriorFunctionLibrary.h"

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

	const bool bIsPlayerBlocking = UWarriorFunctionLibrary::NativeDoesActorHaveTag(hitActor, ChosenWarriorGameplayTags::Player_Status_Blocking);
	const bool bIsMyAttackUnblockable = false;

	if (bIsPlayerBlocking && !bIsMyAttackUnblockable)
	{
		bIsValidBlock = UWarriorFunctionLibrary::isValidBlock(getOwningPawn(), hitActor);
	}

	FGameplayEventData eventData;
	eventData.Instigator = getOwningPawn();
	eventData.Target = hitActor;

	if (bIsValidBlock)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor( //register valid block
			hitActor,
			ChosenWarriorGameplayTags::Player_Event_SuccessfulBlock,
			eventData
		);
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
