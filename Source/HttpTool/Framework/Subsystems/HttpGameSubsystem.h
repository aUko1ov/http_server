
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "HttpTool/Global/GlobalStructs.h"
#include "HttpGameSubsystem.generated.h"


UCLASS()
class HTTPTOOL_API UHttpGameSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHttpResponseReceived, const FString&, ResponseContent);

public:
	FOnHttpResponseReceived OnHttpResponseReceived;
	
	void AddKeyToMap(const int32 Key, const FString& RequestAddress);

	TFuture<bool> CreateRequestAsync(EHttpRequestType HttpType, const int32 Key, const FString& Content = "");

private:
	static FString ConvertRequestTypeToString(EHttpRequestType RequestType);
	TFuture<bool> PerformHttpRequestAsync(EHttpRequestType HttpType, const int32 Key, const FString& Content);

	TMap<int32, FString> RequestMap;
	
};
