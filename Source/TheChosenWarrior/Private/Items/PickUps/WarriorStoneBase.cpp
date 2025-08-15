// All Rights Reserved


#include "Items/PickUps/WarriorStoneBase.h"
#include "Characters/HeroCharacter.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "ChosenWarriorGameplayTags.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AbilitySystem/WarriorHeroGameplayAbility.h"

void AWarriorStoneBase::onPickUpCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AHeroCharacter* overLappedHeroCharacter = Cast<AHeroCharacter>(OtherActor))
	{
		overLappedHeroCharacter->getWarriorAbilitySystemComponent()->tryActivateAbilityByTag(ChosenWarriorGameplayTags::Player_Ability_PickUp_Stones);
	}
}


