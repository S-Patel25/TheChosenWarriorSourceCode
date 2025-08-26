// All Rights Reserved


#include "WarriorGameInstance.h"
#include "MoviePlayer.h"

void UWarriorGameInstance::Init()
{
    Super::Init();

    FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &ThisClass::onPreLoadMap);
    FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &ThisClass::onDestinationWorldLoaded);
}

void UWarriorGameInstance::onPreLoadMap(const FString& mapName)
{
    FLoadingScreenAttributes loadingScreenAttributes; //setting up config for loading screen, while preloading map
    loadingScreenAttributes.bAutoCompleteWhenLoadingCompletes = true;
    loadingScreenAttributes.MinimumLoadingScreenDisplayTime = 2.f;
    loadingScreenAttributes.WidgetLoadingScreen = FLoadingScreenAttributes::NewTestLoadingScreenWidget(); //will have a simple screen, TODO: MAKE CUSTOM LOADING SCREEN

    GetMoviePlayer()->SetupLoadingScreen(loadingScreenAttributes); //will not cause hitch when press play
}

void UWarriorGameInstance::onDestinationWorldLoaded(UWorld* loadedWorld)
{
    GetMoviePlayer()->StopMovie(); //stops loading screen
}

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
