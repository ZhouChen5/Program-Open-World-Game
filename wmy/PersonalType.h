// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class PALUWORLD_API PersonalType
{
public:
	PersonalType();
	~PersonalType();
};


namespace type 
{
	//创建所有帕鲁的特性

	enum class Type {
		//创建一些固定帕鲁才有的特性
		Legend,            //【传说级帕鲁】攻击-20%，防御+20%，移动速度+15%
		Elf_King,          //【百合女王】草属性攻击伤害增加20%     
		Fire_Emperor,      //【焰煌】火属性攻击伤害增加20%
		Sea_Emperor,       //【覆海龙】水属性攻击伤害增加20% 
		Rock_Emperor,      //【阿努比斯】地属性攻击伤害增加20%
		Holy_Heaven,       //【圣光骑士】无属性攻击伤害增加20%
		Hades,             //【混沌骑士】暗属性攻击伤害增加20%
		Ice_Emperor,       //【唤冬兽】冰属性攻击伤害增加20%
		Thunder_Emperor,   //【波鲁杰克斯】雷属性攻击伤害增加20%
		Divine_Dragon,     //【空涡龙】龙属性攻击伤害增加20%                      //10个

		//创建一些三级特性
		Ferocious,            //攻击+20%
		Tenacious_Body,       //防御+20%
		Swiftness,            //移动速度+30%
		Artisan_Spirit,       //工作速度+50%
		Rare,                 //工作速度+15%，攻击+15%
		Raid_Commander,       //玩家的攻击提升10%
		Iron_wall_Strategist, //玩家的防御提升10%
		Cheerleading_Team,    //玩家的工作速度提升25%
		Logging_Leader,       //玩家的砍伐速度提升25%
		Mining_Chief,         //玩家的挖掘速度提升25%
		Workaholic,           //减少SAN值下降速度+15%
		Diet_Master,          //减少饱腹度下降速度+10%

		//创建一些二级特性
		Brainy,               //攻击+30%，工作速度-50%
		Athlete,              //移动速度+20%
		Earnest,              //工作速度+20%

		//创建一些一级特性
		Brave,                          //攻击+10%
		Tough_Skin,                     //防御+10%
		Agile,                          //移动速度+10%
		Sadist,                         //攻击+15%，防御-10%
		Dominant,                       //攻击+10%，防御-10%
		Rough,                          //攻击-15%，工作速度-10%
		Masochist,                      //防御+15%，攻击-15%
		Fragrant_Grass,                 //草属性攻击伤害增加10%
		Fire_loving,                    //火属性攻击伤害增加10%
		Water_loving,                   //水属性攻击伤害增加10%
		Earth_based_Power,              //地属性攻击伤害增加10%
		Meditative_State,               //无属性攻击伤害增加10%
		Nightfall,                      //暗属性攻击伤害增加10%
		Cold_blooded,                   //冰属性攻击伤害增加10%
		Electric_Capacity,              //雷属性攻击伤害增加10%
		Dragon_Bloodline,               //雷属性攻击伤害增加10%
		Weed_killing_Effect,            //草属性伤害减少10%
		Fire_proof_Performance,         //火属性伤害减少10%
		Water_proof_Performance,        //水属性伤害减少10%
		Earthquake_resistant_Structure, //地属性伤害减少10%
		Sunny_and_Cheerful,             //暗属性伤害减少10%
		Unusual,                        //无属性伤害减少10%
		High_temperature_Constitution,  //冰属性伤害减少10%
		Insulator,                      //雷属性伤害减少10%
		Dragon_slayer,                  //龙属性伤害减少10%
		Workaholic_Animal,              //工作速度+30%，攻击-30%
		Narcissist,                     //工作速度+10，防御-10%
		Positive_Thinking,              //减少SAN值下降速度+10%
		Small_Stomach,                  //减少饱腹度下降速度+10%

		//创建一些负值特性
		Gluttony,                       //增加饱腹度下降速度+10%
		Emotional_Instability,          //增加SAN值下降速度+10%
		Cowardly,                       //攻击-10%
		Frail,                          //防御-10%
		Clumsy,                         //工作速度-10%
		bottomless_stomach,             //增加饱腹度下降速度+15%
		desire_for_destruction,         //增加SAN值下降速度+15%
		pessimism,                      //攻击-20%
		osteoporosis,                   //防御-20%
		loafing_addiction               //工作速度-30%


	};

}