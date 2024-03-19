
#include "HttpConnectorComponent.h"

#include "JsonObjectConverter.h"
#include "HttpTool/Framework/Subsystems/HttpGameSubsystem.h"
#include "HttpTool/Global/GlobalStructs.h"
#include "Kismet/GameplayStatics.h"


UHttpConnectorComponent::UHttpConnectorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UHttpConnectorComponent::SendHttpRequest(const EHttpRequestType RequestType, int32 PlayerId, const FString& Content = TEXT(""))
{
	UHttpGameSubsystem* HttpGameSubsystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UHttpGameSubsystem>();

	const FString RequestAddress = CreateUrlString(EGameObject::player_id, PlayerId);
	HttpGameSubsystem->AddKeyToMap(PlayerId, RequestAddress);
	HttpGameSubsystem->CreateRequestAsync(RequestType, PlayerId, Content);
	HttpGameSubsystem->OnHttpResponseReceived.AddUniqueDynamic(this, &UHttpConnectorComponent::SetJsonStructToPlayer);
}

void UHttpConnectorComponent::GetPlayerInfoFromServer(int32 PlayerId)
{
	SendHttpRequest(EHttpRequestType::GET, PlayerId);
}

void UHttpConnectorComponent::SendPlayerInfoToServer(const FPlayerInfo& PlayerInfo, int32 PlayerId)
{
	SendHttpRequest(EHttpRequestType::PUT, PlayerId, StructToJson(PlayerInfo));
}

void UHttpConnectorComponent::AddNewPlayerToServer(const FPlayerInfo& PlayerInfo, int32 PlayerId)
{
	SendHttpRequest(EHttpRequestType::POST, PlayerId, StructToJson(PlayerInfo));
}

void UHttpConnectorComponent::SetJsonStructToPlayer(const FString& jsonString)
{
	OnGetJsonRequest.Broadcast(JsonToStruct(jsonString));
}

FString UHttpConnectorComponent::StructToJson(const FPlayerInfo& PlayerInfo)
{
	TSharedRef<FJsonObject> JsonObject = MakeShared<FJsonObject>();

	if(FJsonObjectConverter::UStructToJsonObject(FPlayerInfo::StaticStruct(), &PlayerInfo, JsonObject, 0, 0))
	{
		FString OutputString;
		TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&OutputString);
		FJsonSerializer::Serialize(JsonObject, Writer);
		return OutputString;
	}
    
	return "";
}

FPlayerInfo UHttpConnectorComponent::JsonToStruct(const FString& jsonString)
{
	FPlayerInfo PlayerInfo;
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(jsonString);

	if(FJsonSerializer::Deserialize(Reader, JsonObject) &&
	   FJsonObjectConverter::JsonObjectStringToUStruct(jsonString, &PlayerInfo, 0, 0))
	{
		return PlayerInfo;
	}
	
	return FPlayerInfo(); 
}


FString UHttpConnectorComponent::CreateUrlString(EGameObject GameObject, int32 PlayerId) const
{
	FString PlayerTypeString = *UEnum::GetValueAsString(GameObject);
	
	int32 LastIndex = 0;
	PlayerTypeString.FindLastChar(':', LastIndex);
	PlayerTypeString = PlayerTypeString.Mid(LastIndex + 1);

	return FString::Printf(TEXT("%s/%s/%i"), *ServerUrl, *PlayerTypeString, PlayerId);
}
