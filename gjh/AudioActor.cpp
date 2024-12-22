// Fill out your copyright notice in the Description page of Project Settings.


#include "AudioActor.h"
#include "Components/AudioComponent.h"
// Sets default values
AAudioActor::AAudioActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);
	AudioComponent = CreateDefaultSubobject<UAudioComponent>("Audio");
	AudioComponent->SetupAttachment(Root);

}

// Called when the game starts or when spawned
void AAudioActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAudioActor::ToggleBGMusic(bool isplay)
{
	AudioComponent->SetPaused(isplay);
}

void AAudioActor::SetAudioVolume( float volume)
{
	AudioComponent->VolumeMultiplier = volume;
}

// Called every frame


