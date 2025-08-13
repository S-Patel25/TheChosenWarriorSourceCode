// All Rights Reserved


#include "WarriorTypes/WarriorCountDownAction.h"

void FWarriorCountDownAction::UpdateOperation(FLatentResponse& Response)
{
	if (bNeedToCancel)
	{
		countdownOutput = EWarriorCountDownActionOutput::Cancelled;

		Response.FinishAndTriggerIf(true, executionFunction, outputLink, callbackTarget);
		
		return;
	}

	if (elapsedTimeSinceStart >= totalCountDownTime)
	{
		countdownOutput = EWarriorCountDownActionOutput::Completed;

		Response.FinishAndTriggerIf(true, executionFunction, outputLink, callbackTarget);

		return;
	}

	if (elapsedInterval < updateInterval)
	{
		elapsedInterval += Response.ElapsedTime(); //like deltatime 
	}
	else
	{
		elapsedTimeSinceStart += updateInterval > 0.f ? updateInterval : Response.ElapsedTime();

		outRemainingTime = totalCountDownTime - elapsedTimeSinceStart;

		countdownOutput = EWarriorCountDownActionOutput::Updated;

		Response.TriggerLink(executionFunction, outputLink, callbackTarget);

		elapsedInterval = 0.f;
	}
}

void FWarriorCountDownAction::cancelAction()
{
	bNeedToCancel = true;
}
