// All Rights Reserved


#include "Widgets/WarriorWidgetBase.h"
#include "Interfaces/PawnUIInterface.h"

void UWarriorWidgetBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (IPawnUIInterface* pawnUIInterface = Cast<IPawnUIInterface>(GetOwningPlayerPawn()))
	{
		if (UHeroUIComponent* heroUIComponent = pawnUIInterface->getHeroUIComponent())
		{
			BP_OnOwningHeroUIComponentInitialized(heroUIComponent);
		}
	}
}

