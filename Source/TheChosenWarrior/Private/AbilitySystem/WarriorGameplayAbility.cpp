// All Rights Reserved


#include "AbilitySystem/WarriorGameplayAbility.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "Components/Combat/PawnCombatComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

void UWarriorGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	if (abilityActivationPolicy == EWarriorAbilityActivationPolicy::OnGiven)
	{
		if (ActorInfo && !Spec.IsActive()) //actorinfo is a useful struct with many info inside it 
		{
			ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle); //activate ability
		}
	}
}

void UWarriorGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActiviationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActiviationInfo, bReplicateEndAbility, bWasCancelled);

	if (abilityActivationPolicy == EWarriorAbilityActivationPolicy::OnGiven)
	{
		if (ActorInfo)
		{
			ActorInfo->AbilitySystemComponent->ClearAbility(Handle); //remove haandle
		}
	}
}

UPawnCombatComponent* UWarriorGameplayAbility::getPawnCombatComponentFromActorInfo() const
{
	return GetAvatarActorFromActorInfo()->FindComponentByClass<UPawnCombatComponent>(); //find the pawn component (finds first, so if multiple may need a diff solution)
}

UWarriorAbilitySystemComponent* UWarriorGameplayAbility::getWarriorAbilitySystemComponentFromActorInfo() const
{
	return Cast<UWarriorAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent);
}

FActiveGameplayEffectHandle UWarriorGameplayAbility::nativeApplyEffectSpecHandleToTarget(AActor* targetActor, const FGameplayEffectSpecHandle& inSpecHandle)
{
	UAbilitySystemComponent* targetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(targetActor);

	check(targetASC && inSpecHandle.IsValid())

	return getWarriorAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(
		*inSpecHandle.Data,
		targetASC
	);

}

FActiveGameplayEffectHandle UWarriorGameplayAbility::BP_ApplyEffectSpecHandleToTarget(AActor* targetActor, const FGameplayEffectSpecHandle& inSpecHandle, EWarriorSuccessType& outSuccessType)
{
	FActiveGameplayEffectHandle activeGameplayEffectHandle = nativeApplyEffectSpecHandleToTarget(targetActor, inSpecHandle);

	outSuccessType = activeGameplayEffectHandle.WasSuccessfullyApplied() ? EWarriorSuccessType::Successful : EWarriorSuccessType::Failed;

	return activeGameplayEffectHandle;
}
