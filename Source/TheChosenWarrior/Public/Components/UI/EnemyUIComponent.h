// All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/UI/PawnUIComponent.h"
#include "EnemyUIComponent.generated.h"

class UWarriorWidgetBase;

/**
 * 
 */
UCLASS()
class THECHOSENWARRIOR_API UEnemyUIComponent : public UPawnUIComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void registerEnemyDrawnWidget(UWarriorWidgetBase* inWidgetToRegister);

	UFUNCTION(BlueprintCallable)
	void removeEnemyDrawnWidgetsIfAny();

private:
	TArray<UWarriorWidgetBase*> enemyDrawnWidgets;
	
};
