// All Rights Reserved


#include "WarriorGameInstance.h"

TSoftObjectPtr<UWorld> UWarriorGameInstance::getGameLevelByTag(FGameplayTag inTag)
{
    for (FWarriorGameLevelSet& gameLevelSet : gameLevelSets)
    {
        if (!gameLevelSet.isValid()) continue;

        if (gameLevelSet.levelTag == inTag)
        {
            gameLevelSet.Level;
        }
    }

    return TSoftObjectPtr<UWorld>();
}
