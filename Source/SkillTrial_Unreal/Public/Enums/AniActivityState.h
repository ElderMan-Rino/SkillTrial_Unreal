#pragma once

UENUM(BlueprintType)
enum class EAniActivityState : uint8
{
	// 1. 애니메이션 재생 중이 아님 (캐릭터가 다음 행동을 할 준비가 됨)
	EAS_Inactive	UMETA(DisplayName = "Inactive"),
	// 2. 애니메이션 재생 중 (캐릭터가 다른 행동을 할 수 없는 상태)
	EAS_Active		UMETA(DisplayName = "Active")
};