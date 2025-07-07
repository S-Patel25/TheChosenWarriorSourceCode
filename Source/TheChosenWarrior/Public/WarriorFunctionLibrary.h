// All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WarriorFunctionLibrary.generated.h" //FUNCTION LIBRARY (good for reusability across different parts of the game (ex. communication between light and heavy attacks for finisher)

class UWarriorAbilitySystemComponent;

UENUM()
enum class EWarriorConfirmType : uint8
{
	Yes,
	No
};

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
};
