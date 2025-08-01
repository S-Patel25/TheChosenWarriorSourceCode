// All Rights Reserved


#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "AbilitySystem/WarriorGameplayAbility.h"
#include "ChosenWarriorGameplayTags.h"
void UWarriorAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& inInputTag)
{
	if (!inInputTag.IsValid())
	{
		return;
	}

	for (const FGameplayAbilitySpec& abilitySpec : GetActivatableAbilities())
	{
		if (!abilitySpec.DynamicAbilityTags.HasTagExact(inInputTag)) continue; //make sure its the exact tag
		
		if (inInputTag.MatchesTag(ChosenWarriorGameplayTags::InputTag_Toggleable)) //how to handle abilities that are toggleable
		{
			if (abilitySpec.IsActive())
			{
				CancelAbilityHandle(abilitySpec.Handle);
			}
			else
			{
				TryActivateAbility(abilitySpec.Handle);
			}
		}
		else
		{
			TryActivateAbility(abilitySpec.Handle);
		}
	}
}

void UWarriorAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& inInputTag)
{
	if (!inInputTag.IsValid() || !inInputTag.MatchesTag(ChosenWarriorGameplayTags::InputTag_MustBeHeld))
	{
		return;
	}

	for (const FGameplayAbilitySpec& abilitySpec : GetActivatableAbilities())
	{
		if (abilitySpec.DynamicAbilityTags.HasTagExact(inInputTag) && abilitySpec.IsActive())
		{
			CancelAbilityHandle(abilitySpec.Handle);
		}
	}

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

bool UWarriorAbilitySystemComponent::tryActivateAbilityByTag(FGameplayTag abilityTagToActivate)
{
	check(abilityTagToActivate.IsValid());

	TArray<FGameplayAbilitySpec*> foundAbilitySpecs;
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(abilityTagToActivate.GetSingleTagContainer(), foundAbilitySpecs);

	if (!foundAbilitySpecs.IsEmpty())
	{
		const int32 randomAbilityIndex = FMath::RandRange(0, foundAbilitySpecs.Num() - 1);
		FGameplayAbilitySpec* specToActivate = foundAbilitySpecs[randomAbilityIndex]; 

		check(specToActivate); //make sure its valid

		if (!specToActivate->IsActive())
		{
			return TryActivateAbility(specToActivate->Handle);
		}
	}

	return false;
}
