// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseMainCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Online/OnlineSessionNames.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ABaseMainCharacter::ABaseMainCharacter() :
	delegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionComplete)),
	findsession(FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::OnFindSessionComplete)),
	join(FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnJoinSessionComplete))
	//�󶨺���
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	//���ɹ�����
	if (OnlineSubsystem)
	{
		OnlineSessionInterface = OnlineSubsystem->GetSessionInterface();
		//���������Ựָ��

		//����Debug��Ϣ
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Blue,
				//ת��C��ʽ�ַ���
				FString::Printf(TEXT("find session %s"), *OnlineSubsystem->GetSubsystemName().ToString())
			);
		}
	}
}

// Called when the game starts or when spawned
void ABaseMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	health = maxHealth;
}

void ABaseMainCharacter::MyDelay(float delayTime, FName functionName)
{
	FLatentActionInfo ActionInfo;
	ActionInfo.Linkage = 1;
	ActionInfo.UUID = 100;
	ActionInfo.CallbackTarget = this;
	ActionInfo.ExecutionFunction = functionName;

	UKismetSystemLibrary::Delay(GetWorld(), delayTime, ActionInfo);
}

// Called every frame
void ABaseMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseMainCharacter::AttackDamage()
{
}

void ABaseMainCharacter::TakeHitDamage(const float damageValue)
{
	if (health > 0)
	{
		health -= damageValue;
		if (health <= 0)
		{
			health = 0;
			DeathResponse();//ִ��������Ӧ
		}
		else
			HitResponse();//ִ���˺���Ӧ
	}
}

void ABaseMainCharacter::HitResponse()
{
	GetMesh()->GetAnimInstance()->Montage_Play(HitMontage);
}

void ABaseMainCharacter::DeathResponse()
{

}

void ABaseMainCharacter::PlayAttack()
{
}

void ABaseMainCharacter::OpenLobby()
{
	//��ʼ����������
	UWorld* world = GetWorld();
	if (world)
	{
		world->ServerTravel("/Game/Connect?listen");
	}
}

void ABaseMainCharacter::CallOpenLevel(const FString& address)
{
	//������ַ�ӿ�
	UGameplayStatics::OpenLevel(this, *address);
}

void ABaseMainCharacter::CallClientTravel(const FString& address)
{
	APlayerController* playercontroller = GetGameInstance()->GetFirstLocalPlayerController();
	//��ַѰ��
	if (playercontroller)
	{
		playercontroller->ClientTravel(address, ETravelType::TRAVEL_Absolute);
	}
}

//ִ�д����Ự���߼�
void ABaseMainCharacter::CreateGameSession()
{
	//ѡ��һ��������������P
	if (!OnlineSessionInterface.IsValid())
	{
		//�жϻỰ��Ч��
		return;
	}
	//��������Ч�߼�
	auto existing = OnlineSessionInterface->GetNamedSession(NAME_GameSession);
	if (existing != nullptr)
	{
		//���зǿ���У��
		OnlineSessionInterface->DestroySession(NAME_GameSession);
	}

	OnlineSessionInterface->AddOnCreateSessionCompleteDelegate_Handle(delegate);
	//����ָ�봴���Ự
	TSharedPtr<FOnlineSessionSettings> settings = MakeShareable(new FOnlineSessionSettings());


	//�����������Ự����
	//��ʵ��ƥ�䣬�û���������,��󹫹���Ϸ����5��˽����Ϸ����5
	//��Ϸ���й�����������Ҽ���
	//steam�������ڵ����ĻỰ
	//�����ҵ����������ڽ��еĻỰ
	settings->bIsLANMatch = false;
	settings->NumPublicConnections = 5;
	settings->NumPrivateConnections = 5;
	settings->bAllowJoinInProgress = true;
	settings->bAllowJoinViaPresence = true;
	settings->bShouldAdvertise = true;
	settings->bUsesPresence = true;
	settings->bUseLobbiesIfAvailable = true;
	settings->Set(FName("MatchType"), FString("FreeForAll"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	//��������playerָ��
	const ULocalPlayer* localplayer = GetWorld()->GetFirstLocalPlayerFromController();
	OnlineSessionInterface->CreateSession(*localplayer->GetPreferredUniqueNetId(), NAME_GameSession, *settings);
}

void ABaseMainCharacter::OnCreateSessionComplete(FName session, bool issuccessful)
{
	//�ص�����
	if (issuccessful)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Blue,
				FString::Printf(TEXT("successful create�� %s"), *session.ToString())
			);
		}
	}
	//����ȥ��������
	UWorld* world = GetWorld();
	if (world)
	{

		world->ServerTravel(FString("/Game/Connect?listen"));

	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Red,
				FString::Printf(TEXT("Failed to Create�� %s"), *session.ToString())
			);
		}
	}
	//����Ự�Ƿ񴴽��ɹ����˴���ǽ��е��˴�ʱ�ɹ�����steam��
}

//�����������
void ABaseMainCharacter::JoinGameSession()
{
	//�����ҵ��Ự
	//���¸����Ự����
	if (!OnlineSessionInterface.IsValid())
	{
		return;
	}

	OnlineSessionInterface->AddOnFindSessionsCompleteDelegate_Handle(findsession);

	search = MakeShareable(new FOnlineSessionSearch());
	search->MaxSearchResults = 10000;//�������������
	search->bIsLanQuery = false;
	search->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	const ULocalPlayer* local_player = GetWorld()->GetFirstLocalPlayerFromController();
	OnlineSessionInterface->FindSessions(*local_player->GetPreferredUniqueNetId(), search.ToSharedRef());


}

//���һỰ
void ABaseMainCharacter::OnFindSessionComplete(bool success)
{
	if (!OnlineSessionInterface.IsValid())
	{
		return;
	}

	//�������
	for (auto result : search->SearchResults)
	{
		//�����������
		FString ID = result.GetSessionIdStr();
		FString User = result.Session.OwningUserName;
		//�Ƿ���ƥ��
		FString matchtype;
		result.Session.SessionSettings.Get(FName("MatchType"), matchtype);
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Cyan,
				FString::Printf(TEXT("ID: %s��User: %s"), *ID, *User)
			);
		}
		if (matchtype == FString("FreeForAll"))
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(
					-1,
					15.f,
					FColor::Cyan,
					FString::Printf(TEXT("Joining Match Type: %s"), *matchtype)
				);
			}
			//����Ự
			OnlineSessionInterface->AddOnJoinSessionCompleteDelegate_Handle(join);
			//�������ӶԻ�
			const ULocalPlayer* localplayer = GetWorld()->GetFirstLocalPlayerFromController();
			OnlineSessionInterface->JoinSession(*localplayer->GetPreferredUniqueNetId(), NAME_GameSession, result);
		}
	}
}

void ABaseMainCharacter::OnJoinSessionComplete(FName name, EOnJoinSessionCompleteResult::Type result)
{
	if (!OnlineSessionInterface.IsValid())
	{
		return;
	}
	//ip��ַ
	FString address;
	bool flag = OnlineSessionInterface->GetResolvedConnectString(NAME_GameSession, address);
	if (flag)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Yellow,
				FString::Printf(TEXT("Connect string: %s"), *address)
			);
		}

		//һ��������Ϸ�ͻ���в��������յõ�address
		APlayerController* controller = GetGameInstance()->GetFirstLocalPlayerController();
		if (controller)
		{
			controller->ClientTravel(address, ETravelType::TRAVEL_Absolute);
		}
	}
}



