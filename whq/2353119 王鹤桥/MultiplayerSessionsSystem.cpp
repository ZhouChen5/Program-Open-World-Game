// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerSessionsSystem.h"
#include "OnlineSubsystem.h"
#include "Online/OnlineSessionNames.h"
#include "OnlineSessionSettings.h"

UMultiplayerSessionsSystem::UMultiplayerSessionsSystem() :
	//���delegate����,��ǰ��cpp�����߼�����
	create_complete_delegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionComplete)),
	find_complete_delegate(FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::OnFindSessionsComplete)),
	join_complete_delegate(FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnJoinSessionComplete)),
	destroy_complete_delegate(FOnDestroySessionCompleteDelegate::CreateUObject(this, &ThisClass::OnDestroySessionComplete)),
	start_complete_delegate(FOnStartSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnStartSessionComplete))

{
	IOnlineSubsystem* subsystem = IOnlineSubsystem::Get();
	//�����ϵͳ
	if (subsystem)
	{
		session_interface = subsystem->GetSessionInterface();
	}
}


void UMultiplayerSessionsSystem::CreateSession(int32 num_connections, FString matchtype)
{
	//ִ�д����Ự�߼�
	if (!session_interface.IsValid())
	{
		return;
	}

	auto existing = session_interface->GetNamedSession(NAME_GameSession);
	if (existing != nullptr)
	{
		session_interface->DestroySession(NAME_GameSession);
	}
	//delegate������������ƶ�
	create_complete_delegate_handle = session_interface->AddOnCreateSessionCompleteDelegate_Handle(create_complete_delegate);
	//�ɹ���
	last_session_settings = MakeShareable(new FOnlineSessionSettings());
	//
	last_session_settings->bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" ? true : false;
	//
	last_session_settings->NumPublicConnections = num_connections;
	//
	last_session_settings->bAllowJoinInProgress = true;
	//
	last_session_settings->bAllowJoinViaPresence = true;
	//
	last_session_settings->bShouldAdvertise = true;
	last_session_settings->bUsesPresence = true;
	last_session_settings->Set(FName("MatchType"),matchtype, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	last_session_settings->BuildUniqueId = 1;
	//�����õ���ID�����ظ�

	const ULocalPlayer* local_player = GetWorld()->GetFirstLocalPlayerFromController();
	bool flag = session_interface->CreateSession(*local_player->GetPreferredUniqueNetId(), NAME_GameSession, *last_session_settings);
	if (!flag)
	{
		session_interface->ClearOnCreateSessionCompleteDelegate_Handle(create_complete_delegate_handle);
		//��չ�������Լ����Զ���delegate
		create_complete.Broadcast(false);
		//false����Ϊû�гɹ���������
	}

}


void UMultiplayerSessionsSystem::FindSessions(int32 max_results)
{
	if (!session_interface.IsValid())
	{
		return;
	}
	find_complete_delegate_handle = session_interface->AddOnFindSessionsCompleteDelegate_Handle(find_complete_delegate);
	//���delegate���,��ʱ���ò��һỰ

	last_session_search = MakeShareable(new FOnlineSessionSearch());
	last_session_search->MaxSearchResults = max_results;
	//�����������
	last_session_search->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" ? true : false;
	last_session_search->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	const ULocalPlayer* local_player = GetWorld()->GetFirstLocalPlayerFromController();
	bool flag = session_interface->FindSessions(*local_player->GetPreferredUniqueNetId(), last_session_search.ToSharedRef());

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			15.f,
			FColor::Yellow,
			FString(TEXT("World Join Button Clicked"))
		);
	}

	if (!flag)
	{
		session_interface->ClearOnFindSessionsCompleteDelegate_Handle(find_complete_delegate_handle);
		//���һỰ������Ϸ
		find_complete.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
		//���ݿ������result���ֲ�
	}

}


void UMultiplayerSessionsSystem::JoinSession(const FOnlineSessionSearchResult& result)
{
	//����������ʾ
	if (!session_interface.IsValid())
	{
		join_complete.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
		return;
	}

	join_complete_delegate_handle = session_interface->AddOnJoinSessionCompleteDelegate_Handle(join_complete_delegate);
	//�󶨽ӿ�

	const ULocalPlayer* local_player = GetWorld()->GetFirstLocalPlayerFromController();
	if (!session_interface->JoinSession(*local_player->GetPreferredUniqueNetId(), NAME_GameSession, result))
	{
		session_interface->ClearOnJoinSessionCompleteDelegate_Handle(join_complete_delegate_handle);

		join_complete.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
	}
}


void UMultiplayerSessionsSystem::DestroySession()
{
}


void UMultiplayerSessionsSystem::StartSession()
{
}


void UMultiplayerSessionsSystem::OnCreateSessionComplete(FName session_name, bool was_successful)
{
	if (session_interface)
	{
		session_interface->ClearOnCreateSessionCompleteDelegate_Handle(create_complete_delegate_handle);
	}

	create_complete.Broadcast(was_successful);
}


void UMultiplayerSessionsSystem::OnFindSessionsComplete(bool was_successful)
{
	if (session_interface)
	{
		session_interface->ClearOnFindSessionsCompleteDelegate_Handle(find_complete_delegate_handle);
		//�Զ��嵽�˵�
	}
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			15.f,
			FColor::Yellow,
			FString(TEXT("World icked"))
		);
	}
	//�ò˵�֪������Ч���
	if (last_session_search->SearchResults.Num() <= 0)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Yellow,
				FString(TEXT("No Sessions"))
			);
		}
		find_complete.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
		return;
	}
	find_complete.Broadcast(last_session_search->SearchResults, was_successful);
}


void UMultiplayerSessionsSystem::OnJoinSessionComplete(FName session_name, EOnJoinSessionCompleteResult::Type result)
{
	if (session_interface)
	{
		session_interface->ClearOnJoinSessionCompleteDelegate_Handle(join_complete_delegate_handle);
	}
	//�Զ���delegate
	join_complete.Broadcast(result);
}


void UMultiplayerSessionsSystem::OnDestroySessionComplete(FName session_name, bool was_successful)
{
}


void UMultiplayerSessionsSystem::OnStartSessionComplete(FName session_name, bool was_successful)
{
}


//�Դ˿���ʹ�öԻ��ӿں�������Ự