
#include "SPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "HttpTool/Core/Components/HttpConnectorComponent.h"


ASPlayerController::ASPlayerController()
{
	HttpConnectorComponent = CreateDefaultSubobject<UHttpConnectorComponent>(TEXT("HttpConnectorComponent"));
}

void ASPlayerController::BeginPlay()
{
	HttpConnectorComponent->OnGetJsonRequest.AddUniqueDynamic(this, &ASPlayerController::SetPlayerInfo);
	Super::BeginPlay();
	
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	HttpConnectorComponent->GetPlayerInfoFromServer(PlayerId);
}

void ASPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, FString::Printf(TEXT("Level: %d, Money: %d, Type: %s"), PlayerInfo.level, PlayerInfo.money, *PlayerInfo.type));

}


void ASPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(SendPlayerDataAction, ETriggerEvent::Started, this, &ASPlayerController::SendPlayerInfoToServer);
	EnhancedInputComponent->BindAction(RandPlayerParams, ETriggerEvent::Started, this, &ASPlayerController::SetRandPlayerInfo);
}

void ASPlayerController::SetRandPlayerInfo()
{
	PlayerInfo.level = FMath::RandRange(1, 100);
	PlayerInfo.money = FMath::RandRange(1, 100);
}

void ASPlayerController::SendPlayerInfoToServer()
{
	HttpConnectorComponent->SendPlayerInfoToServer(PlayerInfo, PlayerId);
}

void ASPlayerController::SetPlayerInfo(const FPlayerInfo& NewPlayerInfo)
{
	PlayerInfo = NewPlayerInfo;
}
