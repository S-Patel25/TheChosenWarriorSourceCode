// All Rights Reserved


#include "AnimInstances/Hero/WarriorHeroAnimInstance.h"
#include "Characters/HeroCharacter.h"

void UWarriorHeroAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation(); //now we call super since overriding our own function

	if (owningCharacter)
	{
		owningHeroCharacter = Cast<AHeroCharacter>(owningCharacter); //get ref of our character
	}

}

void UWarriorHeroAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	if (bHasAcceleration)
	{
		idleElapsedTime = 0.f;
		bShouldEnterRelaxState = false;
	}
	else
	{
		idleElapsedTime += DeltaSeconds; //start counting the idle time then once passed threshold then go into relax state
		bShouldEnterRelaxState = (idleElapsedTime >= enterRelaxStateThreshold);
	}
}
