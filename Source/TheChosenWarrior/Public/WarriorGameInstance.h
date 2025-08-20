// All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameplayTagContainer.h"
#include "WarriorGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FWarriorGameLevelSet
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, meta = (Categories = "GameData.Level"))
	FGameplayTag levelTag;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> Level;

	bool isValid() const
	{
		return levelTag.IsValid() && !Level.IsNull();
	}

};

/**
 * 
 */
UCLASS()
class THECHOSENWARRIOR_API UWarriorGameInstance : public UGameInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FWarriorGameLevelSet> gameLevelSets;

public:
	UFUNCTION(BlueprintCallable, meta = (GameplayTagFilter = "GameData.Level"))
	TSoftObjectPtr<UWorld> getGameLevelByTag(FGameplayTag inTag);
	
};
