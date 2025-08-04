// All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/WarriorHeroGameplayAbility.h"
#include "HeroGameplayAbility_TargetLock.generated.h"

class UWarriorWidgetBase;
class UInputMappingContext;
/**
 * 
 */
UCLASS()
class THECHOSENWARRIOR_API UHeroGameplayAbility_TargetLock : public UWarriorHeroGameplayAbility
{
	GENERATED_BODY()

protected:

	//begin UGameplayAbilityInterface
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//end UGameplayAbilityInterface

	UFUNCTION(BlueprintCallable)
	void onTargetLockTick(float DeltaTime);

	UFUNCTION(BlueprintCallable)
	void switchTarget(const FGameplayTag& inSwitchDirectionTag);

private:
	void tryLockOnTarget();
	void getAvailableActorsToLock();
	AActor* getNearestTargetFromAvailableActors(const TArray<AActor*>& inAvailableActors);
	void getAvailableActorsAroundTarget(TArray<AActor*>& outActorsOnLeft, TArray<AActor*>& outActorsOnRight);
	void drawTargetLockWidget();
	void setTargetLockWidgetPosition();
	void initTargetLockMovement();
	void initTargetLockMappingContext();

	void cancelTargetLockAbility();
	void cleanUp();
	void resetTargetLockMovement();
	void resetTargetLockMappingContext();

	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	float boxTraceDistance = 5000.f; //how far should line trace go!

	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	FVector traceBoxSize = FVector(5000.f, 5000.f, 300.f);

	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	TArray < TEnumAsByte < EObjectTypeQuery > > boxTraceChannel;

	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	bool bShowPersistentShape = false;

	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	TSubclassOf<UWarriorWidgetBase> targetLockWidgetClass; //hard ref is ok here since its a very small icon

	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	float targetLockRotationInterpSpeed = 5.f;

	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	float targetLockMaxWalkSpeed = 250.f;

	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	UInputMappingContext* targetLockMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	float targetLockCameraOffsetDistance = 20.f;

	UPROPERTY()
	TArray<AActor*> availableActorsToLock;

	UPROPERTY()
	AActor* currentLockedActor;

	UPROPERTY()
	UWarriorWidgetBase* drawnTargetLockWidget;

	UPROPERTY()
	FVector2D targetLockWidgetSize = FVector2D::ZeroVector;

	UPROPERTY()
	float cachedDefaultMaxWalkSpeed = 0.f;
};
