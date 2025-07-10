// All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/Combat/PawnCombatComponent.h"
#include "HeroCombatComponent.generated.h"

class AWarriorHeroWeapon;

/**
 * 
 */
UCLASS()
class THECHOSENWARRIOR_API UHeroCombatComponent : public UPawnCombatComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Warrior|Combat")
	AWarriorHeroWeapon* getHeroCarriedWeaponByTag(FGameplayTag inWeaponTag) const;

	UFUNCTION(BlueprintCallable, Category = "Warrior|Combat")
	AWarriorHeroWeapon* getHeroCurrentEquippedWeapon() const;

	UFUNCTION(BlueprintCallable, Category = "Warrior|Combat")
	float getHeroCurrentEquippedWeaponDamageAtLevel(float inLevel) const;

	virtual void OnHitTargetActor(AActor* hitActor) override;
	virtual void OnWeaponPulledFromTargetActor(AActor* interactedActor) override;
};
