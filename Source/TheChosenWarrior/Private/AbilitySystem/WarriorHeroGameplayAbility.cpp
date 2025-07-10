// All Rights Reserved


#include "AbilitySystem/WarriorHeroGameplayAbility.h"
#include "Characters/HeroCharacter.h"
#include "TheChosenWarriorHeroController.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "ChosenWarriorGameplayTags.h"

AHeroCharacter* UWarriorHeroGameplayAbility::getHeroCharacterFromActorInfo()
{
	if (!cachedWarriorHeroCharacter.IsValid())
	{
		cachedWarriorHeroCharacter = Cast<AHeroCharacter>(CurrentActorInfo->AvatarActor);
	}

	return cachedWarriorHeroCharacter.IsValid()? cachedWarriorHeroCharacter.Get() : nullptr; //get the cache ref
	
}

ATheChosenWarriorHeroController* UWarriorHeroGameplayAbility::getHeroControllerFromActorInfo()
{
	if (!cachedWarriorHeroController.IsValid())
	{
		cachedWarriorHeroController = Cast<ATheChosenWarriorHeroController>(CurrentActorInfo->PlayerController);
	}

	return cachedWarriorHeroController.IsValid() ? cachedWarriorHeroController.Get() : nullptr;
}

UHeroCombatComponent* UWarriorHeroGameplayAbility::getHeroCombatComponentFromActorInfo()
{
	return getHeroCharacterFromActorInfo()->getHeroCombatComponent();
}

FGameplayEffectSpecHandle UWarriorHeroGameplayAbility::makeHeroDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> effectClass, float inWeaponBaseDamage, FGameplayTag inCurrentAttackTypeTag, int32 inUsedComboCount)
{
	check(effectClass);

	FGameplayEffectContextHandle contextHandle = getWarriorAbilitySystemComponentFromActorInfo()->MakeEffectContext();
	contextHandle.SetAbility(this);
	contextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
	contextHandle.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());

	FGameplayEffectSpecHandle effectSpecHandle = getWarriorAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(  //returns spec handle
		effectClass,
		GetAbilityLevel(),
		contextHandle
	);

	effectSpecHandle.Data->SetSetByCallerMagnitude( //emplacing values into a t map
		ChosenWarriorGameplayTags::Shared_SetByCaller_BaseDamage,
		inWeaponBaseDamage
	);

	if (inCurrentAttackTypeTag.IsValid())
	{
		effectSpecHandle.Data->SetSetByCallerMagnitude(inCurrentAttackTypeTag, inUsedComboCount);
	}

	return effectSpecHandle;
}
