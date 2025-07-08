// All Rights Reserved


#include "Components/Combat/HeroCombatComponent.h"
#include "Items/Weapons/WarriorHeroWeapon.h"

#include "WarriorDebugHelpers.h"

AWarriorHeroWeapon* UHeroCombatComponent::getHeroCarriedWeaponByTag(FGameplayTag inWeaponTag) const
{
    return Cast<AWarriorHeroWeapon>(getCharacterCarriedWeaponByTag(inWeaponTag));
}

void UHeroCombatComponent::OnHitTargetActor(AActor* hitActor)
{
    Debug::Print(getOwningPawn()->GetActorNameOrLabel() + TEXT(" hit ") + hitActor->GetActorNameOrLabel(), FColor::Yellow);
}

void UHeroCombatComponent::OnWeaponPulledFromTargetActor(AActor* interactedActor)
{
    Debug::Print(getOwningPawn()->GetActorNameOrLabel() + TEXT("'s weapon pulled from ") + interactedActor->GetActorNameOrLabel(), FColor::Red);
}
