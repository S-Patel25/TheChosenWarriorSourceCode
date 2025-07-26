// All Rights Reserved


#include "AnimInstances/ChosenWarriorBaseAnimInstance.h"
#include "WarriorFunctionLibrary.h"

bool UChosenWarriorBaseAnimInstance::doesOwnerHaveTag(FGameplayTag tagToCheck) const
{
	if (APawn* owningPawn = TryGetPawnOwner()) //so we can check to see if strafe is possible
	{
		return UWarriorFunctionLibrary::NativeDoesActorHaveTag(owningPawn, tagToCheck);
	}

	return false;
}
