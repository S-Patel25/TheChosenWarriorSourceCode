// All Rights Reserved


#include "DataAssets/StartUpData/DataAsset_HeroStartUpData.h"
#include "AbilitySystem/WarriorGameplayAbility.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"


void UDataAsset_HeroStartUpData::GiveToAbilitySystemComponent(UWarriorAbilitySystemComponent* inASCToGive, int32 ApplyLevel)
{
    Super::GiveToAbilitySystemComponent(inASCToGive, ApplyLevel);

    for (const FWarriorHeroAbilitySet& abilitySet : heroStartUpAbilitySets)
    {
        if (!abilitySet.isValid()) continue;

        FGameplayAbilitySpec abilitySpec(abilitySet.abilityToGrant); //set attributes
        abilitySpec.SourceObject = inASCToGive->GetAvatarActor();
        abilitySpec.Level = ApplyLevel;
        abilitySpec.DynamicAbilityTags.AddTag(abilitySet.inputTag);

        inASCToGive->GiveAbility(abilitySpec); //give ability

    }


}
