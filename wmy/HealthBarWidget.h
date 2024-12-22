// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class PALWORLD_API UHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;

protected:
	//����Ѫ��������
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthProgress;
	//��ȡ������
	UFUNCTION()
	float GetHealthProgress();

public:
	UPROPERTY(BlueprintReadWrite, Category = "BaseCharacter")
	class ABasePalCharacter* ownerCharacter;

	
};
