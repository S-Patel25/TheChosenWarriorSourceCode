// All Rights Reserved


#include "GameModes/WarriorSurvivalGameMode.h"
#include "Engine/AssetManager.h"
#include "Characters/WarriorEnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/TargetPoint.h"
#include "NavigationSystem.h"

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
			currentSpawnedEnemiesCounter += trySpawnWaveEnemies();

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

	preLoadedEnemyClassMap.Empty();

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

int32 AWarriorSurvivalGameMode::trySpawnWaveEnemies()
{
	if (targetPointsArray.IsEmpty())
	{
		UGameplayStatics::GetAllActorsOfClass(this, ATargetPoint::StaticClass(), targetPointsArray);
	}

	checkf(!targetPointsArray.IsEmpty(), TEXT("No valid target points for the level!"));
	
	uint32 enemiesSpawnedThisTime = 0;

	FActorSpawnParameters spawnParam;
	spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	for (const FWarriorEnemyWaveSpawnerInfo& spawnerInfo : getCurrentWaveSpawnerTableRow()->enemyWaveSpawnerDefinitions)
	{
		if (spawnerInfo.softEnemyClassToSpawn.IsNull()) continue;

		const int32 numToSpawn = FMath::RandRange(spawnerInfo.minPerSpawnCount, spawnerInfo.maxPerSpawnCount);

		UClass* loadedEnemyClass = preLoadedEnemyClassMap.FindChecked(spawnerInfo.softEnemyClassToSpawn);

		for (int32 x = 0; x < numToSpawn; x++)
		{
			const int32 randomTargetPointIndex = FMath::RandRange(0, targetPointsArray.Num() - 1);
			const FVector spawnOrigin = targetPointsArray[randomTargetPointIndex]->GetActorLocation();
			const FRotator spawnRotation = targetPointsArray[randomTargetPointIndex]->GetActorForwardVector().ToOrientationRotator();

			FVector randomLocation;
			UNavigationSystemV1::K2_GetRandomLocationInNavigableRadius(this, spawnOrigin, randomLocation, 400.f);

			randomLocation += FVector(0.f, 0.f, 150.f); //add z offset so it doesnt spawn underground and anim can play properly

			AWarriorEnemyCharacter* spawnedEnemy = GetWorld()->SpawnActor<AWarriorEnemyCharacter>(loadedEnemyClass, randomLocation, spawnRotation, spawnParam);

			if (spawnedEnemy)
			{
				spawnedEnemy->OnDestroyed.AddUniqueDynamic(this, &ThisClass::OnEnemyDestroyed);

				enemiesSpawnedThisTime++;
				totalSpawnEnemiesThisWaveCounter++;
			}
			
			if (!shouldKeepSpawnEnemies())
			{
				return enemiesSpawnedThisTime;
			}
		}
	}

	return enemiesSpawnedThisTime;

}

bool AWarriorSurvivalGameMode::shouldKeepSpawnEnemies() const
{
	return totalSpawnEnemiesThisWaveCounter < getCurrentWaveSpawnerTableRow()->totalEnemyToSpawnThisWave;
}

void AWarriorSurvivalGameMode::OnEnemyDestroyed(AActor* destroyedActor)
{
	currentSpawnedEnemiesCounter--;

	Debug::Print(FString::Printf(TEXT("current: %i, total: %i"), currentSpawnedEnemiesCounter, totalSpawnEnemiesThisWaveCounter));

	if (shouldKeepSpawnEnemies())
	{
		currentSpawnedEnemiesCounter += trySpawnWaveEnemies();
	}
	else if (currentSpawnedEnemiesCounter == 0)
	{
		totalSpawnEnemiesThisWaveCounter = 0;
		currentSpawnedEnemiesCounter = 0;

		setCurrentSurvivalGameModeState(EWarriorSurvivalGameModeState::WaveCompleted);
	}
}

void AWarriorSurvivalGameMode::registerSpawnedEnemies(const TArray<AWarriorEnemyCharacter*>& inEnemiesToRegister)
{
	for (AWarriorEnemyCharacter* spawnedEnemy : inEnemiesToRegister)
	{
		if (spawnedEnemy) //fix issue with not registering spawned enemy from boss
		{
			currentSpawnedEnemiesCounter++;

			spawnedEnemy->OnDestroyed.AddUniqueDynamic(this, &ThisClass::OnEnemyDestroyed);
		}
	}
}
