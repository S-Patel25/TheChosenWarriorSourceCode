// All Rights Reserved


#include "GameModes/WarriorSurvivalGameMode.h"

void AWarriorSurvivalGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AWarriorSurvivalGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWarriorSurvivalGameMode::setCurrentSurvivalGameModeState(EWarriorSurvivalGameModeState inState)
{
	currentSurvivalGameModeState = inState;

	onSurvivalGameModeStateChanged.Broadcast(currentSurvivalGameModeState);
}
