// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 *
 */
class PALUWORLD_API DebugHelper
{
public:
	DebugHelper();
	~DebugHelper();
};
namespace Debug {
	static void print(const FString& message, const FColor color = FColor::MakeRandomColor(), float showtime = 2.0, int32 inkey = -1) {
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(inkey, showtime, color, message);
		}
		UE_LOG(LogTemp, Warning, TEXT("%s"), *message);
	}
	static void Print(const FString& message, const FColor color = FColor::MakeRandomColor(), float showtime = 2.0, int32 inkey = -1) {
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(inkey, showtime, color, message);
		}
		UE_LOG(LogTemp, Warning, TEXT("%s"), *message);
	}

}