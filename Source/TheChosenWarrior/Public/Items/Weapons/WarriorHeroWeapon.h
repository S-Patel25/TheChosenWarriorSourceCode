// All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Items/Weapons/WarriorWeaponBase.h"
#include "WarriorTypes/WarriorStructTypes.h"
#include "GameplayAbilitySpecHandle.h"
#include "WarriorHeroWeapon.generated.h"

/**
 * 
 */
UCLASS()
class THECHOSENWARRIOR_API AWarriorHeroWeapon : public AWarriorWeaponBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData")
	FWarriorHeroWeaponData heroWeaponData;

	UFUNCTION(BlueprintCallable)
	void assignGrantedAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& inSpecHandles);

	UFUNCTION(BlueprintPure)
	TArray<FGameplayAbilitySpecHandle> getGrantedAbilitySpecHandles() const;

private:

	TArray<FGameplayAbilitySpecHandle> grantedAbilitySpecHandle;
	
};
