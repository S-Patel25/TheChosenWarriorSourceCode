// All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnExtensionComponentBase.h"
#include "GameplayTagContainer.h"
#include "PawnCombatComponent.generated.h"

class AWarriorWeaponBase;

UENUM(BlueprintType)
enum class EToggleDamageType : uint8 //how they are doing the damage
{
	CurrentEquippedWeapon,
	LeftHand,
	RightHand
};

/**
 * 
 */
UCLASS()
class THECHOSENWARRIOR_API UPawnCombatComponent : public UPawnExtensionComponentBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Warrior|Combat")
	void registerSpawnedWeapon(FGameplayTag inWeaponTagToRegister, AWarriorWeaponBase* inWeaponToRegister, bool bRegisterAsEquippedWeapon = false); //registers to gas

	UFUNCTION(BlueprintCallable, Category = "Warrior|Combat")
	AWarriorWeaponBase* getCharacterCarriedWeaponByTag(FGameplayTag inWeaponTagToGet) const; //gets the weapon

	UPROPERTY(BlueprintReadWrite, Category = "Warrior|Combat")
	FGameplayTag currentEquippedWeaponTag; 

	UFUNCTION(BlueprintCallable, Category = "Warrior|Combat")
	AWarriorWeaponBase* getCharacterCurrentEquippedWeapon() const;

	UFUNCTION(BlueprintCallable, Category = "Warrior|Combat")
	void toggleWeaponCollision(bool bShouldEnable, EToggleDamageType toggleDamageType = EToggleDamageType::CurrentEquippedWeapon);

	virtual void OnHitTargetActor(AActor* hitActor);
	virtual void OnWeaponPulledFromTargetActor(AActor* interactedActor);



private:
	TMap<FGameplayTag, AWarriorWeaponBase*> characterCarriedWeaponMap; //can allow for multiple weapons to be stored!

};
