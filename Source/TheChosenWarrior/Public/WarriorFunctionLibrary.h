// All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WarriorTypes/WarriorEnumTypes.h"
#include "WarriorFunctionLibrary.generated.h" //FUNCTION LIBRARY (good for reusability across different parts of the game (ex. communication between light and heavy attacks for finisher)

class UWarriorAbilitySystemComponent;
class UPawnCombatComponent;
struct FScalableFloat;
class UWarriorGameInstance;

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

	UFUNCTION(BlueprintPure, Category = "Warrior|FunctionLibrary", meta = (CompactNodeTitle = "Get Value at Level"))
	static float getScalableFloatValueAtLevel(const FScalableFloat& inScalableFloat, float inLevel = 1.f);

	UFUNCTION(BlueprintPure, Category = "Warrior|FunctionLibrary")
	static FGameplayTag computeHitReactDirectionTag(AActor* inAttacker, AActor* inVictim, float& outAngleDifference);

	UFUNCTION(BlueprintPure, Category = "Warrior|FunctionLibrary")
	static bool isValidBlock(AActor* inAttacker, AActor* inDefender);

	UFUNCTION(BlueprintCallable, Category = "Warrior|FunctionLibrary")
	static bool applyGameplayEffectSpecHandleToTargetActor(AActor* inInstigator, AActor* inTargetActor, const FGameplayEffectSpecHandle& inSpecHandle);

	UFUNCTION(BlueprintCallable, Category = "Warrior|FunctionLibrary", meta = (Latent, WorldContext = "worldContextObject", LatentInfo = "latentInfo", ExpandEnumAsExecs = "countdownInput|countdownOutput", totalTime = "1.0", updateInterval = "0.1")) //creating custom latent function to handle cooldown time for special abilities
	static void countDown(const UObject* worldContextObject, float totalTime, float updateInterval, 
		float& outRemainingTime, EWarriorCountDownActionInput countdownInput, 
		UPARAM(DisplayName = "Output") EWarriorCountDownActionOutput & countdownOutput, FLatentActionInfo latentInfo);

	UFUNCTION(BlueprintPure, Category = "Warrior|FunctionLibrary", meta = (WorldContext = "worldContextObject"))
	static UWarriorGameInstance* getWarriorGameInstance(const UObject* worldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Warrior|FunctionLibrary", meta = (WorldContext = "worldContextObject"))
	static void toggleInputMode(const UObject* worldContextObject, EWarriorInputMode inInputMode);
};
