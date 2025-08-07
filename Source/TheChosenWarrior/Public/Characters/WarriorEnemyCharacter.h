// All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Characters/TheChosenWarriorBaseCharacter.h"
#include "WarriorEnemyCharacter.generated.h"

class UEnemyCombatComponent;
class UEnemyUIComponent;
class UWidgetComponent;
class UBoxComponent;

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

	virtual void BeginPlay() override;

	//Begin APawn Interface (follow UE best practices)
	virtual void PossessedBy(AController* NewController) override;
	//End APawn Interface

#if WITH_EDITOR //only works in editor, not runtime
	//Begin UObject Interface
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& propertyChangedEvent) override; //this function allows to attach collsion to left hand specfic (or anything else)
	//End UObject Interface
#endif

	//Begin IPawnUIInterface
	virtual UPawnUIComponent* getPawnUIComponent() const override;
	virtual UEnemyUIComponent* getEnemyUIComponent() const override;
	//End IPawnUIInterface

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	UEnemyCombatComponent* enemyCombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	UBoxComponent* leftHandCollisionBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	FName leftHandCollisionBoxAttachBoneName; //attach to specfic part rather then use getmesh()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	UBoxComponent* rightHandCollisionBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	FName rightHandCollisionBoxAttachBoneName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UEnemyUIComponent* enemyUIComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* enemyHealthWidgetComponent;

	UFUNCTION()
	virtual void onBodyCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	void initEnemyStartUpData();

public:
	FORCEINLINE UEnemyCombatComponent* getEnemyCombatComponent() const { return enemyCombatComponent;  }
	FORCEINLINE UBoxComponent* getLeftHandCollisionBox() const { return leftHandCollisionBox; }
	FORCEINLINE UBoxComponent* getRightHandCollisionBox() const { return rightHandCollisionBox; }
};
