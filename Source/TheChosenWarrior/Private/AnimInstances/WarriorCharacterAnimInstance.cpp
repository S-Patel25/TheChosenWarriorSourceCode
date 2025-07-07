// All Rights Reserved


#include "AnimInstances/WarriorCharacterAnimInstance.h"
#include "Characters/TheChosenWarriorBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UWarriorCharacterAnimInstance::NativeInitializeAnimation()
{
	owningCharacter = Cast<ATheChosenWarriorBaseCharacter>(TryGetPawnOwner());

	if (owningCharacter) //null check since it will be in editor, so can't use check
	{
		owningMovementComponent = owningCharacter->GetCharacterMovement();
	}
}

void UWarriorCharacterAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds) //good for optimization
{
	if (!owningCharacter || !owningMovementComponent)
	{
		return; //leave function early if null
	}

	groundSpeed = owningCharacter->GetVelocity().Size2D();

	bHasAcceleration = owningMovementComponent->GetCurrentAcceleration().SizeSquared2D() > 0.f; //if true means acceleration!

}
