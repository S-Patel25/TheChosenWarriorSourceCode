// All Rights Reserved


#include "Controllers/WarriorAIController.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "WarriorDebugHelpers.h"

AWarriorAIController::AWarriorAIController(const FObjectInitializer& objectInitializer)
	: Super(objectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>("PathFollowingComponent")) //overriding
{
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

	if (otherTeamAgent && otherTeamAgent->GetGenericTeamId() < GetGenericTeamId()) //check to see if its our enemy and avoid the eqs test pawn
	{
		return ETeamAttitude::Hostile;
	}

	return ETeamAttitude::Friendly;
}

void AWarriorAIController::BeginPlay()
{
	Super::BeginPlay();

	if (UCrowdFollowingComponent* crowdComp = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent())) //improving quality of AI avoidance inside the if statement
	{
		crowdComp->SetCrowdSimulationState(bEnableDetourCrowdAvoidance ? ECrowdSimulationState::Enabled : ECrowdSimulationState::Disabled);

		switch (detourCrowdAvoidanceQuality)
		{
		case 1: crowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Low);    break;
		case 2: crowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Medium); break;
		case 3: crowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Good);   break;
		case 4: crowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::High);   break;
		default:
			break;
		}

		crowdComp->SetAvoidanceGroup(1);
		crowdComp->SetGroupsToAvoid(1);
		crowdComp->SetCrowdCollisionQueryRange(collisionQueryRange);
	}

}

void AWarriorAIController::OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (UBlackboardComponent* blackBoardComponent = GetBlackboardComponent())
	{
		if (!blackBoardComponent->GetValueAsObject(FName("TargetActor")))
		{
			if (Stimulus.WasSuccessfullySensed() && Actor)
			{
				blackBoardComponent->SetValueAsObject(FName("TargetActor"), Actor); //so enemies know who target actor is, fname must be same as the name set in bb
			}
		}
	}
}
