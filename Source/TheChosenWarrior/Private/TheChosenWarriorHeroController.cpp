// All Rights Reserved


#include "TheChosenWarriorHeroController.h"

ATheChosenWarriorHeroController::ATheChosenWarriorHeroController()
{
	HeroTeamId = FGenericTeamId(0); //hero id
}

FGenericTeamId ATheChosenWarriorHeroController::GetGenericTeamId() const
{
	return HeroTeamId;
}
