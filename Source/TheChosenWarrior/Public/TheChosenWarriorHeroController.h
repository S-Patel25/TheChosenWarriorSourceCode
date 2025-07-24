// All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GenericTeamAgentInterface.h"
#include "TheChosenWarriorHeroController.generated.h"

/**
 * 
 */
UCLASS()
class THECHOSENWARRIOR_API ATheChosenWarriorHeroController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	ATheChosenWarriorHeroController();

	//Begin IGenericTeamAgentInterface Interface
	virtual FGenericTeamId GetGenericTeamId() const override;
	//End IGenericTeamAgentInterface Interface

private:
	FGenericTeamId HeroTeamId;

};