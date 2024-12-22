// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MultiplayerSessionsSystem.generated.h"


//�����Ƿ��ַ�������ǵĲ˵����Իص�
//������5�ֹ���һһ��Ӧ
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnCreateSessionComplete, bool, was_successful);
DECLARE_MULTICAST_DELEGATE_TwoParams(FMultiplayerOnFindSessionsComplete, const TArray<FOnlineSessionSearchResult>& session_results, bool was_successful);
DECLARE_MULTICAST_DELEGATE_OneParam(FMultiplayerOnJoineSessionComplete, EOnJoinSessionCompleteResult::Type result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnDestroySessionComplete, bool, was_successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnStartSessionComplete, bool, was_successful);

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UMultiplayerSessionsSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UMultiplayerSessionsSystem();

	//
	//����Ự��������cpp�������Щ
	//
	//�����Ự
	void CreateSession(int32 num_connections, FString matchtype);
	//���һỰ
	void FindSessions(int32 max_results);
	//����Ự
	void JoinSession(const FOnlineSessionSearchResult& result);
	//���ٻỰ
	void DestroySession();
	//��ʼ�Ự
	void StartSession();
	//���ǵ��Զ���delegate�����Ա��ڲ˵��ص�
	FMultiplayerOnCreateSessionComplete create_complete;
	FMultiplayerOnFindSessionsComplete find_complete;
	FMultiplayerOnJoineSessionComplete join_complete;
	FMultiplayerOnDestroySessionComplete destroy_complete;
	FMultiplayerOnStartSessionComplete start_complate;


protected:
	//
	// �ڲ��ص���������
	//
	void OnCreateSessionComplete(FName session_name, bool was_successful);

	void OnFindSessionsComplete(bool was_successful);

	void OnJoinSessionComplete(FName session_name, EOnJoinSessionCompleteResult::Type result);

	void OnDestroySessionComplete(FName session_name, bool was_successful);

	void OnStartSessionComplete(FName session_name, bool was_successful);

private:
	IOnlineSessionPtr session_interface;
	TSharedPtr<FOnlineSessionSettings> last_session_settings;
	TSharedPtr<FOnlineSessionSearch> last_session_search;
	// 
	//�������󶨶�����Ϸ�Ự
	//
	FOnCreateSessionCompleteDelegate create_complete_delegate;
	FDelegateHandle create_complete_delegate_handle;

	FOnFindSessionsCompleteDelegate find_complete_delegate;
	FDelegateHandle find_complete_delegate_handle;

	FOnJoinSessionCompleteDelegate join_complete_delegate;
	FDelegateHandle join_complete_delegate_handle;

	FOnDestroySessionCompleteDelegate destroy_complete_delegate;
	FDelegateHandle destroy_complete_delegate_handle;

	FOnStartSessionCompleteDelegate start_complete_delegate;
	FDelegateHandle start_complete_delegate_handle;

};
