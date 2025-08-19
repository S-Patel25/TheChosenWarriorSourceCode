// All Rights Reserved


#include "GameModes/WarriorSurvivalGameMode.h"
#include "Engine/AssetManager.h"
#include "Characters/WarriorEnemyCharacter.h"

#include "WarriorDebugHelpers.h"

void AWarriorSurvivalGameMode::BeginPlay()
{
	Super::BeginPlay();

	checkf(enemyWaveSpawnerDataTable, TEXT("Forgot to assign valid Data Table!"));

	setCurrentSurvivalGameModeState(EWarriorSurvivalGameModeState::WaitSpawnNewWave);

	totalWavesToSpawn = enemyWaveSpawnerDataTable->GetRowNames().Num();

	preLoadNextWaveEnemies();

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
				preLoadNextWaveEnemies();
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
	return currentWaveCount > totalWavesToSpawn;
}

void AWarriorSurvivalGameMode::preLoadNextWaveEnemies()
{
	if (hasFinishedAllWaves())
	{
		return;
	}

	for (const FWarriorEnemyWaveSpawnerInfo& spawnerInfo : getCurrentWaveSpawnerTableRow()->enemyWaveSpawnerDefinitions)
	{
		if (spawnerInfo.softEnemyClassToSpawn.IsNull()) continue;

		UAssetManager::GetStreamableManager().RequestAsyncLoad(
			spawnerInfo.softEnemyClassToSpawn.ToSoftObjectPath(),
			FStreamableDelegate::CreateLambda(
				[spawnerInfo, this]()
				{
					if (UClass* loadedEnemyClass = spawnerInfo.softEnemyClassToSpawn.Get())
					{
						preLoadedEnemyClassMap.Emplace(spawnerInfo.softEnemyClassToSpawn, loadedEnemyClass);
						Debug::Print(loadedEnemyClass->GetName() + TEXT(" is loaded!"));
					}
				}
			)
		);
	}
}

FWarriorEnemyWaveSpawnerTableRow* AWarriorSurvivalGameMode::getCurrentWaveSpawnerTableRow() const
{
	const FName rowName = FName(TEXT("Wave") + FString::FromInt(currentWaveCount));

	FWarriorEnemyWaveSpawnerTableRow* foundRow = enemyWaveSpawnerDataTable->FindRow<FWarriorEnemyWaveSpawnerTableRow>(rowName, FString());

	checkf(foundRow, TEXT("Could not find valid row in the Data Table!"));

	return foundRow;
}
