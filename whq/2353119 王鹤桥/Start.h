// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Start.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UStart : public UUserWidget
{
	GENERATED_BODY()
public:

	UPROPERTY(meta = (BindWidget))
	class UImage* Image_1;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* ProgressBar_1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float barValue;

	UFUNCTION(BlueprintCallable)
	void Load();

	// 重写 Tick 函数
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	FTimerDelegate Timerdelegat;

protected:
	// 是否启用 Tick（必须启用）
	virtual void NativeConstruct() override;

};
