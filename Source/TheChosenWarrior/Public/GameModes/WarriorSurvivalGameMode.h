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

	UPROPERTY()
	EWarriorSurvivalGameModeState currentSurvivalGameModeState;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnSurvivalGameModeStateChangedDelegate onSurvivalGameModeStateChanged;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	UDataTable* enemyWaveSpawnerDataTable;
};
