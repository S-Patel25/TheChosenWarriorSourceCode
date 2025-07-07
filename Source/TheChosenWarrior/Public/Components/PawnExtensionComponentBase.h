// All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PawnExtensionComponentBase.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THECHOSENWARRIOR_API UPawnExtensionComponentBase : public UActorComponent
{
	GENERATED_BODY()
	

protected:
	template <class T> //very commmon var name
	T* getOwningPawn() const
	{
		static_assert(TPointerIsConvertibleFromTo<T, APawn>::Value, "T param must be derived from APawn!"); //making sure the cast is to the right part!
		return CastChecked<T>(GetOwner()); //helper func
	}

	APawn* getOwningPawn() const //func incase just for pawn and not anything else
	{
		return getOwningPawn<APawn>();
	}

	template <class T>
	T* getOwningController() const
	{
		static_assert(TPointerIsConvertibleFromTo<T, AController>::Value, "T param must be derived from AController!"); 
		return getOwningPawn<APawn>()->GetController<T>();
	}
};
