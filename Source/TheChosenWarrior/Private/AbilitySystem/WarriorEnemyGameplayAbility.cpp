// All Rights Reserved


#include "AbilitySystem/WarriorEnemyGameplayAbility.h"
#include "Characters/WarriorEnemyCharacter.h"

AWarriorEnemyCharacter* UWarriorEnemyGameplayAbility::getEnemyCharacterFromActorInfo()
{
    if (!cachedWarriorEnemyCharacter.IsValid())
    {
       cachedWarriorEnemyCharacter = Cast<AWarriorEnemyCharacter>(CurrentActorInfo->AvatarActor);
    }

    return cachedWarriorEnemyCharacter.IsValid() ? cachedWarriorEnemyCharacter.Get() : nullptr; //.get() means to deference the weak pointer
}

UEnemyCombatComponent* UWarriorEnemyGameplayAbility::getEnemyCombatComponentFromActorInfo()
{
    return getEnemyCharacterFromActorInfo()->getEnemyCombatComponent();
}
