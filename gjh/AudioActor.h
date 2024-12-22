// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AudioActor.generated.h"


class UAudioComponent;


UCLASS()
class PALUWORLD_API AAudioActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAudioActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="AudioComponent")
	TObjectPtr<UAudioComponent> AudioComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RootComponent")
	TObjectPtr<USceneComponent> Root;
public:	
	UFUNCTION()
	void SetAudioVolume(float volume);

	UFUNCTION()
	void ToggleBGMusic(bool isplay);
};
