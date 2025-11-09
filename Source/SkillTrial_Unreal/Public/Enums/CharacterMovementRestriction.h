#pragma once


enum class ECharacterMovementRestriction : uint32
{
    None = 0,
    AniActivity = 1,
    Dead = 1 << 2,

//    // 최소한 다음 비트를 하나 더 추가해야 Enum 정의가 완료됩니다.
//    // 2번 비트: 예시로 Stunned를 추가
//    Stunned = 1 << 1 UMETA(DisplayName = "Stunned"), // 0x00000002

//    //// 2번 비트: 공중에 띄워진 상태 (Launch)
//    //Launched = 1 << 1, // 0x00000002
//
//    //// 3번 비트: 구르기, 대시 등 이동이 선점된 상태 (Dash/Roll)
//    //ActionOverride = 1 << 2, // 0x00000004
//
//    //// 4번 비트: 애니메이션 재생 중 이동이 금지된 상태 (e.g., 특정 스킬 사용)
//    //AnimLock = 1 << 3, // 0x00000008
//
//    //// 5번 비트: UI 또는 인벤토리 오픈 등으로 입력이 막힌 상태
//    //UILock = 1 << 4, // 0x00000010
//
//    //// ... 필요한 제한 상태를 계속 추가
};