// All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "WarriorAttributeSet.generated.h"

//helper functions for attributes

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class THECHOSENWARRIOR_API UWarriorAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UWarriorAttributeSet();

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FGameplayAttributeData currentHealth;
	ATTRIBUTE_ACCESSORS(UWarriorAttributeSet, currentHealth);

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FGameplayAttributeData maxHealth;
	ATTRIBUTE_ACCESSORS(UWarriorAttributeSet, maxHealth);

	UPROPERTY(BlueprintReadOnly, Category = "Rage")
	FGameplayAttributeData currentRage;
	ATTRIBUTE_ACCESSORS(UWarriorAttributeSet, currentRage);

	UPROPERTY(BlueprintReadOnly, Category = "Rage")
	FGameplayAttributeData maxRage;
	ATTRIBUTE_ACCESSORS(UWarriorAttributeSet, maxRage);

	UPROPERTY(BlueprintReadOnly, Category = "Rage")
	FGameplayAttributeData attackPower;
	ATTRIBUTE_ACCESSORS(UWarriorAttributeSet, attackPower);

	UPROPERTY(BlueprintReadOnly, Category = "Rage")
	FGameplayAttributeData defensePower;
	ATTRIBUTE_ACCESSORS(UWarriorAttributeSet, defensePower);




	
};
