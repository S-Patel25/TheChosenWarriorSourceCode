// All Rights Reserved


#include "GameModes/WarriorSurvivalGameMode.h"

void AWarriorSurvivalGameMode::BeginPlay()
{
	Super::BeginPlay();

	checkf(enemyWaveSpawnerDataTable, TEXT("Forgot to assign valid Data Table!"));

	setCurrentSurvivalGameModeState(EWarriorSurvivalGameModeState::WaitSpawnNewWave);

	totalWavesToSpawn = enemyWaveSpawnerDataTable->GetRowNames().Num();

}

void AWarriorSurvivalGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (currentSurvivalGameModeState == EWarriorSurvivalGameModeState::WaitSpawnNewWave)
	{
		timePassedSinceStart += DeltaTime;
		
		if (timePassedSinceStart >= spawnNewWaveWaitTime) //means starting new wave
		{
			timePassedSinceStart = 0.f;

			setCurrentSurvivalGameModeState(EWarriorSurvivalGameModeState::SpawningNewWave);
		}
	}

	if (currentSurvivalGameModeState == EWarriorSurvivalGameModeState::SpawningNewWave)
	{
		timePassedSinceStart += DeltaTime;

		if (timePassedSinceStart >= spawnEnemiesDelayTime)
		{
			timePassedSinceStart = 0.f;

			setCurrentSurvivalGameModeState(EWarriorSurvivalGameModeState::InProgress);
		}
	}

	if (currentSurvivalGameModeState == EWarriorSurvivalGameModeState::WaveCompleted)
	{
		timePassedSinceStart += DeltaTime;

		if (timePassedSinceStart >= waveCompletedWaitTime)
		{
			timePassedSinceStart = 0.f;
			currentWaveCount++;

			if (hasFinishedAllWaves())
			{
				setCurrentSurvivalGameModeState(EWarriorSurvivalGameModeState::AllWavesDone);
			}
			else
			{
				setCurrentSurvivalGameModeState(EWarriorSurvivalGameModeState::WaitSpawnNewWave);
			}
		}
	}
}

void AWarriorSurvivalGameMode::setCurrentSurvivalGameModeState(EWarriorSurvivalGameModeState inState)
{
	currentSurvivalGameModeState = inState;

	onSurvivalGameModeStateChanged.Broadcast(currentSurvivalGameModeState);
}

bool AWarriorSurvivalGameMode::hasFinishedAllWaves() const
{
	return currentWaveCount >= totalWavesToSpawn;
}
