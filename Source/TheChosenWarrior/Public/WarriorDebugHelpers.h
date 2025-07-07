#pragma once


namespace Debug
{
	static void Print(const FString& Msg, const FColor& Color = FColor::MakeRandomColor(), int32 inKey = -1) //print function to help with debug messages
	{
		if (GEngine) //pointer to engine itself
		{
			GEngine->AddOnScreenDebugMessage(inKey, 7.f, Color, Msg);

			UE_LOG(LogTemp, Warning, TEXT("%s"), *Msg); //print to ue logs aswell
		}
	}
}