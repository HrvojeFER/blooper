#ifndef BLOOPER_PROPERTY_FILE_THEME_HPP
#define BLOOPER_PROPERTY_FILE_THEME_HPP

#include <blooper/internal/internal.hpp>

BLOOPER_NAMESPACE_BEGIN

class [[maybe_unused]] PropertyFileTheme final :
    public virtual AbstractTheme
{
 public:
  [[maybe_unused]] explicit PropertyFileTheme(const JuceFile& file);

  [[maybe_unused]] explicit PropertyFileTheme(const JuceString& path);


  [[maybe_unused, nodiscard]] bool
  hasColour(JuceColourId id) const final;

  [[maybe_unused, nodiscard]] JuceColour
  getColour(JuceColourId id) const final;


  [[maybe_unused, nodiscard]] inline const JuceXmlFile&
  getFile() const noexcept;

  [[maybe_unused, nodiscard]] inline JuceXmlFile&
  getFile() noexcept;


 private:
  std::unique_ptr<JuceXmlFile> file;
};


const JuceXmlFile& PropertyFileTheme::getFile() const noexcept
{
  return *this->file;
}

JuceXmlFile& PropertyFileTheme::getFile() noexcept
{
  return *this->file;
}

BLOOPER_NAMESPACE_END

#endif // BLOOPER_PROPERTY_FILE_THEME_HPP
