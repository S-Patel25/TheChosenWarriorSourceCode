// All Rights Reserved


#include "Characters/WarriorEnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/Combat/EnemyCombatComponent.h"
#include "Engine/AssetManager.h"
#include "DataAssets/StartUpData/DataAsset_EnemyStartUpData.h"
#include "Components/UI/EnemyUIComponent.h"
#include "Components/WidgetComponent.h"
#include "Widgets/WarriorWidgetBase.h"

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

UPawnUIComponent* AWarriorEnemyCharacter::getPawnUIComponent() const
{
	return enemyUIComponent;
}

UEnemyUIComponent* AWarriorEnemyCharacter::getEnemyUIComponent() const
{
	return enemyUIComponent;
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
