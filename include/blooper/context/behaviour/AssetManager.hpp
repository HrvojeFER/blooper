#ifndef BLOOPER_ASSET_MANAGER_HPP
#define BLOOPER_ASSET_MANAGER_HPP

#include <blooper/internal/internal.hpp>

BLOOPER_NAMESPACE_BEGIN

class AssetManager :
    public CoreContextualBase
{
 public:
  explicit AssetManager(AbstractCoreContext& context);
  ~AssetManager() override;

  [[maybe_unused]] std::unique_ptr<JuceDrawable>
  getDrawableUnsafe(assets::ResourceIndex index);

  [[maybe_unused]] inline std::unique_ptr<JuceDrawable>
  getDrawable(assets::DrawableAssetId id)
  {
    return getDrawableUnsafe(static_cast<assets::ResourceIndex>(id));
  }

 private:
  std::unordered_map<
      assets::ResourceIndex,
      std::unique_ptr<JuceDrawable>>
      drawables;
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_ASSET_MANAGER_HPP
