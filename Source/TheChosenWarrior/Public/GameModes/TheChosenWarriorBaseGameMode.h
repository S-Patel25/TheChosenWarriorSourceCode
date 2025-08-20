// All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WarriorTypes/WarriorEnumTypes.h"
#include "TheChosenWarriorBaseGameMode.generated.h"

/**
 * 
 */
UCLASS()
class THECHOSENWARRIOR_API ATheChosenWarriorBaseGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATheChosenWarriorBaseGameMode();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Settings")
	EWarriorGameDifficulty currentGameDifficulty;

public:
	FORCEINLINE EWarriorGameDifficulty getCurrentGameDifficulty() const { return currentGameDifficulty; }
};
