// All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Characters/TheChosenWarriorBaseCharacter.h"
#include "WarriorEnemyCharacter.generated.h"

class UEnemyCombatComponent;
class UEnemyUIComponent;

/**
 * 
 */
UCLASS()
class THECHOSENWARRIOR_API AWarriorEnemyCharacter : public ATheChosenWarriorBaseCharacter
{
	GENERATED_BODY()

public:
	AWarriorEnemyCharacter();

	//Begin IPawnCombatInterface
	virtual UPawnCombatComponent* getPawnCombatComponent() const override;
	//End IPawnCombatInterface

protected:
	//Begin APawn Interface (follow UE best practices)
	virtual void PossessedBy(AController* NewController) override;
	//End APawn Interface

	//Begin IPawnUIInterface
	virtual UPawnUIComponent* getPawnUIComponent() const override;
	virtual UEnemyUIComponent* getEnemyUIComponent() const override;
	//End IPawnUIInterface

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	UEnemyCombatComponent* enemyCombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UEnemyUIComponent* enemyUIComponent;

private:
	void initEnemyStartUpData();

public:
	FORCEINLINE UEnemyCombatComponent* getEnemyCombatComponent() const { return enemyCombatComponent;  }
};
