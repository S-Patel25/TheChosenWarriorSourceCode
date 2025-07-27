// All Rights Reserved


#include "WarriorFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "Interfaces/PawnCombatInterface.h"
#include "GenericTeamAgentInterface.h"

UWarriorAbilitySystemComponent* UWarriorFunctionLibrary::NativeGetWarriorASCFromActor(AActor* InActor)
{
	check(InActor);

	return CastChecked<UWarriorAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor)); //cast to get our warrior component

}

void UWarriorFunctionLibrary::addGameplayTagToActorIfNone(AActor* inActor, FGameplayTag tagToAdd)
{
	UWarriorAbilitySystemComponent* ASC = NativeGetWarriorASCFromActor(inActor);

	if (!ASC->HasMatchingGameplayTag(tagToAdd))
	{
		ASC->AddLooseGameplayTag(tagToAdd);
	}
}

void UWarriorFunctionLibrary::removeGameplayFromActorIfFound(AActor* inActor, FGameplayTag tagToRemove)
{
	UWarriorAbilitySystemComponent* ASC = NativeGetWarriorASCFromActor(inActor);

	if (ASC->HasMatchingGameplayTag(tagToRemove))
	{
		ASC->RemoveLooseGameplayTag(tagToRemove);
	}
}

bool UWarriorFunctionLibrary::NativeDoesActorHaveTag(AActor* inActor, FGameplayTag tagToCheck)
{
	UWarriorAbilitySystemComponent* ASC = NativeGetWarriorASCFromActor(inActor);

	return ASC->HasMatchingGameplayTag(tagToCheck);
}

void UWarriorFunctionLibrary::BP_DoesActorHaveTag(AActor* inActor, FGameplayTag tagToCheck, EWarriorConfirmType& outConfirmType)
{
	outConfirmType = NativeDoesActorHaveTag(inActor, tagToCheck)? EWarriorConfirmType::Yes : EWarriorConfirmType::No;

}

UPawnCombatComponent* UWarriorFunctionLibrary::nativeGetPawnCombatComponentFromActor(AActor* inActor)
{
	check(inActor);

	if (IPawnCombatInterface* pawnCombatInterface = Cast<IPawnCombatInterface>(inActor)) //if interface valid after cast, return combat component
	{
		return pawnCombatInterface->getPawnCombatComponent(); //will be enemy or hero
	}

	return nullptr;
}

UPawnCombatComponent* UWarriorFunctionLibrary::BP_GetPawnCombatComponentFromActor(AActor* inActor, EWarriorValidType& outValidType)
{
	UPawnCombatComponent* combatComponent = nativeGetPawnCombatComponentFromActor(inActor);

	outValidType = combatComponent? EWarriorValidType::Valid : EWarriorValidType::Invalid;

	return combatComponent;
}

bool UWarriorFunctionLibrary::isTargetPawnHostile(APawn* queryPawn, APawn* targetPawn)
{
	check(queryPawn && targetPawn);

	IGenericTeamAgentInterface* queryTeamAgent = Cast<IGenericTeamAgentInterface>(queryPawn->GetController());
	IGenericTeamAgentInterface* targetTeamAgent = Cast<IGenericTeamAgentInterface>(targetPawn->GetController());

	if (queryTeamAgent && targetTeamAgent)
	{
		return queryTeamAgent->GetGenericTeamId() != targetTeamAgent->GetGenericTeamId(); //if they have diff ID's, means they are hostile (not same enemy or etc.)
	}

	return false;
}
