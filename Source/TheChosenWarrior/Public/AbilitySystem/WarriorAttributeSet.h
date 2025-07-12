// All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "WarriorAttributeSet.generated.h"

class IPawnUIInterface;

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

	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

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

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData attackPower;
	ATTRIBUTE_ACCESSORS(UWarriorAttributeSet, attackPower);

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData defensePower;
	ATTRIBUTE_ACCESSORS(UWarriorAttributeSet, defensePower);

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData damageTaken; 
	ATTRIBUTE_ACCESSORS(UWarriorAttributeSet, damageTaken);



private:
	TWeakInterfacePtr<IPawnUIInterface> cachedPawnUIInterface; //caching so we dont call cast everytime execute happens

	
};
