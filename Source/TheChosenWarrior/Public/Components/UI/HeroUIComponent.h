// All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/UI/PawnUIComponent.h"
#include "HeroUIComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEquippedWeaponChangedDelegate, TSoftObjectPtr<UTexture2D>, softWeaponIcon);

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

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnEquippedWeaponChangedDelegate onEquippedWeaponChanged;
};
