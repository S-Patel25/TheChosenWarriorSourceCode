// All Rights Reserved

#pragma once

#include "GameplayTagContainer.h"
#include "ScalableFloat.h"
#include "WarriorStructTypes.generated.h"

class UWarriorHeroLinkedAnimLayer;
class UWarriorHeroGameplayAbility;
class UInputMappingContext;

USTRUCT(BlueprintType)
struct FWarriorHeroAbilitySet
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "InputTag"))
	FGameplayTag inputTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UWarriorHeroGameplayAbility> abilityToGrant;

	bool isValid() const;


};

USTRUCT(BlueprintType)
struct FWarriorHeroWeaponData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UWarriorHeroLinkedAnimLayer> weaponAnimLayerToLink;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputMappingContext* weaponInputMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag"))
	TArray<FWarriorHeroAbilitySet> defaultWeaponAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FScalableFloat weaponBaseDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> softWeaponIconTexture;
};