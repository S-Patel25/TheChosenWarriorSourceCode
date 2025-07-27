// All Rights Reserved


#include "Components/Combat/EnemyCombatComponent.h"

#include "WarriorDebugHelpers.h"

void UEnemyCombatComponent::OnHitTargetActor(AActor* hitActor)
{
	if (hitActor)
	{
		Debug::Print(getOwningPawn()->GetActorNameOrLabel() + TEXT(" is hitting ") + hitActor->GetActorNameOrLabel());
	}
}
