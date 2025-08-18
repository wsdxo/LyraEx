#pragma once
#include "CoreMinimal.h"
#include "ItemInteractionType.generated.h"

UENUM()
enum class ETPItemType:uint8
{
	//可拾取物品
	IT_Pickable    			UMETA(DisplayName="Pickable"),
	//场景交互物品
	IT_SceneInteractive    	UMETA(DisplayName="SceneInteractive"),
	//非法物品
	IT_InValid				UMETA(DisplayName="InValid",Hidden),
};
