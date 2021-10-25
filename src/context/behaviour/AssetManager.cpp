#include <blooper/context/behaviour/AssetManager.hpp>

BLOOPER_NAMESPACE_BEGIN

AssetManager::AssetManager(AbstractCoreContext& context)
    : CoreContextualBase(context)
{
}

AssetManager::~AssetManager() = default;


const JuceDrawable*
AssetManager::getIconViewUnsafe(assets::ResourceIndex index)
{
  auto& drawable = drawables[index];
  if (!drawable) drawable = assets::getIconAssetUnsafe(index);

  return drawable.get();
}

BLOOPER_NAMESPACE_END
