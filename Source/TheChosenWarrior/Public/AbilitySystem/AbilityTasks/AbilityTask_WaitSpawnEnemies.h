// All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_WaitSpawnEnemies.generated.h"

class AWarriorEnemyCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitSpawnEnemiesDelegate, const TArray<AWarriorEnemyCharacter*>&, spawnedEnemies);

/**
 * 
 */
UCLASS()
class THECHOSENWARRIOR_API UAbilityTask_WaitSpawnEnemies : public UAbilityTask
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Warrior|AbilityTasks", meta = (DisplayName = "Wait Gameplay Event and Spawn Enemies", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true", numToSpawn = "1", randomSpawnRadius = "200"))
	static UAbilityTask_WaitSpawnEnemies* waitSpawnEnemies(
		UGameplayAbility* owningAbility,
		FGameplayTag eventTag,
		TSoftClassPtr<AWarriorEnemyCharacter> softEnemyClassToSpawn,
		int32 numToSpawn,
		const FVector& spawnOrigin,
		float randomSpawnRadius,
		const FRotator& spawnRotation
	);

	UPROPERTY(BlueprintAssignable)
	FWaitSpawnEnemiesDelegate onSpawnFinished;

	UPROPERTY(BlueprintAssignable)
	FWaitSpawnEnemiesDelegate didNotSpawn;

private:
	FGameplayTag cachedEventTag;
	TSoftClassPtr<AWarriorEnemyCharacter> cachedSoftEnemyClassToSpawn;
	int32 cachedNumToSpawn;
	FVector cachedSpawnOrigin;
	float cachedRandomSpawnRadius;
	FRotator cachedSpawnRotation;
};
