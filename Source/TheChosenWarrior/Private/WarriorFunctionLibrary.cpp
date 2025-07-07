// All Rights Reserved


#include "WarriorFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"

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
