// All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_RotateToFaceTarget.generated.h"

struct FRotateToFaceTargetTaskMemory
{
	TWeakObjectPtr<APawn> owningPawn;
	TWeakObjectPtr<AActor> targetActor;

	bool isValid() const
	{
		return owningPawn.IsValid() && targetActor.IsValid();
	}

	void Reset()
	{
		owningPawn.Reset();
		targetActor.Reset();
	}
};

/**
 * 
 */
UCLASS()
class THECHOSENWARRIOR_API UBTTask_RotateToFaceTarget : public UBTTaskNode
{
	GENERATED_BODY()
	
	UBTTask_RotateToFaceTarget();

	//Begin UBTNode Interface
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual uint16 GetInstanceMemorySize() const override; //need to get mem size 
	virtual FString GetStaticDescription() const override;
	//End UBTNode Interface

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	bool hasReachedAnglePrecision(APawn* queryPawn, AActor* targetActor) const;

	UPROPERTY(EditAnywhere, Category = "Face Target")
	float anglePrecision;

	UPROPERTY(EditAnywhere, Category = "Face Target")
	float rotationInterpSpeed;

	UPROPERTY(EditAnywhere, Category = "Face Target")
	FBlackboardKeySelector inTargetToFaceKey;
};
