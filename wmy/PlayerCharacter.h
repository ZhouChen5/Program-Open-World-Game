// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePalCharacter.h"
#include "PlayerCharacter.generated.h"

/**
 * 
 */
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class PALWORLD_API APlayerCharacter : public ABasePalCharacter
{
	GENERATED_BODY()
	
public:
	APlayerCharacter();

	virtual void BeginPlay() override;
	
	virtual void Tick(float deltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* FollowCamera;

	//UFUNCTION(BluePrintCallable, Category = "MyFunction")
	//void InputMove(const FVector2D axis2D);

	//UFUNCTION(BluePrintCallable, Category = "MyFunction")
	//void LookRotation(const FVector2D axis2D);

	//UFUNCTION(BluePrintCallable, Category = "MyFunction")
	//void InputJump(const bool isjump);


};
