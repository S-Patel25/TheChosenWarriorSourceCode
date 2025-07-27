// All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WarriorTypes/WarriorEnumTypes.h"
#include "WarriorFunctionLibrary.generated.h" //FUNCTION LIBRARY (good for reusability across different parts of the game (ex. communication between light and heavy attacks for finisher)

class UWarriorAbilitySystemComponent;
class UPawnCombatComponent;

/**
 * 
 */
UCLASS()
class THECHOSENWARRIOR_API UWarriorFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	static UWarriorAbilitySystemComponent* NativeGetWarriorASCFromActor(AActor* InActor); //native to c++ only

	UFUNCTION(BlueprintCallable, Category = "Warrior|FunctionLibrary")
	static void addGameplayTagToActorIfNone(AActor* inActor, FGameplayTag tagToAdd);

	UFUNCTION(BlueprintCallable, Category = "Warrior|FunctionLibrary")
	static void removeGameplayFromActorIfFound(AActor* inActor, FGameplayTag tagToRemove);
	
	static bool NativeDoesActorHaveTag(AActor* inActor, FGameplayTag tagToCheck);

	UFUNCTION(BlueprintCallable, Category = "Warrior|FunctionLibrary", meta = (DisplayName = "Does Actor Have Tag", ExpandEnumAsExecs = "outConfirmType")) //will show this instead of BP_ if we use displayname meta tag
	static void BP_DoesActorHaveTag(AActor* inActor, FGameplayTag tagToCheck, EWarriorConfirmType& outConfirmType); //bp version, prefix with BP_MethodName, using enum gives more execution pins

	static UPawnCombatComponent* nativeGetPawnCombatComponentFromActor(AActor* inActor);

	UFUNCTION(BlueprintCallable, Category = "Warrior|FunctionLibrary", meta = (DisplayName = "Get Pawn Combat Component From Actor", ExpandEnumAsExecs = "outValidType"))
	static UPawnCombatComponent* BP_GetPawnCombatComponentFromActor(AActor* inActor, EWarriorValidType& outValidType);

	UFUNCTION(BlueprintPure, Category = "Warrior|FunctionLibrary")
	static bool isTargetPawnHostile(APawn* queryPawn, APawn* targetPawn);

};
