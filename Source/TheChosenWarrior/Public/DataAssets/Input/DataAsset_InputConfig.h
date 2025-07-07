// All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "DataAsset_InputConfig.generated.h"

class UInputAction;
class UInputMappingContext;

USTRUCT(BlueprintType)
struct FChosenWarriorInputActionConfig //using struct to map bindings to config
{
	GENERATED_BODY() //make sure to not forget this

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "InputTag"))
	FGameplayTag inputTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* inputAction;

	bool isValid() const
	{
		return inputTag.IsValid() && inputAction;
	}
};

/**
 * 
 */
UCLASS()
class THECHOSENWARRIOR_API UDataAsset_InputConfig : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputMappingContext* defaultMappingContext;
	 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "inputTag")) //better for readability
	TArray<FChosenWarriorInputActionConfig> nativeInputActions; //creating tarray of struct so we can reference it later

	UInputAction* findNativeInputActionByTag(const FGameplayTag& inInputTag) const; //helper function

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "inputTag")) //better for readability
	TArray<FChosenWarriorInputActionConfig> abilityInputActions; //creating tarray of struct so we can reference it later
	
};
