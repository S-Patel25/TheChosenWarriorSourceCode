// All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Characters/TheChosenWarriorBaseCharacter.h"
#include "GameplayTagContainer.h"
#include "HeroCharacter.generated.h"

class USpringArmComponent; //forward declare, so it doesn't give errors
class UCameraComponent;
class UDataAsset_InputConfig;
struct FInputActionValue;
class UHeroCombatComponent;
class UHeroUIComponent;

/**
 * 
 */
UCLASS()
class THECHOSENWARRIOR_API AHeroCharacter : public ATheChosenWarriorBaseCharacter
{
	GENERATED_BODY()
	

public:
	AHeroCharacter();

	//Begin IPawnCombatInterface
	virtual UPawnCombatComponent* getPawnCombatComponent() const override;
	//End IPawnCombatInterface

	//Begin IPawnUIInterface
	virtual UPawnUIComponent* getPawnUIComponent() const override;
	virtual UHeroUIComponent* getHeroUIComponent() const override;
	//End IPawnUIInterface

protected:
	//Begin APawn Interface (follow UE best practices)
	virtual void PossessedBy(AController* NewController) override;
	//End APawn Interface

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override; //player input setup
	virtual void BeginPlay() override;

private:

#pragma region Components

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true")) //need to use meta tag as bp's can't see when priv variables
	USpringArmComponent* cameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true")) //always a good practice to have UPROPERTY() even if empty as reflection system will sort out any null pointer or weird issues
	UCameraComponent* followCamera; 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UHeroCombatComponent* heroCombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	UHeroUIComponent* heroUIComponent;

#pragma endregion

#pragma region Inputs

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"))
	class UDataAsset_InputConfig* inputConfigDataAsset; //hard ref since it won't be expensive

	UPROPERTY()
	FVector2D switchDirection = FVector2D::ZeroVector;

	void Input_Move(const FInputActionValue& InputActionValue); //actual movement methods
	void Input_Look(const FInputActionValue& InputActionValue);

	void Input_SwitchTargetTriggered(const FInputActionValue& InputActionValue);
	void Input_SwitchTargetCompleted(const FInputActionValue& InputActionValue);

	void Input_PickUpStonesStarted(const FInputActionValue& InputActionValue);

	void Input_AbilityInputPressed(FGameplayTag inInputTag); //ability inputs
	void Input_AbilityInputReleased(FGameplayTag inInputTag);

#pragma endregion


public:

	FORCEINLINE UHeroCombatComponent* getHeroCombatComponent() const { return heroCombatComponent; } //getter
};
