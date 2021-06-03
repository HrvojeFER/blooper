#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

AssetManager::AssetManager(AbstractCoreContext& context)
    : CoreContextualBase(context)
{
}

AssetManager::~AssetManager() = default;


std::unique_ptr<JuceDrawable>
AssetManager::getDrawableUnsafe(assets::ResourceIndex index)
{
  auto& drawable = drawables[index];
  if (!drawable) drawable = assets::getDrawableAssetUnsafe(index);

  return drawable->createCopy();
}

BLOOPER_NAMESPACE_END
