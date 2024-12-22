// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatGameMode.h"
#include "PlayerBaseCharacter.h"
#include "Components/WidgetComponent.h"
#include "DebugHelper.h"
#include "EnemyBaseCharacter.h"
#include "AudioActor.h"
#include "AIController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Soldier1Character.h"
#include "BasePalCharacter.h"

ACombatGameMode::ACombatGameMode()
{
	DefaultPawnClass = APlayerBaseCharacter::StaticClass();
}

void ACombatGameMode::BeginPlay()
{
	UUserWidget* widget = CreateWidget<UUserWidget>(GetWorld(), MainUIClass);

	if (widget)
	{
		widget->AddToViewport();
	}

	if (AudioActorClass) {
		AudioActor=GetWorld()->SpawnActor<AAudioActor>(AudioActorClass);
	}
}

void ACombatGameMode::AddPointToList(AActor* point)
{
	CreatePointList.Add(point);
}

//开始创建敌人
void ACombatGameMode::BeginCreateEnemy()
{
	Debug::print("Begin Create Enemy!");
	
	if (EnemyClassList.Num() > 0)
	{
		int32 rIndex = FMath::RandRange(0, EnemyClassList.Num() - 1);
		int32 pointIndex = FMath::RandRange(0, CreatePointList.Num() - 1);

		FTransform transform = CreatePointList[pointIndex]->GetActorTransform();

		AEnemyBaseCharacter* enemy=  GetWorld()->SpawnActor<AEnemyBaseCharacter>(EnemyClassList[rIndex], transform);

		EnemyList.Add(enemy);
		CreateNum++;

		FLatentActionInfo actionInfo;

		actionInfo.Linkage = 1;
		actionInfo.UUID = 100;
		actionInfo.CallbackTarget = this;
		actionInfo.ExecutionFunction = FName("LoopCreateEnemy");

		UKismetSystemLibrary::Delay(GetWorld(), 3.5f, actionInfo);
	}
}

//循环创建敌人
//当敌人小于一定数值的时候
void ACombatGameMode::LoopCreateEnemy()
{
	if (CreateNum < 5)
	{
		BeginCreateEnemy();
	}
}

void ACombatGameMode::RemoveEnemy(AEnemyBaseCharacter* enemy)
{
	if (enemy)
	{
		EnemyList.Remove(enemy);

		if (EnemyList.Num() == 0)
		//表示所有敌人都被打死了
		{
			IsCreateBoss = true;
		}
	}
}

void ACombatGameMode::AddPointToSoldierList(AActor* point)
{
	SoldierCreatePointList.Add(point);
}

void ACombatGameMode::BeginCreateSoldier()
{
	Debug::print("Begin Create Soldiers!");

	if (SoldierClassList.Num() > 0)
	{
		int32 rIndex = FMath::RandRange(0, SoldierClassList.Num() - 1);
		if (SoldierCreatePointList.Num() <= 0)
			Debug::print("No Points!");
		int32 pointIndex = FMath::RandRange(0, SoldierCreatePointList.Num() - 1);

		FTransform transform = SoldierCreatePointList[pointIndex]->GetActorTransform();

		ASoldier1Character* soldier = GetWorld()->SpawnActor<ASoldier1Character>(SoldierClassList[rIndex], transform);

		SoldierList.Add(soldier);
		SoldierNum++;

		FLatentActionInfo actionInfo;

		actionInfo.Linkage = 1;
		actionInfo.UUID = 100;
		actionInfo.CallbackTarget = this;
		actionInfo.ExecutionFunction = FName("LoopCreateSoldier");

		UKismetSystemLibrary::Delay(GetWorld(), 2.8f, actionInfo);
	}

}

void ACombatGameMode::LoopCreateSoldier()
{
	if (SoldierNum < 5)
	{
		BeginCreateSoldier();
	}
}

void ACombatGameMode::RemoveSoldier(ASoldier1Character* enemy)
{
	if (enemy)
	{
		SoldierList.Remove(enemy);
	}
}

void ACombatGameMode::AddPointToPalList(AActor* point)
{
	PalCreatePointList.Add(point);
}

void ACombatGameMode::BeginCreatePal()
{
	Debug::print("Begin Create Pals!");

	if (PalClassList.Num() > 0)
	{
		int32 rIndex = FMath::RandRange(0, PalClassList.Num() - 1);
		if (PalCreatePointList.Num() <= 0)
			Debug::print("No Points!");
		int32 pointIndex = FMath::RandRange(0, PalCreatePointList.Num() - 1);

		FTransform transform = PalCreatePointList[pointIndex]->GetActorTransform();

		ABasePalCharacter* pal = GetWorld()->SpawnActor<ABasePalCharacter>(PalClassList[rIndex], transform);

		PalList.Add(pal);
		PalNum++;

		FLatentActionInfo actionInfo;

		actionInfo.Linkage = 1;
		actionInfo.UUID = 100;
		actionInfo.CallbackTarget = this;
		actionInfo.ExecutionFunction = FName("LoopCreatePal");

		UKismetSystemLibrary::Delay(GetWorld(), 2.0f, actionInfo);
	}

}

void ACombatGameMode::LoopCreatePal()
{
	if (PalNum < 5)
	{
		BeginCreatePal();
	}
}

void ACombatGameMode::RemovePal(ABasePalCharacter* enemy)
{
	if (enemy)
	{
		PalList.Remove(enemy);
	}
}

void ACombatGameMode::OnVolumeValueChange(float value)
{
	AudioActor->SetAudioVolume(value);
}

void ACombatGameMode::OnMusicPaused(bool ison)
{
	AudioActor->ToggleBGMusic(ison);

}
