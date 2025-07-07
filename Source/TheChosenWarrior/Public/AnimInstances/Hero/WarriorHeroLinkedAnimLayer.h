// All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/ChosenWarriorBaseAnimInstance.h"
#include "WarriorHeroLinkedAnimLayer.generated.h"

class UWarriorHeroAnimInstance;

/**
 * 
 */
UCLASS()
class THECHOSENWARRIOR_API UWarriorHeroLinkedAnimLayer : public UChosenWarriorBaseAnimInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe)) //meed to be bp pure and thread safe for it to show up on bp
	UWarriorHeroAnimInstance* getHeroAnimInstance() const;
	
};
