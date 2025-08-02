// All Rights Reserved


#include "AbilitySystem/HeroGameplayAbility_TargetLock.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Characters/HeroCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/WarriorWidgetBase.h"
#include "TheChosenWarriorHeroController.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Components/SizeBox.h"
#include "WarriorFunctionLibrary.h"
#include "ChosenWarriorGameplayTags.h"
#include "Kismet/KismetMathLibrary.h"


#include "WarriorDebugHelpers.h"
//doing most of the target lock functionality in C++ as its more performant considering this will be handled frame by frame (switching targets, animation poses, etc.)
void UHeroGameplayAbility_TargetLock::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	tryLockOnTarget();

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UHeroGameplayAbility_TargetLock::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	cleanUp();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UHeroGameplayAbility_TargetLock::onTargetLockTick(float DeltaTime)
{
	if (!currentLockedActor || 
		UWarriorFunctionLibrary::NativeDoesActorHaveTag(currentLockedActor, ChosenWarriorGameplayTags::Shared_Status_Dead) ||
		UWarriorFunctionLibrary::NativeDoesActorHaveTag(getHeroCharacterFromActorInfo(), ChosenWarriorGameplayTags::Shared_Status_Dead)
		)
	{
		cancelTargetLockAbility();
		return;
	}

	setTargetLockWidgetPosition();

	const bool bShouldOverrideRotation = //not override if blocking or rolling
	!UWarriorFunctionLibrary::NativeDoesActorHaveTag(getHeroCharacterFromActorInfo(), ChosenWarriorGameplayTags::Player_Status_Rolling)
	&&
	!UWarriorFunctionLibrary::NativeDoesActorHaveTag(getHeroCharacterFromActorInfo(), ChosenWarriorGameplayTags::Player_Status_Blocking);

	if (bShouldOverrideRotation)
	{
		const FRotator lookAtRot = UKismetMathLibrary::FindLookAtRotation(
			getHeroCharacterFromActorInfo()->GetActorLocation(),
			currentLockedActor->GetActorLocation()
		);

		const FRotator currentControlRot = getHeroControllerFromActorInfo()->GetControlRotation();
		const FRotator targetRot = FMath::RInterpTo(currentControlRot, lookAtRot, DeltaTime, targetLockRotationInterpSpeed); //make rotation oriented to the target lock

		getHeroControllerFromActorInfo()->SetControlRotation(FRotator(targetRot.Pitch, targetRot.Yaw, 0.f)); //camera rot
		getHeroCharacterFromActorInfo()->SetActorRotation(FRotator(0.f, targetRot.Yaw, 0.f));
	}
}

void UHeroGameplayAbility_TargetLock::tryLockOnTarget()
{
	getAvailableActorsToLock();

	if (availableActorsToLock.IsEmpty())
	{
		cancelTargetLockAbility(); //if no targets found then we cancel
		return;
	}

	currentLockedActor = getNearestTargetFromAvailableActors(availableActorsToLock); //has a valid lock on target at this point

	if (currentLockedActor)
	{
		drawTargetLockWidget();
		setTargetLockWidgetPosition();
	}
	else
	{
		cancelTargetLockAbility();
	}
}

void UHeroGameplayAbility_TargetLock::getAvailableActorsToLock()
{
	TArray<FHitResult> boxTraceHits; //will have the enemies we can target lock
	 
	UKismetSystemLibrary::BoxTraceMultiForObjects( //box trace mulitple to get enemies
		getHeroCharacterFromActorInfo(),
		getHeroCharacterFromActorInfo()->GetActorLocation(),
		getHeroCharacterFromActorInfo()->GetActorLocation() + getHeroCharacterFromActorInfo()->GetActorForwardVector() * boxTraceDistance,
		traceBoxSize / 2.f, //divide as its half size
		getHeroCharacterFromActorInfo()->GetActorForwardVector().ToOrientationRotator(), //where box is facing
		boxTraceChannel,
		false,
		TArray<AActor*>(),
		bShowPersistentShape ? EDrawDebugTrace::Persistent : EDrawDebugTrace::None, //should we see shape?
		boxTraceHits,
		true
	);

	for (const FHitResult& traceHit : boxTraceHits) //loop through results
	{
		if (AActor* hitActor = traceHit.GetActor())
		{
			if (hitActor != getHeroCharacterFromActorInfo()) //means its can be locked on
			{
				availableActorsToLock.AddUnique(hitActor);

				Debug::Print(hitActor->GetActorNameOrLabel());
			}
		}
	}
}

AActor* UHeroGameplayAbility_TargetLock::getNearestTargetFromAvailableActors(const TArray<AActor*>& inAvailableActors)
{
	float closestDistance = 0.f;
	return UGameplayStatics::FindNearestActor(getHeroCharacterFromActorInfo()->GetActorLocation(), inAvailableActors, closestDistance); //get nearest actor to target lock onto
}

void UHeroGameplayAbility_TargetLock::drawTargetLockWidget()
{
	if (!drawnTargetLockWidget) //only draw if not valid
	{
		checkf(targetLockWidgetClass, TEXT("Forgot to assign valid widget class!"))

		drawnTargetLockWidget = CreateWidget<UWarriorWidgetBase>(getHeroControllerFromActorInfo(), targetLockWidgetClass);
		check(drawnTargetLockWidget);

		drawnTargetLockWidget->AddToViewport();
	}
}

void UHeroGameplayAbility_TargetLock::setTargetLockWidgetPosition()
{
	if (!drawnTargetLockWidget || !currentLockedActor)
	{
		cancelTargetLockAbility();
		return;
	}

	//converts 3d to 2d (gets vector to screen position)
	FVector2D screenPosition;
	UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(
		getHeroControllerFromActorInfo(),
		currentLockedActor->GetActorLocation(),
		screenPosition,
		true
	); 

	if (targetLockWidgetSize == FVector2D::ZeroVector) //only loop if its needed
	{
		drawnTargetLockWidget->WidgetTree->ForEachWidget(
			[this](UWidget* foundWidget) //lamda func
			{
				if (USizeBox* foundSizeBox = Cast<USizeBox>(foundWidget))
				{
					targetLockWidgetSize.X = foundSizeBox->GetWidthOverride(); //we can get width and height set from wbp
					targetLockWidgetSize.Y = foundSizeBox->GetHeightOverride();
				}
			}
		);
	}
	
	screenPosition -= (targetLockWidgetSize / 2.f); //center

	drawnTargetLockWidget->SetPositionInViewport(screenPosition, false);


}

void UHeroGameplayAbility_TargetLock::cancelTargetLockAbility()
{
	CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
}

void UHeroGameplayAbility_TargetLock::cleanUp()
{
	availableActorsToLock.Empty(); //empty array (perfomant)

	currentLockedActor = nullptr; //good to have this

	if (drawnTargetLockWidget)
	{
		drawnTargetLockWidget->RemoveFromParent(); //will remove the lock in
	}

	drawnTargetLockWidget = nullptr;
	targetLockWidgetSize = FVector2D::ZeroVector;
}
