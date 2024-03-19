
#include "HttpGameSubsystem.h"

#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

void UHttpGameSubsystem::AddKeyToMap(const int32 Key, const FString& RequestAddress)
{
	RequestMap.Add(Key, RequestAddress);
}

TFuture<bool> UHttpGameSubsystem::CreateRequestAsync(const EHttpRequestType HttpType, const int32 Key, const FString& Content)
{
    return Async(EAsyncExecution::ThreadPool, [this, HttpType, Key, Content]() -> bool
    {
        return PerformHttpRequestAsync(HttpType, Key, Content).Get();
    });
}

TFuture<bool> UHttpGameSubsystem::PerformHttpRequestAsync(const EHttpRequestType HttpType, const int32 Key, const FString& Content)
{
    return Async(EAsyncExecution::ThreadPool, [this, HttpType, Key, Content]() -> bool
    {
        const TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
        const FString* RequestAddress = RequestMap.Find(Key);
        if (!RequestAddress)
        {
            return false;
        }

        Request->SetURL(*RequestAddress);
        Request->SetVerb(ConvertRequestTypeToString(HttpType));
        Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

        if (HttpType != EHttpRequestType::GET)
        {
            Request->SetContentAsString(Content);
        }

        // Используем TSharedPtr для Promise для возможности захвата в лямбду
        auto PromisePtr = MakeShared<TPromise<bool>>();
        auto Future = PromisePtr->GetFuture();
        
        Request->OnProcessRequestComplete().BindLambda([PromisePtr, this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) mutable
        {
            if (bWasSuccessful && Response.IsValid())
            {
                switch (Response->GetResponseCode())
                {
                case 200:
                    OnHttpResponseReceived.Broadcast(Response->GetContentAsString());
                default:;
                }

                PromisePtr->SetValue(true);
            }
            else
            {
                PromisePtr->SetValue(false);
            }
        });
        
        Request->ProcessRequest();

        return Future.Get();
    });
}


FString UHttpGameSubsystem::ConvertRequestTypeToString(EHttpRequestType RequestType)
{
	switch (RequestType)
	{
		case EHttpRequestType::GET: return TEXT("GET");
		case EHttpRequestType::POST: return TEXT("POST");
		case EHttpRequestType::PUT: return TEXT("PUT");
		default: return TEXT("");
	}
}
