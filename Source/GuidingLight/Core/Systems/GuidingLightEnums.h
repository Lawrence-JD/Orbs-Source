#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

UENUM(BlueprintType)
enum class ERumbleTypeEnum : uint8
{
	RTE_Low	UMETA(DisplayName = "Low"),
	RTE_Medium UMETA(DisplayName = "Medium"),
	RTE_High UMETA(DisplayName = "High")
};