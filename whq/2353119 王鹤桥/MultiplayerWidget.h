// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSessionSettings.h"
#include "Start.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MultiplayerWidget.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UMultiplayerWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void MenuSetup(int32 num_public_connect = 4, FString type_of_match = FString(TEXT("FreeForAll")));

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool is_clicked = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool is_transform = 0;

protected:

	virtual bool Initialize() override;
	virtual void NativeDestruct() override;

	//�Զ���delegate�Ļص�����
	UFUNCTION()
	void OnCreateSession(bool was_successful);
	//���������Ƕ�̬�ģ�����ҪUFUNCTION();
	void OnFindSessions(const TArray<FOnlineSessionSearchResult>& session_results, bool was_successful);
	void OnJoinSession(EOnJoinSessionCompleteResult::Type result);
	UFUNCTION()
	void OnDestroySession(bool was_successful);
	UFUNCTION()
	void OnStartSession(bool was_successful);

private:
	//����ͼ��ϵ����
	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
	UButton* SmallJoin;
	//��������������������
	UPROPERTY(meta = (BindWidget))
	UButton* WorldJoin;
	//��ť�������
	UFUNCTION()
	void HostButtonClicked();


	UFUNCTION()
	void WorldJoinClicked();

	void MenuTearDown();

	//��ϵͳ��ָ��
	class UMultiplayerSessionsSystem* subsystem;

	int32 num_public{ 4 };
	FString matchtype{ TEXT("FreeForAll") };




};
