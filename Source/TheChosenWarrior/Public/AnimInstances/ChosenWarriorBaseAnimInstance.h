// All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameplayTagContainer.h"
#include "ChosenWarriorBaseAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class THECHOSENWARRIOR_API UChosenWarriorBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
	bool doesOwnerHaveTag(FGameplayTag tagToCheck) const;
};
