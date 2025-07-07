// All Rights Reserved


#include "DataAssets/StartUpData/DataAsset_EnemyStartUpData.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "AbilitySystem/WarriorEnemyGameplayAbility.h"

void UDataAsset_EnemyStartUpData::GiveToAbilitySystemComponent(UWarriorAbilitySystemComponent* inASCToGive, int32 ApplyLevel)
{
	Super::GiveToAbilitySystemComponent(inASCToGive, ApplyLevel);

	if (!enemyCombatAbilities.IsEmpty())
	{
		for (const TSubclassOf < UWarriorEnemyGameplayAbility >& abilityClass : enemyCombatAbilities)
		{
			if (!abilityClass) continue;

			FGameplayAbilitySpec abilitySpec(abilityClass); //same process as hero

			abilitySpec.SourceObject = inASCToGive->GetAvatarActor();
			abilitySpec.Level = ApplyLevel;

			inASCToGive->GiveAbility(abilitySpec);
		}
	}
}
