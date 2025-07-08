// All Rights Reserved


#include "Characters/TheChosenWarriorBaseCharacter.h"
#include "AbilitySystem/WarriorAttributeSet.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"

// Sets default values
ATheChosenWarriorBaseCharacter::ATheChosenWarriorBaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false; //dont need the character class to tick every frame!
	PrimaryActorTick.bStartWithTickEnabled = false;

	GetMesh()->bReceivesDecals = false; //decals needed later, don't want this to be on our character!


	warriorAbilitySystemComponent = CreateDefaultSubobject<UWarriorAbilitySystemComponent>(TEXT("WarriorAbilitySystemComponent"));
	warriorAttributeSet = CreateDefaultSubobject<UWarriorAttributeSet>(TEXT("WarriorAttributeSet"));

}

UPawnCombatComponent* ATheChosenWarriorBaseCharacter::getPawnCombatComponent() const
{
	return nullptr;
}

UAbilitySystemComponent* ATheChosenWarriorBaseCharacter::GetAbilitySystemComponent() const
{
	return getWarriorAbilitySystemComponent(); //return getter
}

void ATheChosenWarriorBaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (warriorAbilitySystemComponent)
	{
		warriorAbilitySystemComponent->InitAbilityActorInfo(this, this); //we can use this keywords for both

		ensureMsgf(!characterStartUpData.IsNull(), TEXT("Forgot to assign start up data to %s"), *GetName()); //will trigger a breakpoint and lots of error messages in case null good practice
	}
}

