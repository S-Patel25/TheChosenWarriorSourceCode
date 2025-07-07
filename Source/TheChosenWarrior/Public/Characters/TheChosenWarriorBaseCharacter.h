// All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "TheChosenWarriorBaseCharacter.generated.h"

class UWarriorAbilitySystemComponent;
class UWarriorAttributeSet;
class UDataAsset_StartUpDataBase;

UCLASS()
class THECHOSENWARRIOR_API ATheChosenWarriorBaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATheChosenWarriorBaseCharacter();

	//Begin IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//End IAbilitySystemInterface

protected:
	
	//Begin APawn Interface (follow UE best practices)
	virtual void PossessedBy(AController* NewController) override;
	//End APawn Interface


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	UWarriorAbilitySystemComponent* warriorAbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	UWarriorAttributeSet* warriorAttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData")
	TSoftObjectPtr<UDataAsset_StartUpDataBase> characterStartUpData; //will use synchronous loading to load this asset


	
public:
	FORCEINLINE UWarriorAbilitySystemComponent* getWarriorAbilitySystemComponent() const { return warriorAbilitySystemComponent; } //inline getter

	FORCEINLINE UWarriorAttributeSet* getWarriorAttributeSet() const { return warriorAttributeSet; }


};
