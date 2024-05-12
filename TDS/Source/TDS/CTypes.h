
#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "CTypes.generated.h"

UENUM(BlueprintType)
enum class EMovementState : uint8
{
    Aim_State UMETA(DisplayName="Aim State"),
    Walk_State UMETA(DisplayName = "Walk State"),
    Run_State UMETA(DisplayName = "Run State"),
};

USTRUCT(BlueprintType)
struct FCharacterSpeed
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float AimSpeed = 200.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float WalkSpeed = 300.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float RunSpeed = 600.0f;
};

UCLASS()
class TDS_API UCTypes : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
};
