// All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/WarriorGameplayAbility.h"
#include "WarriorHeroGameplayAbility.generated.h"

class AHeroCharacter;
class ATheChosenWarriorHeroController;
class UHeroCombatComponent;

/**
 * 
 */
UCLASS()
class THECHOSENWARRIOR_API UWarriorHeroGameplayAbility : public UWarriorGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Warrior|Ability") //not const
	AHeroCharacter* getHeroCharacterFromActorInfo(); 

	UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
	ATheChosenWarriorHeroController* getHeroControllerFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
	UHeroCombatComponent* getHeroCombatComponentFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
	FGameplayEffectSpecHandle makeHeroDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> effectClass, float inWeaponBaseDamage, FGameplayTag inCurrentAttackTypeTag, int32 inUsedComboCount);

private:
	TWeakObjectPtr<AHeroCharacter> cachedWarriorHeroCharacter; //cache since we will refer to this many times, so no need to cast every time
	TWeakObjectPtr<ATheChosenWarriorHeroController> cachedWarriorHeroController;
	
};
