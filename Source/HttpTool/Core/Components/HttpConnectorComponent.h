
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HttpTool/Global/GlobalStructs.h"
#include "HttpConnectorComponent.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HTTPTOOL_API UHttpConnectorComponent : public UActorComponent
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGetJsonRequest, const FPlayerInfo&, ResponseContent);

public:
	FOnGetJsonRequest OnGetJsonRequest;
	
	void SendPlayerInfoToServer(const FPlayerInfo& PlayerInfo, int32 PlayerId);
	void AddNewPlayerToServer(const FPlayerInfo& PlayerInfo, int32 PlayerId);
	void GetPlayerInfoFromServer(int32 PlayerId);

protected:
	UHttpConnectorComponent();
	
	void SendHttpRequest(EHttpRequestType RequestType, int32 PlayerId, const FString& Content);

	FPlayerInfo JsonToStruct(const FString& jsonString);

	UFUNCTION()
	void SetJsonStructToPlayer(const FString& jsonString);
	
	FString StructToJson(const FPlayerInfo& PlayerInfo);

private:	
	const FString ServerUrl = FString::Printf(TEXT("http://localhost:5000"));

	FString CreateUrlString(EGameObject GameObject, int32 PlayerId) const;
};
