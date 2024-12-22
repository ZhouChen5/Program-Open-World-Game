// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class PALUWORLD_API PersonalProperty
{
public:
	PersonalProperty();
	~PersonalProperty();
};

//表示属性的枚举类
UENUM(BlueprintType)
enum class PersonProperty : uint8 {
	NONE UMETA(DisplayName = "None"),
	FIRE UMETA(DisplayName = "Fire"),
	WATER UMETA(DisplayName = "Water"),
	GRASS UMETA(DisplayName = "Grass"),
	THUNDER UMETA(DisplayName = "Thunder"),
	ICE UMETA(DisplayName = "Ice"),
	GROUND UMETA(DisplayName = "Ground"),
	DARK UMETA(DisplayName = "Dark")
};

//获取实际伤害
inline float RealInjury(float Injury, PersonProperty ownerProperty, PersonProperty enemyProperty) {

	//伤害系数
	float coe = 1.0f; 
	//判断属性克制或被克制
	switch (ownerProperty) {
		//火系
	case PersonProperty::FIRE:
		switch (enemyProperty) {
		case PersonProperty::WATER:
			coe = 0.5f;
			break;
		case PersonProperty::ICE:
		case PersonProperty::GRASS:
			coe = 2.0f;
			break;
		}
		break;
		//水系
	case PersonProperty::WATER:
		switch (enemyProperty) {
		case PersonProperty::THUNDER:
			coe = 0.5f;
			break;
		case PersonProperty::FIRE:
			coe = 2.0f;
			break;
		}
		break;
		//草系
	case PersonProperty::GRASS:
		switch (enemyProperty) {
		case PersonProperty::FIRE:
			coe = 0.5f;
			break;
		case PersonProperty::GROUND:
			coe = 2.0f;
			break;
		}
		break;
		//无属性
	case PersonProperty::NONE:
		switch (enemyProperty) {
		case PersonProperty::DARK:
			coe = 0.5f;
			break;
		}
		break;
		//雷系
	case PersonProperty::THUNDER:
		switch (enemyProperty) {
		case PersonProperty::GROUND:
			coe = 0.5f;
			break;
		case PersonProperty::WATER:
			coe = 2.0f;
			break;
		}
		break;
		//冰系
	case PersonProperty::ICE:
		switch (enemyProperty) {
		case PersonProperty::FIRE:
			coe = 0.5f;
			break;
		}
		break;
		//地面系
	case PersonProperty::GROUND:
		switch (enemyProperty) {
		case PersonProperty::GRASS:
			coe = 0.5f;
			break;
		case PersonProperty::THUNDER:
			coe = 2.0f;
			break;
		}
		break;
		//暗系
	default:
		switch (enemyProperty) {
		case PersonProperty::NONE:
			coe = 2.0f;
			break;
		}
		break;
	}
	return Injury * coe;
}