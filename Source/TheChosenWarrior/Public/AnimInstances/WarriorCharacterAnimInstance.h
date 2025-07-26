// All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/ChosenWarriorBaseAnimInstance.h"
#include "WarriorCharacterAnimInstance.generated.h"

class ATheChosenWarriorBaseCharacter;
class UCharacterMovementComponent;
/**
 * 
 */
UCLASS()
class THECHOSENWARRIOR_API UWarriorCharacterAnimInstance : public UChosenWarriorBaseAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override; //overriding base anim method
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;
	
protected:

	UPROPERTY() //always explose to reflection system, ensure garbage collection
	ATheChosenWarriorBaseCharacter* owningCharacter;

	UPROPERTY()
	UCharacterMovementComponent* owningMovementComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomationData")
	float groundSpeed;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomationData")
	bool bHasAcceleration;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomationData")
	float locomotionDirection;

	
};
