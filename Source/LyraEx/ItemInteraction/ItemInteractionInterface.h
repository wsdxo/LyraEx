#pragma once
#include "UObject/Interface.h"
#include "ItemInteractionInterface.generated.h"

UINTERFACE()
class UItemInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

class IItemInteractionInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category="Item Interface")
	USkeletalMeshComponent* GetCharacterMesh();
};
