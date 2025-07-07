// All Rights Reserved


#include "Components/Combat/HeroCombatComponent.h"
#include "Items/Weapons/WarriorHeroWeapon.h"

AWarriorHeroWeapon* UHeroCombatComponent::getHeroCarriedWeaponByTag(FGameplayTag inWeaponTag) const
{
    return Cast<AWarriorHeroWeapon>(getCharacterCarriedWeaponByTag(inWeaponTag));
}
