// All Rights Reserved


#include "WarriorFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "Interfaces/PawnCombatInterface.h"
#include "GenericTeamAgentInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "ChosenWarriorGameplayTags.h"
#include "WarriorTypes/WarriorCountDownAction.h"

#include "WarriorDebugHelpers.h"

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

float UWarriorFunctionLibrary::getScalableFloatValueAtLevel(const FScalableFloat& inScalableFloat, float inLevel)
{
	return inScalableFloat.GetValueAtLevel(inLevel);
}

FGameplayTag UWarriorFunctionLibrary::computeHitReactDirectionTag(AActor* inAttacker, AActor* inVictim, float& outAngleDifference)
{
	check(inAttacker && inVictim);

	const FVector victimForward = inVictim->GetActorForwardVector();
	const FVector victimToAttackerNormalized = (inAttacker->GetActorLocation() - inVictim->GetActorLocation()).GetSafeNormal(); //make sure to always have the get safe normal

	const float dotResult = FVector::DotProduct(victimForward, victimToAttackerNormalized);
	outAngleDifference = UKismetMathLibrary::DegAcos(dotResult);

	const FVector crossResult = FVector::CrossProduct(victimForward, victimToAttackerNormalized); //cross product needed as dot only gets +ve values (so cant tell left or right)

	if (crossResult.Z < 0.f) //pointing down
	{
		outAngleDifference = -1.f;
	}

	if (outAngleDifference >= -45.f && outAngleDifference <= 45.f) //front
	{
		return ChosenWarriorGameplayTags::Shared_Status_HitReact_Front;
	}
	else if (outAngleDifference < -45.f && outAngleDifference >= -135.f) //left
	{
		return ChosenWarriorGameplayTags::Shared_Status_HitReact_Left;
	}
	else if (outAngleDifference < -135.f || outAngleDifference > 135.f) //back
	{
		return ChosenWarriorGameplayTags::Shared_Status_HitReact_Back;
	}
	else if (outAngleDifference > 45.f && outAngleDifference <= 135.f) //right
	{
		return ChosenWarriorGameplayTags::Shared_Status_HitReact_Right;
	}

	return ChosenWarriorGameplayTags::Shared_Status_HitReact_Front;
}

bool UWarriorFunctionLibrary::isValidBlock(AActor* inAttacker, AActor* inDefender)
{
	check(inAttacker && inDefender);

	const float dotResult = FVector::DotProduct(inAttacker->GetActorForwardVector(), inDefender->GetActorForwardVector());

	return dotResult < -0.1f; //if less then 0 then it means they are facing each other so can block! (tweak numbers as needed)
}

bool UWarriorFunctionLibrary::applyGameplayEffectSpecHandleToTargetActor(AActor* inInstigator, AActor* inTargetActor, const FGameplayEffectSpecHandle& inSpecHandle)
{
	UWarriorAbilitySystemComponent* sourceASC = NativeGetWarriorASCFromActor(inInstigator);
	UWarriorAbilitySystemComponent* targetASC = NativeGetWarriorASCFromActor(inTargetActor);

	FActiveGameplayEffectHandle activeGameplayEffectHandle = sourceASC->ApplyGameplayEffectSpecToTarget(*inSpecHandle.Data, targetASC);

	return activeGameplayEffectHandle.WasSuccessfullyApplied(); //handy helper function
}

void UWarriorFunctionLibrary::countDown(const UObject* worldContextObject, float totalTime, float updateInterval, float& outRemainingTime, EWarriorCountDownActionInput countdownInput, UPARAM(DisplayName = "Output") EWarriorCountDownActionOutput& countdownOutput, FLatentActionInfo latentInfo)
{
	UWorld* world = nullptr;

	if (GEngine)
	{
		world = GEngine->GetWorldFromContextObject(worldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	}

	if (!world)
	{
		return;
	}

	FLatentActionManager& latentActionManager = world->GetLatentActionManager();

	FWarriorCountDownAction* foundAction = latentActionManager.FindExistingAction<FWarriorCountDownAction>(latentInfo.CallbackTarget, latentInfo.UUID);

	if (countdownInput == EWarriorCountDownActionInput::Start)
	{
		if (!foundAction) //create new action if not found
		{
			latentActionManager.AddNewAction(
				latentInfo.CallbackTarget,
				latentInfo.UUID,
				new FWarriorCountDownAction(totalTime, updateInterval, outRemainingTime, countdownOutput, latentInfo) //using new keyword won't cause mem leak as its already managed by latent manager
			);
		}
	}

	if (countdownInput == EWarriorCountDownActionInput::Cancel)
	{
		if (foundAction)
		{
			foundAction->cancelAction();
		}
	}

}
