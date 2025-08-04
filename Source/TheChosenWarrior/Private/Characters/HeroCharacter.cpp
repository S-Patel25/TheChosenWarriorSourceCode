// All Rights Reserved


#include "Characters/HeroCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DataAssets/Input/DataAsset_InputConfig.h"
#include "Components/Input/WarriorEnhancedInputComponent.h"
#include "ChosenWarriorGameplayTags.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "DataAssets/StartUpData/DataAsset_HeroStartUpData.h"
#include "Components/Combat/HeroCombatComponent.h"
#include "Components/UI/HeroUIComponent.h"
#include "AbilitySystemBlueprintLibrary.h"


#include "WarriorDebugHelpers.h"

AHeroCharacter::AHeroCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f); //setting custom capsule size

	bUseControllerRotationPitch = false; //dont want camera to move with char
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	cameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom")); //cdo creates what we see on bp!
	cameraBoom->SetupAttachment(GetRootComponent()); //attaching to root (the bp hierarchy)
	cameraBoom->TargetArmLength = 200.f; //adjust as needed
	cameraBoom->SocketOffset = FVector(0.f, 55.f, 65.f); //how off to left and right spring arm is! adjust as needed
	cameraBoom->bUsePawnControlRotation = true;

	followCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	followCamera->SetupAttachment(cameraBoom, USpringArmComponent::SocketName); //attach to spring arm
	followCamera->bUsePawnControlRotation = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f); //rot rate, adjust if needed
	GetCharacterMovement()->MaxWalkSpeed = 400.f; //can change in bp's!
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	heroCombatComponent = CreateDefaultSubobject<UHeroCombatComponent>(TEXT("Hero Combat Component"));
	heroUIComponent = CreateDefaultSubobject<UHeroUIComponent>(TEXT("Hero UI Component"));


}

UPawnCombatComponent* AHeroCharacter::getPawnCombatComponent() const
{
	return heroCombatComponent; //return hero
}

UPawnUIComponent* AHeroCharacter::getPawnUIComponent() const
{
	return heroUIComponent;
}

UHeroUIComponent* AHeroCharacter::getHeroUIComponent() const
{
	return heroUIComponent;
}

void AHeroCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//if (warriorAbilitySystemComponent && warriorAttributeSet)
	//{
	//	const FString ASCText = FString::Printf(TEXT("Owner Actor: %s, Avatar Actor: %s"), *warriorAbilitySystemComponent->GetOwnerActor()->GetActorLabel(), *warriorAbilitySystemComponent->GetAvatarActor()->GetActorLabel()); //to make sure it works
	//	Debug::Print(TEXT("Ability System Component Valid. ") + ASCText, FColor::Green);
	//	Debug::Print(TEXT("AttributeSet Valid. ") + ASCText, FColor::Green);
	//}

	if (!characterStartUpData.IsNull())
	{
		if (UDataAsset_StartUpDataBase* loadedData = characterStartUpData.LoadSynchronous()) //load sync method
		{
			loadedData->GiveToAbilitySystemComponent(warriorAbilitySystemComponent);
		}
	}
}

void AHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	checkf(inputConfigDataAsset, TEXT("Forgot to assign the input data asset!"));

	ULocalPlayer* localPlayer = GetController<APlayerController>()->GetLocalPlayer(); //getting player controller for imc 

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(localPlayer); //similar setup to when doing it the traditioal way (without gameplay tags)

	check(Subsystem); //null check

	Subsystem->AddMappingContext(inputConfigDataAsset->defaultMappingContext, 0);

	UWarriorEnhancedInputComponent* warrirorEnhancedInputComponent = CastChecked<UWarriorEnhancedInputComponent>(PlayerInputComponent); //casting to our custom input component from the player input component, cast will fail if null

	warrirorEnhancedInputComponent->bindNativeInputAction(inputConfigDataAsset, ChosenWarriorGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
	warrirorEnhancedInputComponent->bindNativeInputAction(inputConfigDataAsset, ChosenWarriorGameplayTags::InputTag_Look, ETriggerEvent::Triggered, this, &ThisClass::Input_Look);

	warrirorEnhancedInputComponent->bindNativeInputAction(inputConfigDataAsset, ChosenWarriorGameplayTags::InputTag_SwitchTarget, ETriggerEvent::Triggered, this, &ThisClass::Input_SwitchTargetTriggered);
	warrirorEnhancedInputComponent->bindNativeInputAction(inputConfigDataAsset, ChosenWarriorGameplayTags::InputTag_SwitchTarget, ETriggerEvent::Completed, this, &ThisClass::Input_SwitchTargetCompleted);

	warrirorEnhancedInputComponent->bindAbilityInputAction(inputConfigDataAsset, this, &ThisClass::Input_AbilityInputPressed, &ThisClass::Input_AbilityInputReleased);


}

void AHeroCharacter::BeginPlay()
{
	Super::BeginPlay(); //don't forget to call super!
}

void AHeroCharacter::Input_Move(const FInputActionValue& InputActionValue) //this method to setup the actual movement of the char (same as other projects)
{
	const FVector2D movementVector = InputActionValue.Get<FVector2D>();
	const FRotator movementRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);

	if (movementVector.Y != 0.f) //moving forward backwards
	{
		const FVector forwardDirection = movementRotation.RotateVector(FVector::ForwardVector);

		AddMovementInput(forwardDirection, movementVector.Y); //move the character forward
	}

	if (movementVector.X != 0.f) //moving right left
	{
		const FVector rightDirection = movementRotation.RotateVector(FVector::RightVector);

		AddMovementInput(rightDirection, movementVector.X); //move the character forward
	}
}

void AHeroCharacter::Input_Look(const FInputActionValue& InputActionValue)
{
	const FVector2D lookAxisVector = InputActionValue.Get<FVector2D>();
	
	if (lookAxisVector.X != 0.f) //YAW FOR X, PITCH FOR Y, will move mouse
	{
		AddControllerYawInput(lookAxisVector.X);
	}

	if (lookAxisVector.Y != 0.f)
	{
		AddControllerPitchInput(-lookAxisVector.Y);
	}

}

void AHeroCharacter::Input_SwitchTargetTriggered(const FInputActionValue& InputActionValue)
{
	switchDirection = InputActionValue.Get<FVector2D>();
}

void AHeroCharacter::Input_SwitchTargetCompleted(const FInputActionValue& InputActionValue)
{
	FGameplayEventData Data;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		this,
		switchDirection.X > 0.f ? ChosenWarriorGameplayTags::Player_Event_SwitchTarget_Right : ChosenWarriorGameplayTags::Player_Event_SwitchTarget_Left, //get vector direction then add tag based on it
		Data
	);

	
}

void AHeroCharacter::Input_AbilityInputPressed(FGameplayTag inInputTag)
{
	warriorAbilitySystemComponent->OnAbilityInputPressed(inInputTag);
}

void AHeroCharacter::Input_AbilityInputReleased(FGameplayTag inInputTag)
{
	warriorAbilitySystemComponent->OnAbilityInputReleased(inInputTag);
}
