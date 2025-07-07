// All Rights Reserved


#include "Items/Weapons/WarriorHeroWeapon.h"

void AWarriorHeroWeapon::assignGrantedAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& inSpecHandles)
{
	grantedAbilitySpecHandle = inSpecHandles;
}

TArray<FGameplayAbilitySpecHandle> AWarriorHeroWeapon::getGrantedAbilitySpecHandles() const
{
	return grantedAbilitySpecHandle;
}
