// All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/WarriorGameplayAbility.h"
#include "WarriorEnemyGameplayAbility.generated.h"

class AWarriorEnemyCharacter;
class UEnemyCombatComponent;

/**
 * 
 */
UCLASS()
class THECHOSENWARRIOR_API UWarriorEnemyGameplayAbility : public UWarriorGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
	AWarriorEnemyCharacter* getEnemyCharacterFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
	UEnemyCombatComponent* getEnemyCombatComponentFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
	FGameplayEffectSpecHandle makeEnemyDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> effectClass, const FScalableFloat& inDamageScalableFloat);
	
private:
	TWeakObjectPtr<AWarriorEnemyCharacter> cachedWarriorEnemyCharacter; //caching
};
