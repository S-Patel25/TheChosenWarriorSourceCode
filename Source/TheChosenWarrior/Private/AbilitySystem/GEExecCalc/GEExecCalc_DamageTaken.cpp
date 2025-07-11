// All Rights Reserved


#include "AbilitySystem/GEExecCalc/GEExecCalc_DamageTaken.h"
#include "AbilitySystem/WarriorAttributeSet.h"
#include "ChosenWarriorGameplayTags.h"
#include "WarriorDebugHelpers.h"

struct FWarriorDamageCapture
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(attackPower); //fast way
	DECLARE_ATTRIBUTE_CAPTUREDEF(defensePower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(damageTaken);

	FWarriorDamageCapture()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UWarriorAttributeSet, attackPower, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UWarriorAttributeSet, defensePower, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UWarriorAttributeSet, damageTaken, Target, false);
	}
};

static const FWarriorDamageCapture& getWarriorDamageCapture()
{
	static FWarriorDamageCapture warriorDamageCapture;
	return warriorDamageCapture;
}

UGEExecCalc_DamageTaken::UGEExecCalc_DamageTaken()
{
	//Slow way

	// FProperty* attackPowerProperty = FindFieldChecked<FProperty>(
	//	UWarriorAttributeSet::StaticClass(),
	//	GET_MEMBER_NAME_CHECKED(UWarriorAttributeSet, attackPower)
	// );

	// FGameplayEffectAttributeCaptureDefinition attackPowerCaptureDefinition(
	//	attackPowerProperty,
	//	EGameplayEffectAttributeCaptureSource::Source,
	//	false
	// );

	// RelevantAttributesToCapture.Add(attackPowerCaptureDefinition); */
	
	//all this code is how to capture an attribute

	RelevantAttributesToCapture.Add(getWarriorDamageCapture().attackPowerDef);
	RelevantAttributesToCapture.Add(getWarriorDamageCapture().defensePowerDef);
	RelevantAttributesToCapture.Add(getWarriorDamageCapture().damageTakenDef);


}

void UGEExecCalc_DamageTaken::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& effectSpec = ExecutionParams.GetOwningSpec();

	FAggregatorEvaluateParameters evaluateParameters;
	evaluateParameters.SourceTags = effectSpec.CapturedSourceTags.GetAggregatedTags();
	evaluateParameters.TargetTags = effectSpec.CapturedTargetTags.GetAggregatedTags();

	float sourceAttackPower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(getWarriorDamageCapture().attackPowerDef, evaluateParameters, sourceAttackPower);
	//Debug::Print(TEXT("sourceAttackPower"), sourceAttackPower);


	float baseDamage = 0.f;
	int32 usedLightAttackComboCount = 0.f;
	int32 usedHeavyAttackComboCount = 0.f;
	
	for (const TPair<FGameplayTag, float>& tagMagnitude : effectSpec.SetByCallerTagMagnitudes)
	{
		if (tagMagnitude.Key.MatchesTagExact(ChosenWarriorGameplayTags::Shared_SetByCaller_BaseDamage))
		{
			baseDamage = tagMagnitude.Value; //get val from the tag
			//Debug::Print(TEXT("baseDamage"), baseDamage);
		}

		if (tagMagnitude.Key.MatchesTagExact(ChosenWarriorGameplayTags::Player_SetByCaller_AttackType_Light))
		{
			usedLightAttackComboCount = tagMagnitude.Value;
			//Debug::Print(TEXT("usedLightAttackComboCount"), usedLightAttackComboCount);
		}

		if (tagMagnitude.Key.MatchesTagExact(ChosenWarriorGameplayTags::Player_SetByCaller_AttackType_Heavy))
		{
			usedHeavyAttackComboCount = tagMagnitude.Value;
			//Debug::Print(TEXT("usedHeavyAttackComboCount"), usedHeavyAttackComboCount);
		}
	}

	float targetDefensePower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(getWarriorDamageCapture().defensePowerDef, evaluateParameters, targetDefensePower);
	//Debug::Print(TEXT("targetDefensePower"), targetDefensePower);

	if (usedLightAttackComboCount != 0) //increase damage based on where combo count is (reward for doing full combo)
	{
		const float damageIncreasePercentLight = (usedLightAttackComboCount - 1) * 0.07 + 1.f;

		baseDamage *= damageIncreasePercentLight;
		//Debug::Print(TEXT("damageIncreasePercentLight"), baseDamage);
	}

	if (usedHeavyAttackComboCount != 0)
	{
		const float damageIncreasePercentHeavy = (usedHeavyAttackComboCount - 1) * 0.15f + 1.f;

		baseDamage *= damageIncreasePercentHeavy;
		//Debug::Print(TEXT("damageIncreasePercentHeavy"), baseDamage);

	}

	const float finalDamageDone = baseDamage * sourceAttackPower / targetDefensePower; //easy way to calculate final damage done
	//Debug::Print(TEXT("finalDamageDone"), finalDamageDone);

	if (finalDamageDone > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(
				getWarriorDamageCapture().damageTakenProperty,
				EGameplayModOp::Override,
				finalDamageDone
			)
		);
	}
}
