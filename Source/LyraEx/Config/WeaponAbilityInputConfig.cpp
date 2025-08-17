#include "WeaponAbilityInputConfig.h"

const UInputAction* UWeaponAbilityInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag) const
{
	for (const FWeaponInputAction& TmpAction:AbilityInputActions)
	{
		if (TmpAction.InputAction&&TmpAction.InputTag==InputTag)
		{
			return TmpAction.InputAction;
		}
	}
	return nullptr;
}
