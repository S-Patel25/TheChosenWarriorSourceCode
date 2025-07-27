// All Rights Reserved


#include "AbilitySystem/WarriorEnemyGameplayAbility.h"
#include "Characters/WarriorEnemyCharacter.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "ChosenWarriorGameplayTags.h"

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

FGameplayEffectSpecHandle UWarriorEnemyGameplayAbility::makeEnemyDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> effectClass, const FScalableFloat& inDamageScalableFloat)
{
    check(effectClass);

    //code setup very similar to player character

    FGameplayEffectContextHandle contextHandle = getWarriorAbilitySystemComponentFromActorInfo()->MakeEffectContext();
    contextHandle.SetAbility(this);
    contextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
    contextHandle.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());


    FGameplayEffectSpecHandle effectSpecHandle = getWarriorAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec( 
        effectClass,
        GetAbilityLevel(),
        contextHandle
    );
    
    effectSpecHandle.Data->SetSetByCallerMagnitude(
        ChosenWarriorGameplayTags::Shared_SetByCaller_BaseDamage,
        inDamageScalableFloat.GetValueAtLevel(GetAbilityLevel())
    );

    return effectSpecHandle;

}
