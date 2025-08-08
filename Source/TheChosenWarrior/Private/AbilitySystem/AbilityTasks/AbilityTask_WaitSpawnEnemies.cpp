// All Rights Reserved


#include "AbilitySystem/AbilityTasks/AbilityTask_WaitSpawnEnemies.h"

UAbilityTask_WaitSpawnEnemies* UAbilityTask_WaitSpawnEnemies::waitSpawnEnemies(UGameplayAbility* owningAbility, FGameplayTag eventTag, TSoftClassPtr<AWarriorEnemyCharacter> softEnemyClassToSpawn, int32 numToSpawn, const FVector& spawnOrigin, float randomSpawnRadius, const FRotator& spawnRotation)
{
    UAbilityTask_WaitSpawnEnemies* Node = NewAbilityTask<UAbilityTask_WaitSpawnEnemies>(owningAbility);
    Node->cachedEventTag = eventTag;
    Node->cachedSoftEnemyClassToSpawn = softEnemyClassToSpawn;
    Node->cachedNumToSpawn = numToSpawn;
    Node->cachedSpawnOrigin = spawnOrigin;
    Node->cachedRandomSpawnRadius = randomSpawnRadius;
    Node->cachedSpawnRotation = spawnRotation;

    return Node;
}
