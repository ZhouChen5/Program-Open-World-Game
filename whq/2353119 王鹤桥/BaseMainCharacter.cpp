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
	//绑定函数
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	//若成功创建
	if (OnlineSubsystem)
	{
		OnlineSessionInterface = OnlineSubsystem->GetSessionInterface();
		//返回联机会话指针

		//联机Debug信息
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Blue,
				//转成C形式字符串
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
			DeathResponse();//执行死亡相应
		}
		else
			HitResponse();//执行伤害相应
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
	//初始化联机大堂
	UWorld* world = GetWorld();
	if (world)
	{
		world->ServerTravel("/Game/Connect?listen");
	}
}

void ABaseMainCharacter::CallOpenLevel(const FString& address)
{
	//给出地址接口
	UGameplayStatics::OpenLevel(this, *address);
}

void ABaseMainCharacter::CallClientTravel(const FString& address)
{
	APlayerController* playercontroller = GetGameInstance()->GetFirstLocalPlayerController();
	//地址寻找
	if (playercontroller)
	{
		playercontroller->ClientTravel(address, ETravelType::TRAVEL_Absolute);
	}
}

//执行创建会话的逻辑
void ABaseMainCharacter::CreateGameSession()
{
	//选择一个按键，这里是P
	if (!OnlineSessionInterface.IsValid())
	{
		//判断会话有效性
		return;
	}
	//以下是有效逻辑
	auto existing = OnlineSessionInterface->GetNamedSession(NAME_GameSession);
	if (existing != nullptr)
	{
		//现有非空则校徽
		OnlineSessionInterface->DestroySession(NAME_GameSession);
	}

	OnlineSessionInterface->AddOnCreateSessionCompleteDelegate_Handle(delegate);
	//共享指针创建会话
	TSharedPtr<FOnlineSessionSettings> settings = MakeShareable(new FOnlineSessionSettings());


	//以下是联机会话设置
	//非实地匹配，用互联网联机,最大公共游戏人数5，私人游戏人数5
	//游戏运行过程中允许玩家加入
	//steam搜索所在地区的会话
	//允许找到世界上正在进行的会话
	settings->bIsLANMatch = false;
	settings->NumPublicConnections = 5;
	settings->NumPrivateConnections = 5;
	settings->bAllowJoinInProgress = true;
	settings->bAllowJoinViaPresence = true;
	settings->bShouldAdvertise = true;
	settings->bUsesPresence = true;
	settings->bUseLobbiesIfAvailable = true;
	settings->Set(FName("MatchType"), FString("FreeForAll"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	//常量本地player指针
	const ULocalPlayer* localplayer = GetWorld()->GetFirstLocalPlayerFromController();
	OnlineSessionInterface->CreateSession(*localplayer->GetPreferredUniqueNetId(), NAME_GameSession, *settings);
}

void ABaseMainCharacter::OnCreateSessionComplete(FName session, bool issuccessful)
{
	//回调函数
	if (issuccessful)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Blue,
				FString::Printf(TEXT("successful create： %s"), *session.ToString())
			);
		}
	}
	//允许去到大厅打开
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
				FString::Printf(TEXT("Failed to Create： %s"), *session.ToString())
			);
		}
	}
	//检验会话是否创建成功（此处标记进行到此处时成功连接steam）
}

//与键盘相连接
void ABaseMainCharacter::JoinGameSession()
{
	//首先找到会话
	//以下给出会话设置
	if (!OnlineSessionInterface.IsValid())
	{
		return;
	}

	OnlineSessionInterface->AddOnFindSessionsCompleteDelegate_Handle(findsession);

	search = MakeShareable(new FOnlineSessionSearch());
	search->MaxSearchResults = 10000;//设置最大搜索数
	search->bIsLanQuery = false;
	search->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	const ULocalPlayer* local_player = GetWorld()->GetFirstLocalPlayerFromController();
	OnlineSessionInterface->FindSessions(*local_player->GetPreferredUniqueNetId(), search.ToSharedRef());


}

//查找会话
void ABaseMainCharacter::OnFindSessionComplete(bool success)
{
	if (!OnlineSessionInterface.IsValid())
	{
		return;
	}

	//遍历结果
	for (auto result : search->SearchResults)
	{
		//搜索结果名称
		FString ID = result.GetSessionIdStr();
		FString User = result.Session.OwningUserName;
		//是否有匹配
		FString matchtype;
		result.Session.SessionSettings.Get(FName("MatchType"), matchtype);
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Cyan,
				FString::Printf(TEXT("ID: %s，User: %s"), *ID, *User)
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
			//加入会话
			OnlineSessionInterface->AddOnJoinSessionCompleteDelegate_Handle(join);
			//调用连接对话
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
	//ip地址
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

		//一旦连接游戏就会进行操作，最终得到address
		APlayerController* controller = GetGameInstance()->GetFirstLocalPlayerController();
		if (controller)
		{
			controller->ClientTravel(address, ETravelType::TRAVEL_Absolute);
		}
	}
}



