// All Rights Reserved


#include "AbilitySystem/WarriorAttributeSet.h"
#include "GameplayEffectExtension.h"

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
	if (Data.EvaluatedData.Attribute == GetcurrentHealthAttribute())
	{
		const float newCurrentHealth = FMath::Clamp(GetcurrentHealth(), 0.f, GetmaxHealth());

		SetcurrentHealth(newCurrentHealth);
	}

	if (Data.EvaluatedData.Attribute == GetcurrentRageAttribute())
	{
		const float newCurrentRage = FMath::Clamp(GetcurrentRage(), 0.f, GetmaxRage());

		SetcurrentHealth(newCurrentRage);
	}

	if (Data.EvaluatedData.Attribute == GetdamageTakenAttribute())
	{
		const float oldHealth = GetcurrentHealth();
		const float damageDone = GetdamageTaken();

		const float newCurrentHealth = FMath::Clamp(oldHealth - damageDone, 0.f, GetmaxHealth());

		SetcurrentHealth(newCurrentHealth);

		const FString debugString = FString::Printf(
			TEXT("Old Health: %f, Damage Done: %f, newCurrentHealth: %f"),
			oldHealth,
			damageDone,
			newCurrentHealth
		);

		Debug::Print(debugString, FColor::Green);

		//TODO: Notify the UI

		//TODO: Handle character death
		if (newCurrentHealth == 0.f)
		{

		}
	}


}
