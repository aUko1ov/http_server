
#pragma once
#include "CoreMinimal.h"
#include "GlobalStructs.generated.h"

USTRUCT(BlueprintType)
struct FPlayerInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo")
	int32 level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo")
	int32 money;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo")
	FString type;
};

UENUM(BlueprintType)
enum class EHttpRequestType : uint8
{
	GET,
	POST,
	PUT
};

UENUM(BlueprintType)
enum class EGameObject : uint8
{
	player_id,
	party_id
};
