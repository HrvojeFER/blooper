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


  [[maybe_unused]] const JuceDrawable*
  getIconViewUnsafe(assets::ResourceIndex index);

  [[maybe_unused]] inline auto
  getIconView(assets::IconAssetId id)
  {
    return getIconViewUnsafe(static_cast<assets::ResourceIndex>(id));
  }

  [[maybe_unused]] inline auto
  getIconUnsafe(assets::ResourceIndex index)
  {
    return getIconViewUnsafe(index)->createCopy();
  }

  [[maybe_unused]] inline auto
  getIcon(assets::IconAssetId id)
  {
    return getIconUnsafe(static_cast<assets::ResourceIndex>(id));
  }


 private:
  using DrawableCollection [[maybe_unused]] =
      std::unordered_map<assets::ResourceIndex, std::unique_ptr<JuceDrawable>>;

  DrawableCollection drawables;
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_ASSET_MANAGER_HPP
