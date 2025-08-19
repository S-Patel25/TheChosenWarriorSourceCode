// All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameModes/TheChosenWarriorBaseGameMode.h"
#include "WarriorSurvivalGameMode.generated.h"

class AWarriorEnemyCharacter;

UENUM(BlueprintType)
enum class EWarriorSurvivalGameModeState : uint8
{
	WaitSpawnNewWave,
	SpawningNewWave,
	InProgress,
	WaveCompleted,
	AllWavesDone,
	PlayerDied
};

USTRUCT(BlueprintType)
struct FWarriorEnemyWaveSpawnerInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSoftClassPtr<AWarriorEnemyCharacter> softEnemyClassToSpawn;

	UPROPERTY(EditAnywhere)
	int32 minPerSpawnCount = 1;

	UPROPERTY(EditAnywhere)
	int32 maxPerSpawnCount = 3;

};

USTRUCT(BlueprintType)
struct FWarriorEnemyWaveSpawnerTableRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<FWarriorEnemyWaveSpawnerInfo> enemyWaveSpawnerDefinitions;

	UPROPERTY(EditAnywhere)
	int32 totalEnemyToSpawnThisWave = 1;

};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSurvivalGameModeStateChangedDelegate, EWarriorSurvivalGameModeState, currentState);


/**
 * 
 */
UCLASS()
class THECHOSENWARRIOR_API AWarriorSurvivalGameMode : public ATheChosenWarriorBaseGameMode
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	void setCurrentSurvivalGameModeState(EWarriorSurvivalGameModeState inState);
	bool hasFinishedAllWaves() const;
	void preLoadNextWaveEnemies();
	FWarriorEnemyWaveSpawnerTableRow* getCurrentWaveSpawnerTableRow() const;
	int32 trySpawnWaveEnemies();
	bool shouldKeepSpawnEnemies();

	UFUNCTION() //reminder: dynamic multicast functions need this!!
	void OnEnemyDestroyed(AActor* destroyedActor);

	UPROPERTY()
	EWarriorSurvivalGameModeState currentSurvivalGameModeState;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnSurvivalGameModeStateChangedDelegate onSurvivalGameModeStateChanged;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	UDataTable* enemyWaveSpawnerDataTable;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	int32 totalWavesToSpawn;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	int32 currentWaveCount = 1;

	UPROPERTY()
	int32 currentSpawnedEnemiesCounter = 0;

	UPROPERTY()
	int32 totalSpawnEnemiesThisWaveCounter = 0;

	UPROPERTY()
	TArray<AActor*> targetPointsArray;

	UPROPERTY()
	float timePassedSinceStart = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	float spawnNewWaveWaitTime = 5.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	float spawnEnemiesDelayTime = 2.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	float waveCompletedWaitTime = 5.f;

	UPROPERTY()
	TMap< TSoftClassPtr < AWarriorEnemyCharacter >, UClass* > preLoadedEnemyClassMap; //since async doesn't keep in memory, need to make tmap to access


};
