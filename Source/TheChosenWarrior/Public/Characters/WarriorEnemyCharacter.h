// All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Characters/TheChosenWarriorBaseCharacter.h"
#include "WarriorEnemyCharacter.generated.h"

class UEnemyCombatComponent;

/**
 * 
 */
UCLASS()
class THECHOSENWARRIOR_API AWarriorEnemyCharacter : public ATheChosenWarriorBaseCharacter
{
	GENERATED_BODY()

public:
	AWarriorEnemyCharacter();

protected:
	//Begin APawn Interface (follow UE best practices)
	virtual void PossessedBy(AController* NewController) override;
	//End APawn Interface

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	UEnemyCombatComponent* enemyCombatComponent;

private:
	void initEnemyStartUpData();

public:
	FORCEINLINE UEnemyCombatComponent* getEnemyCombatComponent() const { return enemyCombatComponent;  }
};
