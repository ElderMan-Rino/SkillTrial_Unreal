#pragma once

UENUM(BlueprintType)
enum class EItemState : uint8
{
	EIS_None,
	EIS_Hovering,
	EIS_Equipped,
};