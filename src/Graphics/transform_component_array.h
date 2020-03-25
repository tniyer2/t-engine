
#ifndef GRAPHICS_TRANSFORM_COMPONENT_ARRAY_H
#define GRAPHICS_TRANSFORM_COMPONENT_ARRAY_H

#include "transform.h"
#include "core/pooled_component_array.h"

namespace TEngine::Graphics {

class TransformComponentArray : public Core::PooledComponentArray<Transform> {
public:
	using Core::PooledComponentArray<Transform>::PooledComponentArray;

	void setRootTransform();
private:
	void _removeComponent(entity) override;
};
}
#endif
