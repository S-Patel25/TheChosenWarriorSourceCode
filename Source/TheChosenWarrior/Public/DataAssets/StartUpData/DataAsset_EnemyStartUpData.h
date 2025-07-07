// All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/StartUpData/DataAsset_StartUpDataBase.h"
#include "DataAsset_EnemyStartUpData.generated.h"

class UWarriorEnemyGameplayAbility;

/**
 * 
 */
UCLASS()
class THECHOSENWARRIOR_API UDataAsset_EnemyStartUpData : public UDataAsset_StartUpDataBase
{
	GENERATED_BODY()

public:
	virtual void GiveToAbilitySystemComponent(UWarriorAbilitySystemComponent* inASCToGive, int32 ApplyLevel = 1) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Start Up Data")
	TArray< TSubclassOf < UWarriorEnemyGameplayAbility > > enemyCombatAbilities; //same process as hero data

	
};
