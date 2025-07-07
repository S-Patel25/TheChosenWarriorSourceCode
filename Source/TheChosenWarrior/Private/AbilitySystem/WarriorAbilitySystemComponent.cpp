// All Rights Reserved


#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "AbilitySystem/WarriorGameplayAbility.h"

void UWarriorAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& inInputTag)
{
	if (!inInputTag.IsValid())
	{
		return;
	}

	for (const FGameplayAbilitySpec& abilitySpec : GetActivatableAbilities())
	{
		if (!abilitySpec.DynamicAbilityTags.HasTagExact(inInputTag)) continue; //make sure its the exact tag
		
		TryActivateAbility(abilitySpec.Handle);
	}
}

void UWarriorAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& inInputTag)
{

}

void UWarriorAbilitySystemComponent::grantHeroWeaponAbilities(const TArray<FWarriorHeroAbilitySet>& inDefaultWeaponAbilities, int32 ApplyLevel, TArray<FGameplayAbilitySpecHandle>& outGrantedAbilitySpecHandles)
{
	if (inDefaultWeaponAbilities.IsEmpty())
	{
		return;
	}

	for (const FWarriorHeroAbilitySet& abilitySet : inDefaultWeaponAbilities)
	{
		if (!abilitySet.isValid()) continue;

		FGameplayAbilitySpec abilitySpec(abilitySet.abilityToGrant); //same setup for granting abilities
		abilitySpec.SourceObject = GetAvatarActor();
		abilitySpec.Level = ApplyLevel;
		abilitySpec.DynamicAbilityTags.AddTag(abilitySet.inputTag);

		outGrantedAbilitySpecHandles.AddUnique(GiveAbility(abilitySpec)); //grant ability then store it! (can remove abilities, etc.)
	}

}

void UWarriorAbilitySystemComponent::removedGrantedHeroWeaponAbilities(UPARAM(ref)TArray<FGameplayAbilitySpecHandle>& inSpecHandlesToRemove)
{
	if (inSpecHandlesToRemove.IsEmpty())
	{
		return;
	}

	for (const FGameplayAbilitySpecHandle& specHandle : inSpecHandlesToRemove)
	{
		if (specHandle.IsValid())
		{
			ClearAbility(specHandle);
		}

		inSpecHandlesToRemove.Empty(); //clear 
	}

}
