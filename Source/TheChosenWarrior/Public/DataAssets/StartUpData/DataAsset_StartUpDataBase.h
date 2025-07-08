// All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_StartUpDataBase.generated.h"

class UWarriorGameplayAbility;
class UWarriorAbilitySystemComponent;
class UGameplayEffect;

/**
 * 
 */
UCLASS()
class THECHOSENWARRIOR_API UDataAsset_StartUpDataBase : public UDataAsset
{
	GENERATED_BODY()

public:
	virtual void GiveToAbilitySystemComponent(UWarriorAbilitySystemComponent* inASCToGive, int32 ApplyLevel = 1); //give ability

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Start Up Data")
	TArray< TSubclassOf < UWarriorGameplayAbility > > activateOnGivenAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "Start Up Data")
	TArray< TSubclassOf < UWarriorGameplayAbility > > reactiveAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "Start Up Data")
	TArray < TSubclassOf < UGameplayEffect > > startUpGameplayEffects;

	void grantAbilities(const TArray< TSubclassOf < UWarriorGameplayAbility > >& inAbilitiesToGive, UWarriorAbilitySystemComponent* inASCToGive, int32 ApplyLevel = 1);
	
};
