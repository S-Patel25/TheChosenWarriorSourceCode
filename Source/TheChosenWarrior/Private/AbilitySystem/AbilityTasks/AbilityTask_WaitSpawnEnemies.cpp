// All Rights Reserved


#include "AbilitySystem/AbilityTasks/AbilityTask_WaitSpawnEnemies.h"
#include "AbilitySystemComponent.h"
#include "Engine/AssetManager.h"
#include "NavigationSystem.h"
#include "Characters/WarriorEnemyCharacter.h"

#include "WarriorDebugHelpers.h"

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

void UAbilityTask_WaitSpawnEnemies::Activate()
{
   FGameplayEventMulticastDelegate& Delegate = AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(cachedEventTag); //will return the delegate with the associate gameplay tag
   //MUST BE LOCAL REF

   delegateHandle = Delegate.AddUObject(this, &ThisClass::onGameplayEventReceived);
}

void UAbilityTask_WaitSpawnEnemies::OnDestroy(bool bInOwnerFinished)
{
    FGameplayEventMulticastDelegate& Delegate = AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(cachedEventTag); 

    Delegate.Remove(delegateHandle);

    Super::OnDestroy(bInOwnerFinished);
}

void UAbilityTask_WaitSpawnEnemies::onGameplayEventReceived(const FGameplayEventData* inPayload)
{
    if (ensure(!cachedSoftEnemyClassToSpawn.IsNull()))
    {
        UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
            cachedSoftEnemyClassToSpawn.ToSoftObjectPath(),
            FStreamableDelegate::CreateUObject(this, &ThisClass::onEnemyClassLoaded)
        );
    }
    else
    {
        if (ShouldBroadcastAbilityTaskDelegates())
        {
            didNotSpawn.Broadcast(TArray<AWarriorEnemyCharacter*>());
        }

        EndTask();
    }

}

void UAbilityTask_WaitSpawnEnemies::onEnemyClassLoaded()
{
    UClass* loadedClass = cachedSoftEnemyClassToSpawn.Get();

    UWorld* World = GetWorld();

    if (!loadedClass || !World)
    {
        if (ShouldBroadcastAbilityTaskDelegates())
        {
            didNotSpawn.Broadcast(TArray<AWarriorEnemyCharacter*>());
        }

        EndTask();

        return;
    }

    TArray<AWarriorEnemyCharacter*> spawnedEnemies;
    
    FActorSpawnParameters spawnParams;
    spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    for (int32 x = 0; x < cachedNumToSpawn; x++)
    {
        FVector randomLocation;
        UNavigationSystemV1::K2_GetRandomReachablePointInRadius(this, cachedSpawnOrigin, randomLocation, cachedRandomSpawnRadius); //handy helper func that spawns random locations based on origin and radisu

        randomLocation += FVector(0.f, 0.f, 150.f); //makes sure they aren't stuck in ground

        AWarriorEnemyCharacter* spawnedEnemy = World->SpawnActor<AWarriorEnemyCharacter>(loadedClass, randomLocation, cachedSpawnRotation, spawnParams);

        if (spawnedEnemy)
        {
            spawnedEnemies.Add(spawnedEnemy);
        }
    }

    if (ShouldBroadcastAbilityTaskDelegates())
    {
        if (!spawnedEnemies.IsEmpty())
        {
            onSpawnFinished.Broadcast(spawnedEnemies);
        }
        else
        {
            didNotSpawn.Broadcast(TArray<AWarriorEnemyCharacter*>());
        }
    }

    EndTask();
}
