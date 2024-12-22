// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class PALUWORLD_API Factions
{
public:
	Factions();
	~Factions();
};


//创建一个表示阵营的枚举类
UENUM(BlueprintType)
enum class Faction : uint8{
	FRIEND UMETA(DisplayName = "Friend"),
	ENEMY UMETA(DisplayName = "Enemy"),
	NEUTRAL UMETA(DisplayName = "Nutral"),
	BOSS UMETA(DisplayName = "Boss")
};                     //友方，敌方，中立，boss

//当前阵营的默认对敌处理机制，为相对敌方就进行返回true，为相对友方就返回false
inline bool IsEnemy(Faction& MyFaction, Faction& OtherFaction) {
	switch (MyFaction) {
	case Faction::FRIEND:                         //己方帕鲁的默认索敌机制
		switch (OtherFaction) {
		case Faction::BOSS:
		case Faction::ENEMY:                      //敌方和boss默认为敌方
			return true;
		default:
			return false;
		}
	case Faction::BOSS:                           //boss的索敌机制
	case Faction::ENEMY:                          //敌方帕鲁的索敌机制
		switch (OtherFaction) {
		case Faction::FRIEND:
			return true;
		default:
			return false;                         //己方为敌人
		}
	default:                                      //中立默认没有敌人
		return true;
	}
}

