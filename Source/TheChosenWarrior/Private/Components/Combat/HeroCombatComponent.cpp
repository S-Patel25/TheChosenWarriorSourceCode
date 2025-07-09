// All Rights Reserved


#include "Components/Combat/HeroCombatComponent.h"
#include "Items/Weapons/WarriorHeroWeapon.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "ChosenWarriorGameplayTags.h"

#include "WarriorDebugHelpers.h"

AWarriorHeroWeapon* UHeroCombatComponent::getHeroCarriedWeaponByTag(FGameplayTag inWeaponTag) const
{
    return Cast<AWarriorHeroWeapon>(getCharacterCarriedWeaponByTag(inWeaponTag));
}

void UHeroCombatComponent::OnHitTargetActor(AActor* hitActor)
{
    if (overlappedActors.Contains(hitActor))
    {
        return;
    }

    overlappedActors.AddUnique(hitActor);

    FGameplayEventData Data;
    Data.Instigator = getOwningPawn();
    Data.Target = hitActor;

    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor( //bp func 
        getOwningPawn(),
        ChosenWarriorGameplayTags::Shared_Event_MeleeHit,
        Data
    );
}

void UHeroCombatComponent::OnWeaponPulledFromTargetActor(AActor* interactedActor)
{

}
