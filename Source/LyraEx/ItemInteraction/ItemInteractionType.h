#pragma once
#include "CoreMinimal.h"
#include "ItemInteractionType.generated.h"

UENUM(BlueprintType)
enum class ETPItemType:uint8
{
	//可拾取物品
	IT_Pickable    			UMETA(DisplayName="Pickable"),
	//场景交互物品
	IT_SceneInteractive    	UMETA(DisplayName="SceneInteractive"),
	//非法物品
	IT_InValid				UMETA(DisplayName="InValid",Hidden),
};

UENUM()
enum class ETPItemPickableType:uint8
{
	PT_Normal,
	PT_Inventary,
	PT_Weapon,
	PT_InValid,
};
