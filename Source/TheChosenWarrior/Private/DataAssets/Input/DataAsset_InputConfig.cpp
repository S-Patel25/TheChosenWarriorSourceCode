// All Rights Reserved


#include "DataAssets/Input/DataAsset_InputConfig.h"

UInputAction* UDataAsset_InputConfig::findNativeInputActionByTag(const FGameplayTag& inInputTag) const
{
	for (const FChosenWarriorInputActionConfig& inputActionConfig : nativeInputActions) //searches for an input action in a list using a tag as the key and returns the corresponding input action if it exists and is valid.
	{
		if (inputActionConfig.inputTag == inInputTag && inputActionConfig.inputAction)
		{
			return inputActionConfig.inputAction;
		}
	}

	return nullptr;
}
