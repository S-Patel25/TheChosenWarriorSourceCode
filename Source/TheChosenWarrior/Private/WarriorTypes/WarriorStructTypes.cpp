// All Rights Reserved


#include "WarriorTypes/WarriorStructTypes.h"
#include "AbilitySystem/WarriorHeroGameplayAbility.h"

bool FWarriorHeroAbilitySet::isValid() const
{
	return inputTag.IsValid() && abilityToGrant;
}
