// All Rights Reserved


#include "AbilitySystem/WarriorAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "WarriorFunctionLibrary.h"
#include "ChosenWarriorGameplayTags.h"
#include "Interfaces/PawnUIInterface.h"
#include "Components/UI/PawnUIComponent.h"
#include "Components/UI/HeroUIComponent.h"

#include "WarriorDebugHelpers.h"

UWarriorAttributeSet::UWarriorAttributeSet()
{
	InitcurrentHealth(1.f); //macro already created init functions for us
	InitmaxHealth(1.f);
	InitcurrentRage(1.f);
	InitmaxRage(1.f);
	InitattackPower(1.f);
	InitdefensePower(1.f);
}

void UWarriorAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	if (!cachedPawnUIInterface.IsValid())
	{
		cachedPawnUIInterface = TWeakInterfacePtr<IPawnUIInterface>(Data.Target.GetAvatarActor());
	}

	checkf(cachedPawnUIInterface.IsValid(), TEXT("Didn't implment the UI pawn interface!"));

	UPawnUIComponent* pawnUIComponent = cachedPawnUIInterface->getPawnUIComponent();

	checkf(pawnUIComponent, TEXT("Couldn't get the PawnUIComponent!"));


	if (Data.EvaluatedData.Attribute == GetcurrentHealthAttribute())
	{
		const float newCurrentHealth = FMath::Clamp(GetcurrentHealth(), 0.f, GetmaxHealth());

		SetcurrentHealth(newCurrentHealth);

		pawnUIComponent->onCurrentHealthChanged.Broadcast(GetcurrentHealth() / GetmaxHealth());
	}

	if (Data.EvaluatedData.Attribute == GetcurrentRageAttribute())
	{
		const float newCurrentRage = FMath::Clamp(GetcurrentRage(), 0.f, GetmaxRage());

		SetcurrentHealth(newCurrentRage);

		if (UHeroUIComponent* heroUIComponent = cachedPawnUIInterface->getHeroUIComponent())
		{
			heroUIComponent->onCurrentRageChanged.Broadcast(GetcurrentRage() / GetmaxRage());
		}
	}

	if (Data.EvaluatedData.Attribute == GetdamageTakenAttribute())
	{
		const float oldHealth = GetcurrentHealth();
		const float damageDone = GetdamageTaken();

		const float newCurrentHealth = FMath::Clamp(oldHealth - damageDone, 0.f, GetmaxHealth());

		SetcurrentHealth(newCurrentHealth);

		/*const FString debugString = FString::Printf(
			TEXT("Old Health: %f, Damage Done: %f, newCurrentHealth: %f"),
			oldHealth,
			damageDone,
			newCurrentHealth
		);*/

		//Debug::Print(debugString, FColor::Green);
		
		pawnUIComponent->onCurrentHealthChanged.Broadcast(GetcurrentHealth() / GetmaxHealth());

		if (GetcurrentHealth() == 0.f)
		{
			UWarriorFunctionLibrary::addGameplayTagToActorIfNone(Data.Target.GetAvatarActor(), ChosenWarriorGameplayTags::Shared_Status_Dead);
		}
	}


}
