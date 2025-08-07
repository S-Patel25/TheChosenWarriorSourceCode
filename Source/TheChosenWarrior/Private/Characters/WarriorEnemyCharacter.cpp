// All Rights Reserved


#include "Characters/WarriorEnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/Combat/EnemyCombatComponent.h"
#include "Engine/AssetManager.h"
#include "DataAssets/StartUpData/DataAsset_EnemyStartUpData.h"
#include "Components/UI/EnemyUIComponent.h"
#include "Components/WidgetComponent.h"
#include "Widgets/WarriorWidgetBase.h"
#include "Components/BoxComponent.h"

#include "WarriorDebugHelpers.h"
AWarriorEnemyCharacter::AWarriorEnemyCharacter()
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned; //makes sure the AI is functional when spawned

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 180.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 450.f; //change as needed
	GetCharacterMovement()->BrakingDecelerationWalking = 1000.f; //change as needed

	enemyCombatComponent = CreateDefaultSubobject<UEnemyCombatComponent>("Enemy Combat Component");
	enemyUIComponent = CreateDefaultSubobject<UEnemyUIComponent>("Enemy UI Component");

	enemyHealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("Enemy Health UI Widget Component");
	enemyHealthWidgetComponent->SetupAttachment(GetMesh());

	leftHandCollisionBox = CreateDefaultSubobject<UBoxComponent>("LeftHandCollisionBox");
	leftHandCollisionBox->SetupAttachment(GetMesh()); //change later as not every enemy will have this
	leftHandCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	leftHandCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::onBodyCollisionBoxBeginOverlap);

	rightHandCollisionBox = CreateDefaultSubobject<UBoxComponent>("RightHandCollisionBox");
	rightHandCollisionBox->SetupAttachment(GetMesh()); //change later as not every enemy will have this
	rightHandCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	rightHandCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::onBodyCollisionBoxBeginOverlap);
}

UPawnCombatComponent* AWarriorEnemyCharacter::getPawnCombatComponent() const
{
	return enemyCombatComponent;
}

void AWarriorEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (UWarriorWidgetBase* healthWidget = Cast<UWarriorWidgetBase>(enemyHealthWidgetComponent->GetUserWidgetObject()))
	{
		healthWidget->initEnemyCreateWidget(this);
	}
}

void AWarriorEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	initEnemyStartUpData();
}

#if WITH_EDITOR
void AWarriorEnemyCharacter::PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent) //will attach box collisions to hand for frost giant (or any place for other enemies!)
{
	Super::PostEditChangeProperty(propertyChangedEvent);

	if (propertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, leftHandCollisionBoxAttachBoneName))
	{
		leftHandCollisionBox->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, leftHandCollisionBoxAttachBoneName);
	}

	if (propertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, rightHandCollisionBoxAttachBoneName))
	{
		rightHandCollisionBox->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, rightHandCollisionBoxAttachBoneName);
	}
}
#endif

UPawnUIComponent* AWarriorEnemyCharacter::getPawnUIComponent() const
{
	return enemyUIComponent;
}

UEnemyUIComponent* AWarriorEnemyCharacter::getEnemyUIComponent() const
{
	return enemyUIComponent;
}

void AWarriorEnemyCharacter::onBodyCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void AWarriorEnemyCharacter::initEnemyStartUpData()
{
	if (characterStartUpData.IsNull())
	{
		return;
	}

	UAssetManager::GetStreamableManager().RequestAsyncLoad( //enemies should load async (in the background) as there will be many of them
		characterStartUpData.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda( //lambda delegate for
			[this]()
			{
				if (UDataAsset_StartUpDataBase* loadedData = characterStartUpData.Get())
				{
					loadedData->GiveToAbilitySystemComponent(warriorAbilitySystemComponent);
				}
			}
		)
	);
}
