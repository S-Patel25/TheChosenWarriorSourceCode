// All Rights Reserved


#include "AI/BTService_OrientToTargetActor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Kismet/KismetMathLibrary.h"


//Since this is done every frame, better to make this service in C++ rather then BP's for better performance!!
UBTService_OrientToTargetActor::UBTService_OrientToTargetActor()
{
	NodeName = TEXT("Native Orient Rotation To Target Actor");

	INIT_SERVICE_NODE_NOTIFY_FLAGS(); //need this macro for native class!

	rotationInterpSpeed = 5.f;
	Interval = 0.f;
	RandomDeviation = 0.f;

	inTargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, inTargetActorKey), AActor::StaticClass()); //for filters
}

void UBTService_OrientToTargetActor::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if (UBlackboardData* BBAsset = GetBlackboardAsset())
	{
		inTargetActorKey.ResolveSelectedKey(*BBAsset);
	}
}

FString UBTService_OrientToTargetActor::GetStaticDescription() const
{
	const FString keyDescription = inTargetActorKey.SelectedKeyName.ToString();

	return FString::Printf(TEXT("Orient rotation to %s Key %s"), *keyDescription, *GetStaticServiceDescription());
}

void UBTService_OrientToTargetActor::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UObject* actorObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(inTargetActorKey.SelectedKeyName);
	AActor* targetActor = Cast<AActor>(actorObject);
	
	APawn* owningPawn = OwnerComp.GetAIOwner()->GetPawn();

	if (owningPawn && targetActor)
	{
		const FRotator lookAtRot = UKismetMathLibrary::FindLookAtRotation(owningPawn->GetActorLocation(), targetActor->GetActorLocation()); //finding look at rot
		const FRotator targetRot = FMath::RInterpTo(owningPawn->GetActorRotation(), lookAtRot, DeltaSeconds, rotationInterpSpeed);

		owningPawn->SetActorRotation(targetRot); //will orient to player character
	}

}
