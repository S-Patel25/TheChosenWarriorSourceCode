// All Rights Reserved


#include "DataAssets/StartUpData/DataAsset_StartUpDataBase.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "AbilitySystem/WarriorGameplayAbility.h"

void UDataAsset_StartUpDataBase::GiveToAbilitySystemComponent(UWarriorAbilitySystemComponent* inASCToGive, int32 ApplyLevel)
{
	check(inASCToGive); //check to make sure if valid

	grantAbilities(activateOnGivenAbilities, inASCToGive, ApplyLevel);
	grantAbilities(reactiveAbilities, inASCToGive, ApplyLevel);

	if (!startUpGameplayEffects.IsEmpty())
	{
		for (const TSubclassOf < UGameplayEffect >& effectClass : startUpGameplayEffects)
		{
			if (!effectClass) continue;

			UGameplayEffect* effectCDO = effectClass->GetDefaultObject<UGameplayEffect>(); //give char attributes to player
			inASCToGive->ApplyGameplayEffectToSelf(
				effectCDO,
				ApplyLevel,
				inASCToGive->MakeEffectContext()
			);
		}
	}
}

void UDataAsset_StartUpDataBase::grantAbilities(const TArray<TSubclassOf<UWarriorGameplayAbility>>& inAbilitiesToGive, UWarriorAbilitySystemComponent* inASCToGive, int32 ApplyLevel)
{
	if (inAbilitiesToGive.IsEmpty()) //return early if empty
	{
		return;
	}
	
	for (const TSubclassOf<UWarriorGameplayAbility>& Ability : inAbilitiesToGive)
	{
		if (!Ability)
		{
			continue;
		}

		FGameplayAbilitySpec abilitySpec(Ability);
		abilitySpec.SourceObject = inASCToGive->GetAvatarActor(); //set attributes to ability, then give it to the ASC (ability system component) or "character"
		abilitySpec.Level = ApplyLevel;

		inASCToGive->GiveAbility(abilitySpec);
	}
}
