// All Rights Reserved


#include "Components/Combat/PawnCombatComponent.h"
#include "Items/Weapons/WarriorWeaponBase.h"
#include "Components/BoxComponent.h"

#include "WarriorDebugHelpers.h"

void UPawnCombatComponent::registerSpawnedWeapon(FGameplayTag inWeaponTagToRegister, AWarriorWeaponBase* inWeaponToRegister, bool bRegisterAsEquippedWeapon)
{
	checkf(!characterCarriedWeaponMap.Contains(inWeaponTagToRegister), TEXT("Weapon has already been registered!"));
	check(inWeaponToRegister); //do checks first!

	characterCarriedWeaponMap.Emplace(inWeaponTagToRegister, inWeaponToRegister);

	if (bRegisterAsEquippedWeapon)
	{
		currentEquippedWeaponTag = inWeaponTagToRegister;
	}

	const FString weaponString = FString::Printf(TEXT("A weapon named: %s has been registered using the tag %s"), *inWeaponToRegister->GetName(), *inWeaponTagToRegister.ToString());
	Debug::Print(weaponString);
}

AWarriorWeaponBase* UPawnCombatComponent::getCharacterCarriedWeaponByTag(FGameplayTag inWeaponTagToGet) const
{
	if (characterCarriedWeaponMap.Contains(inWeaponTagToGet))
	{
		if (AWarriorWeaponBase* const* foundWeapon = characterCarriedWeaponMap.Find(inWeaponTagToGet))
		{
			return *foundWeapon;
		}
	}

	return nullptr;
}

AWarriorWeaponBase* UPawnCombatComponent::getCharacterCurrentEquippedWeapon() const
{
	if (!currentEquippedWeaponTag.IsValid())
	{
		return nullptr;
	}

	return getCharacterCarriedWeaponByTag(currentEquippedWeaponTag);
}

void UPawnCombatComponent::toggleWeaponCollision(bool bShouldEnable, EToggleDamageType toggleDamageType)
{
	if (toggleDamageType == EToggleDamageType::CurrentEquippedWeapon)
	{
		AWarriorWeaponBase* weaponToToggle = getCharacterCurrentEquippedWeapon();

		check(weaponToToggle);

		if (bShouldEnable) //enable collision
		{
			weaponToToggle->getWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::QueryOnly); //only query
		}
		else //disable collision
		{
			weaponToToggle->getWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}


	}
}
