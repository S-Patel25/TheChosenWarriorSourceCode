// All Rights Reserved


#include "WarriorGameInstance.h"

TSoftObjectPtr<UWorld> UWarriorGameInstance::getGameLevelByTag(FGameplayTag inTag) const
{
    for (const FWarriorGameLevelSet& gameLevelSet : gameLevelSets)
    {
        if (!gameLevelSet.isValid()) continue;

        if (gameLevelSet.levelTag == inTag)
        {
            UE_LOG(LogTemp, Warning, TEXT("Returning level: %s"),
                *gameLevelSet.Level.ToSoftObjectPath().ToString());
           return gameLevelSet.Level;
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("No level found for tag: %s"), *inTag.ToString());
    return TSoftObjectPtr<UWorld>();
}
