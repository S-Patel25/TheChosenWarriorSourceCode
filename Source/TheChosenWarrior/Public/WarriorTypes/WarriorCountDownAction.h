// All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "WarriorTypes/WarriorEnumTypes.h"

class FWarriorCountDownAction : public FPendingLatentAction
{
public:
	FWarriorCountDownAction(float inTotalCountDownTime, float inUpdateInterval, float& inOutRemaningTime, EWarriorCountDownActionOutput& inCountdownOutput, const FLatentActionInfo& latentInfo)
		: bNeedToCancel(false)
		, totalCountDownTime(inTotalCountDownTime)
		, updateInterval(inUpdateInterval)
		, outRemainingTime(inOutRemaningTime)
		, countdownOutput(inCountdownOutput)
		, executionFunction(latentInfo.ExecutionFunction)
		, outputLink(latentInfo.Linkage)
		, callbackTarget(latentInfo.CallbackTarget)
		, elapsedInterval(0.f)
		, elapsedTimeSinceStart(0.f)
	{
	}

	virtual void UpdateOperation(FLatentResponse& Response) override;

	void cancelAction();

private:
	bool bNeedToCancel;
	float totalCountDownTime;
	float updateInterval;
	float& outRemainingTime;
	EWarriorCountDownActionOutput& countdownOutput;
	FName executionFunction;
	int32 outputLink;
	FWeakObjectPtr callbackTarget;
	float elapsedInterval;
	float elapsedTimeSinceStart;
};