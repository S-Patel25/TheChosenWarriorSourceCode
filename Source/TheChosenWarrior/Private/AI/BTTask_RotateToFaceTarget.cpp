// All Rights Reserved


#include "AI/BTTask_RotateToFaceTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Kismet/KismetMathLibrary.h"

UBTTask_RotateToFaceTarget::UBTTask_RotateToFaceTarget()
{
	NodeName = TEXT("Native Rotate to Face Target Actor");
	anglePrecision = 10.f;
	rotationInterpSpeed = 5.f;


	bNotifyTick = true;
	bNotifyTaskFinished = true;
	bCreateNodeInstance = false; //in C++ BT is not instanced

	INIT_TASK_NODE_NOTIFY_FLAGS(); //NEEDS TO BE CALLED FOR NATIVE TASK TO WORK

	inTargetToFaceKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, inTargetToFaceKey), AActor::StaticClass());
}

void UBTTask_RotateToFaceTarget::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if (UBlackboardData* BBAsset = GetBlackboardAsset())
	{
		inTargetToFaceKey.ResolveSelectedKey(*BBAsset);
	}
}

uint16 UBTTask_RotateToFaceTarget::GetInstanceMemorySize() const
{
	return sizeof(FRotateToFaceTargetTaskMemory); //allocating block of mem for the struct
}

FString UBTTask_RotateToFaceTarget::GetStaticDescription() const
{
	const FString keyDescription = inTargetToFaceKey.SelectedKeyName.ToString();

	return FString::Printf(TEXT("Smoothly rotates to face %s Key until the angle precision %s is reached"), *keyDescription, *FString::SanitizeFloat(anglePrecision));
}

EBTNodeResult::Type UBTTask_RotateToFaceTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UObject* actorObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(inTargetToFaceKey.SelectedKeyName);
	AActor* targetActor = Cast<AActor>(actorObject);

	APawn* owningPawn = OwnerComp.GetAIOwner()->GetPawn();

	FRotateToFaceTargetTaskMemory* Memory = CastInstanceNodeMemory<FRotateToFaceTargetTaskMemory>(NodeMemory);
	check(Memory);

	Memory->owningPawn = owningPawn;
	Memory->targetActor = targetActor;

	if (!Memory->isValid())
	{
		return EBTNodeResult::Failed;
	}

	if (hasReachedAnglePrecision(owningPawn, targetActor))
	{
		Memory->Reset();
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_RotateToFaceTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FRotateToFaceTargetTaskMemory* Memory = CastInstanceNodeMemory<FRotateToFaceTargetTaskMemory>(NodeMemory);

	if (!Memory->isValid())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed); //this is how to end task
	}

	if (hasReachedAnglePrecision(Memory->owningPawn.Get(), Memory->targetActor.Get()))
	{
		Memory->Reset();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	else
	{
		const FRotator lookAtRot = UKismetMathLibrary::FindLookAtRotation(Memory->owningPawn->GetActorLocation(), Memory->targetActor->GetActorLocation());
		const FRotator targetRot = FMath::RInterpTo(Memory->owningPawn->GetActorRotation(), lookAtRot, DeltaSeconds, rotationInterpSpeed);

		Memory->owningPawn->SetActorRotation(targetRot);
	}
}

bool UBTTask_RotateToFaceTarget::hasReachedAnglePrecision(APawn* queryPawn, AActor* targetActor) const
{
	const FVector ownerForward = queryPawn->GetActorForwardVector();
	const FVector ownerToTargetNormalized = (targetActor->GetActorLocation() - queryPawn->GetActorLocation()).GetSafeNormal();

	const float dotResult = FVector::DotProduct(ownerForward, ownerToTargetNormalized); //get dot product for angle

	const float angleDiff = UKismetMathLibrary::DegAcos(dotResult); //get degrees not radians

	return angleDiff <= anglePrecision;
}
