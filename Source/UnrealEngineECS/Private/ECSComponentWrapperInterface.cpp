
#include "ECSComponentWrapperInterface.h"
#include "ECSGameInstanceInterface.h"


void UECSComponentWrapper::RegisterComponentWithECS()
{
	if (EntityHandle == FEntity::NullEntity)
	{
		entt::registry& Registry = IECSGameInstanceInterface::GetRegistry(reinterpret_cast<UObject*>(this));
		EntityHandle = Registry.create();
	}
}

//////////////////////////////////////////////////
//////////////////////////////////////////////////
// template <typename Component>
// void IECSTemplateComponentWrapper<Component>::RegisterComponentWithECS()
// {
// 	IECSComponentWrapper::RegisterComponentWithECS();
// 	EntityHandle.AddComponent<Component>(DefaultValues);
// }

//////////////////////////////////////////////////
// template <typename Component>
// void IECSTemplateComponentWrapper<Component>::UpdateECSComponent_Implementation()
// {
// 	EntityHandle.GetComponent<Component>() = DefaultValues;
// }
//
// template <typename Component>
// void IECSTemplateComponentWrapper<Component>::UpdateECSComponent(const Component& NewValues)
// {
// 	DefaultValues = NewValues;
// 	IECSComponentWrapper::Execute_UpdateECSComponent(reinterpret_cast<UObject*>(this));
// }
//
// //////////////////////////////////////////////////
// template <typename Component>
// Component& IECSTemplateComponentWrapper<Component>::GetComponent() const
// {
// 	return EntityHandle.GetComponent<Component>();
// }
