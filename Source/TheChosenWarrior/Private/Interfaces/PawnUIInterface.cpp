// All Rights Reserved


#include "Interfaces/PawnUIInterface.h"

// Add default functionality here for any IPawnUIInterface functions that are not pure virtual.

UHeroUIComponent* IPawnUIInterface::getHeroUIComponent() const
{
	return nullptr;
}

UEnemyUIComponent* IPawnUIInterface::getEnemyUIComponent() const
{
	return nullptr;
}
