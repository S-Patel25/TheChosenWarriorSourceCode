// All Rights Reserved


#include "Components/Combat/EnemyCombatComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "ChosenWarriorGameplayTags.h"
#include "WarriorFunctionLibrary.h"
#include "Characters/WarriorEnemyCharacter.h"
#include "Components/BoxComponent.h"

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
	const bool bIsMyAttackUnblockable = UWarriorFunctionLibrary::NativeDoesActorHaveTag(getOwningPawn(), ChosenWarriorGameplayTags::Enemy_Status_Unblockable); //based on this we can create unblockable attacks

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

void UEnemyCombatComponent::toggleBodyCollisionBoxCollision(bool bShouldEnable, EToggleDamageType toggleDamageType)
{
	AWarriorEnemyCharacter* owningEnemyCharacter = getOwningPawn<AWarriorEnemyCharacter>();

	check(owningEnemyCharacter);

	UBoxComponent* leftHandCollisionBox = owningEnemyCharacter->getLeftHandCollisionBox();
	UBoxComponent* rightHandCollisionBox = owningEnemyCharacter->getRightHandCollisionBox();

	check(leftHandCollisionBox && rightHandCollisionBox);

	switch (toggleDamageType)
	{
	case EToggleDamageType::LeftHand:
		leftHandCollisionBox->SetCollisionEnabled(bShouldEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
		break;
	case EToggleDamageType::RightHand:
		rightHandCollisionBox->SetCollisionEnabled(bShouldEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
		break;
	default:
		break;
	}

	if (!bShouldEnable)
	{
		overlappedActors.Empty(); //clear array
	}
}
