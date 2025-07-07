// All Rights Reserved


#include "AbilitySystem/WarriorHeroGameplayAbility.h"
#include "Characters/HeroCharacter.h"
#include "TheChosenWarriorHeroController.h"

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
