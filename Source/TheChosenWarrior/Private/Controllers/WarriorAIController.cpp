// All Rights Reserved


#include "Controllers/WarriorAIController.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

#include "WarriorDebugHelpers.h"

AWarriorAIController::AWarriorAIController(const FObjectInitializer& objectInitializer)
	: Super(objectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>("PathFollowingComponent")) //overriding
{
	if (UCrowdFollowingComponent* crowdComp = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent()))
	{
		Debug::Print(TEXT("Valid comp!"), FColor::Green);
	}

	AISenseConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Enemy Sense Config");
	AISenseConfig_Sight->DetectionByAffiliation.bDetectEnemies = true; //make sure it sees hostile pawns
	AISenseConfig_Sight->DetectionByAffiliation.bDetectFriendlies = false; //dont detect same type
	AISenseConfig_Sight->DetectionByAffiliation.bDetectNeutrals = false;
	AISenseConfig_Sight->SightRadius = 5000.f; //can change later
	AISenseConfig_Sight->LoseSightRadius = 0.f; //will never lose sight of player (can change later)
	AISenseConfig_Sight->PeripheralVisionAngleDegrees = 360.f; //even when behind enenmy can still see

	enemyPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("Enemy Perception Component");
	enemyPerceptionComponent->ConfigureSense(*AISenseConfig_Sight);
	enemyPerceptionComponent->SetDominantSense(UAISenseConfig_Sight::StaticClass());
	enemyPerceptionComponent->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &ThisClass::OnEnemyPerceptionUpdated);

	SetGenericTeamId(FGenericTeamId(1)); //1 will be the team id the enemies will use
}

ETeamAttitude::Type AWarriorAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const APawn* pawnToCheck = Cast<const APawn>(&Other);

	const IGenericTeamAgentInterface* otherTeamAgent = Cast<const IGenericTeamAgentInterface>(pawnToCheck->GetController());

	if (otherTeamAgent && otherTeamAgent->GetGenericTeamId() != GetGenericTeamId()) //check to see if its our enemy
	{
		return ETeamAttitude::Hostile;
	}

	return ETeamAttitude::Friendly;
}

void AWarriorAIController::OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed() && Actor)
	{
		Debug::Print(Actor->GetActorNameOrLabel() + TEXT(" was sensed"), FColor::Green);
	}
}
