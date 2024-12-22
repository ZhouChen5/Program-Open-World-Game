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
	//����������³������

	enum class Type {
		//����һЩ�̶���³���е�����
		Legend,            //����˵����³������-20%������+20%���ƶ��ٶ�+15%
		Elf_King,          //���ٺ�Ů���������Թ����˺�����20%     
		Fire_Emperor,      //����͡������Թ����˺�����20%
		Sea_Emperor,       //����������ˮ���Թ����˺�����20% 
		Rock_Emperor,      //����Ŭ��˹�������Թ����˺�����20%
		Holy_Heaven,       //��ʥ����ʿ�������Թ����˺�����20%
		Hades,             //��������ʿ�������Թ����˺�����20%
		Ice_Emperor,       //�������ޡ������Թ����˺�����20%
		Thunder_Emperor,   //����³�ܿ�˹�������Թ����˺�����20%
		Divine_Dragon,     //���������������Թ����˺�����20%                      //10��

		//����һЩ��������
		Ferocious,            //����+20%
		Tenacious_Body,       //����+20%
		Swiftness,            //�ƶ��ٶ�+30%
		Artisan_Spirit,       //�����ٶ�+50%
		Rare,                 //�����ٶ�+15%������+15%
		Raid_Commander,       //��ҵĹ�������10%
		Iron_wall_Strategist, //��ҵķ�������10%
		Cheerleading_Team,    //��ҵĹ����ٶ�����25%
		Logging_Leader,       //��ҵĿ����ٶ�����25%
		Mining_Chief,         //��ҵ��ھ��ٶ�����25%
		Workaholic,           //����SANֵ�½��ٶ�+15%
		Diet_Master,          //���ٱ������½��ٶ�+10%

		//����һЩ��������
		Brainy,               //����+30%�������ٶ�-50%
		Athlete,              //�ƶ��ٶ�+20%
		Earnest,              //�����ٶ�+20%

		//����һЩһ������
		Brave,                          //����+10%
		Tough_Skin,                     //����+10%
		Agile,                          //�ƶ��ٶ�+10%
		Sadist,                         //����+15%������-10%
		Dominant,                       //����+10%������-10%
		Rough,                          //����-15%�������ٶ�-10%
		Masochist,                      //����+15%������-15%
		Fragrant_Grass,                 //�����Թ����˺�����10%
		Fire_loving,                    //�����Թ����˺�����10%
		Water_loving,                   //ˮ���Թ����˺�����10%
		Earth_based_Power,              //�����Թ����˺�����10%
		Meditative_State,               //�����Թ����˺�����10%
		Nightfall,                      //�����Թ����˺�����10%
		Cold_blooded,                   //�����Թ����˺�����10%
		Electric_Capacity,              //�����Թ����˺�����10%
		Dragon_Bloodline,               //�����Թ����˺�����10%
		Weed_killing_Effect,            //�������˺�����10%
		Fire_proof_Performance,         //�������˺�����10%
		Water_proof_Performance,        //ˮ�����˺�����10%
		Earthquake_resistant_Structure, //�������˺�����10%
		Sunny_and_Cheerful,             //�������˺�����10%
		Unusual,                        //�������˺�����10%
		High_temperature_Constitution,  //�������˺�����10%
		Insulator,                      //�������˺�����10%
		Dragon_slayer,                  //�������˺�����10%
		Workaholic_Animal,              //�����ٶ�+30%������-30%
		Narcissist,                     //�����ٶ�+10������-10%
		Positive_Thinking,              //����SANֵ�½��ٶ�+10%
		Small_Stomach,                  //���ٱ������½��ٶ�+10%

		//����һЩ��ֵ����
		Gluttony,                       //���ӱ������½��ٶ�+10%
		Emotional_Instability,          //����SANֵ�½��ٶ�+10%
		Cowardly,                       //����-10%
		Frail,                          //����-10%
		Clumsy,                         //�����ٶ�-10%
		bottomless_stomach,             //���ӱ������½��ٶ�+15%
		desire_for_destruction,         //����SANֵ�½��ٶ�+15%
		pessimism,                      //����-20%
		osteoporosis,                   //����-20%
		loafing_addiction               //�����ٶ�-30%


	};

}