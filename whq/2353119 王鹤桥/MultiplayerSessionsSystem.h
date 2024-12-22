// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MultiplayerSessionsSystem.generated.h"


//当我们发现房间后，我们的菜单可以回调
//与下面5种功能一一对应
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
	//处理会话函数，主cpp会调用这些
	//
	//创建会话
	void CreateSession(int32 num_connections, FString matchtype);
	//查找会话
	void FindSessions(int32 max_results);
	//加入会话
	void JoinSession(const FOnlineSessionSearchResult& result);
	//销毁会话
	void DestroySession();
	//开始会话
	void StartSession();
	//我们的自定义delegate，可以便于菜单回调
	FMultiplayerOnCreateSessionComplete create_complete;
	FMultiplayerOnFindSessionsComplete find_complete;
	FMultiplayerOnJoineSessionComplete join_complete;
	FMultiplayerOnDestroySessionComplete destroy_complete;
	FMultiplayerOnStartSessionComplete start_complate;


protected:
	//
	// 内部回调函数集合
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
	//联网并绑定多人游戏会话
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
