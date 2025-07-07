// All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "DataAssets/Input/DataAsset_InputConfig.h"
#include "WarriorEnhancedInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class THECHOSENWARRIOR_API UWarriorEnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	template<class UserObject, typename CallbackFunc>
	void bindNativeInputAction(const UDataAsset_InputConfig* inInputConfig, const FGameplayTag& inInputTag, ETriggerEvent triggerEvent, UserObject* contextObject, CallbackFunc func); //template func for custom input (similar to enchnaced func)

	template<class UserObject, typename CallbackFunc>
	void bindAbilityInputAction(const UDataAsset_InputConfig* inInputConfig, UserObject* contextObject, CallbackFunc inputPressedFunc, CallbackFunc inputReleasedFunc);
};

template<class UserObject, typename CallbackFunc>
inline void UWarriorEnhancedInputComponent::bindNativeInputAction(const UDataAsset_InputConfig* inInputConfig, const FGameplayTag& inInputTag, ETriggerEvent triggerEvent, UserObject* contextObject, CallbackFunc func)
{
	checkf(inInputConfig, TEXT("Input config data asset is null, can't bind!")); //checkf is a null ptr checker, will crash editor if not init

	if (UInputAction* foundAction = inInputConfig->findNativeInputActionByTag(inInputTag))
	{
		BindAction(foundAction, triggerEvent, contextObject, func); //similar setup for imc and enchanced input
	}
}

template<class UserObject, typename CallbackFunc>
inline void UWarriorEnhancedInputComponent::bindAbilityInputAction(const UDataAsset_InputConfig* inInputConfig, UserObject* contextObject, CallbackFunc inputPressedFunc, CallbackFunc inputReleasedFunc)
{
	checkf(inInputConfig, TEXT("Input config data asset is null, can't bind!"));

	for (const FChosenWarriorInputActionConfig& abilityInputActionConfig : inInputConfig->abilityInputActions)
	{
		if (!abilityInputActionConfig.isValid()) continue;

		BindAction(abilityInputActionConfig.inputAction, ETriggerEvent::Started, contextObject, inputPressedFunc, abilityInputActionConfig.inputTag); //pressed
		BindAction(abilityInputActionConfig.inputAction, ETriggerEvent::Completed, contextObject, inputReleasedFunc, abilityInputActionConfig.inputTag); //released
	}
}
