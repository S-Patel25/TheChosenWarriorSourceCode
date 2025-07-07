// All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/WarriorCharacterAnimInstance.h"
#include "WarriorHeroAnimInstance.generated.h"

class AHeroCharacter;

/**
 * 
 */
UCLASS()
class THECHOSENWARRIOR_API UWarriorHeroAnimInstance : public UWarriorCharacterAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override; //overriding base anim method
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|References")
	AHeroCharacter* owningHeroCharacter;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomationData")
	bool bShouldEnterRelaxState; //relax anim

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomationData")
	float enterRelaxStateThreshold = 5.f; //how long idle before going into relax state

	float idleElapsedTime;
	
};
