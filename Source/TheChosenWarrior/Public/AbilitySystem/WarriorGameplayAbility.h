// All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "WarriorTypes/WarriorEnumTypes.h"
#include "WarriorGameplayAbility.generated.h"

class UPawnCombatComponent;
class UWarriorAbilitySystemComponent;

UENUM(BlueprintType)
enum class EWarriorAbilityActivationPolicy : uint8
{
	OnTriggered,
	OnGiven
};

/**
 * 
 */
UCLASS()
class THECHOSENWARRIOR_API UWarriorGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

protected:
	//Begin UGamplayAbilityInterface
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActiviationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//End UGamplayAbilityInterface

	UPROPERTY(EditDefaultsOnly, Category = "WarriorAbility")
	EWarriorAbilityActivationPolicy abilityActivationPolicy = EWarriorAbilityActivationPolicy::OnTriggered; //give def value

	UFUNCTION(BlueprintPure, Category = "Warrior|Ability") //bp pure means no execution pins
	UPawnCombatComponent* getPawnCombatComponentFromActorInfo() const;

	UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
	UWarriorAbilitySystemComponent* getWarriorAbilitySystemComponentFromActorInfo() const;

	FActiveGameplayEffectHandle nativeApplyEffectSpecHandleToTarget(AActor* targetActor, const FGameplayEffectSpecHandle& inSpecHandle);

	UFUNCTION(BlueprintCallable, Category = "Warrior|Ability", meta = (DisplayName = "Apply Gameplay Effect Spec Handle To Target Actor", ExpandEnumAsExecs = "outSuccessType"))
	FActiveGameplayEffectHandle BP_ApplyEffectSpecHandleToTarget(AActor* targetActor, const FGameplayEffectSpecHandle& inSpecHandle, EWarriorSuccessType& outSuccessType);

	UFUNCTION(BlueprintCallable, Category = "Warrior|Ability")
	void applyGameplayEffectSpecHandleToHitResults(const FGameplayEffectSpecHandle& inSpecHandle, const TArray<FHitResult>& inHitResults); //apply to multiple targets ("AOE")

};
