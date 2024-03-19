
#pragma once

#include "CoreMinimal.h"

#include "GameFramework/PlayerController.h"
#include "HttpTool/Global/GlobalStructs.h"
#include "SPlayerController.generated.h"


class UInputMappingContext;
class UInputAction;
class UHttpConnectorComponent;

UCLASS()
class HTTPTOOL_API ASPlayerController : public APlayerController
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SendPlayerDataAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> RandPlayerParams;

protected:
	ASPlayerController();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupInputComponent() override;
	
	void SetRandPlayerInfo();
	void SendPlayerInfoToServer();

	UFUNCTION()
	void SetPlayerInfo(const FPlayerInfo& NewPlayerInfo);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Account)
	int32 PlayerId;

	FPlayerInfo PlayerInfo;

private:	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UHttpConnectorComponent> HttpConnectorComponent;
	
};
