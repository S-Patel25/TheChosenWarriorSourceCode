// All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_OrientToTargetActor.generated.h"

/**
 * 
 */
UCLASS()
class THECHOSENWARRIOR_API UBTService_OrientToTargetActor : public UBTService
{
	GENERATED_BODY() //creating service from scracth to better understand how it works!

	UBTService_OrientToTargetActor();

	//Begin UBTNode Interface
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual FString GetStaticDescription() const override;
	//End UBTNode Interface

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	UPROPERTY(EditAnywhere, Category = "Target")
	FBlackboardKeySelector inTargetActorKey;

	UPROPERTY(EditAnywhere, Category = "Target")
	float rotationInterpSpeed;
};
