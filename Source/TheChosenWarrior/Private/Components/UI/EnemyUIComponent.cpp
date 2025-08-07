// All Rights Reserved


#include "Components/UI/EnemyUIComponent.h"
#include "Widgets/WarriorWidgetBase.h"

void UEnemyUIComponent::registerEnemyDrawnWidget(UWarriorWidgetBase* inWidgetToRegister)
{
	enemyDrawnWidgets.Add(inWidgetToRegister);
}

void UEnemyUIComponent::removeEnemyDrawnWidgetsIfAny()
{
	if (enemyDrawnWidgets.IsEmpty())
	{
		return;
	}

	for (UWarriorWidgetBase* drawnWidget : enemyDrawnWidgets)
	{
		if (drawnWidget)
		{
			drawnWidget->RemoveFromParent();
		}
	}
}
