// All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/UI/PawnUIComponent.h"
#include "HeroUIComponent.generated.h"

/**
 * 
 */
UCLASS()
class THECHOSENWARRIOR_API UHeroUIComponent : public UPawnUIComponent
{
	GENERATED_BODY()
	

public:
	UPROPERTY(BlueprintAssignable)
	FOnPercentChangedDelegate onCurrentRageChanged;
};
