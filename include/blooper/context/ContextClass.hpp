#ifndef BLOOPER_CONTEXT_CLASS_HPP
#define BLOOPER_CONTEXT_CLASS_HPP

#include <blooper/internal/internal.hpp>

BLOOPER_NAMESPACE_BEGIN

class Context : public AbstractContext
{
 public:
  BLOOPER_STATE_ID(Context);

  inline constexpr static auto stateFileName = "state.xml";
  inline constexpr static auto stateFileKey = "contextState";


  struct Options
  {
    std::function<void()> onInitSuccess;
    std::function<void()> onInitFailure;
  } options;

  explicit Context(Options options = {});

  ~Context() override;


  [[maybe_unused, nodiscard]] inline const JuceEngine&
  getEngine() const noexcept final;

  [[maybe_unused, nodiscard]] inline JuceEngine&
  getEngine() noexcept final;


  [[maybe_unused, nodiscard]] inline const State&
  getState() const noexcept final;

  [[maybe_unused, nodiscard]] inline State&
  getState() noexcept final;


  [[maybe_unused, nodiscard]] inline const JuceLookAndFeel&
  getLookAndFeel() const noexcept final;

  [[maybe_unused, nodiscard]] inline JuceLookAndFeel&
  getLookAndFeel() noexcept final;


  [[maybe_unused, nodiscard]] inline const JuceUndoManager&
  getUndoManager() const noexcept final;

  [[maybe_unused, nodiscard]] inline JuceUndoManager&
  getUndoManager() noexcept final;


  [[maybe_unused, nodiscard]] inline const JuceProject&
  getProject() const noexcept final;

  [[maybe_unused, nodiscard]] inline JuceProject&
  getProject() noexcept final;

  [[maybe_unused, nodiscard]] inline JuceProjectConstRef
  getProjectRef() const noexcept final;

  [[maybe_unused, nodiscard]] inline JuceProjectRef
  getProjectRef() noexcept final;


  [[maybe_unused, nodiscard]] inline const JuceEdit&
  getEdit() const noexcept final;

  [[maybe_unused, nodiscard]] inline JuceEdit&
  getEdit() noexcept final;


  [[maybe_unused, nodiscard]] inline const JuceTransport&
  getTransport() const noexcept final;

  [[maybe_unused, nodiscard]] inline JuceTransport&
  getTransport() noexcept final;


 private:
  JuceEngine engine;

  JuceXmlFile stateFile;
  State       state;

  std::unique_ptr<JuceLookAndFeel> lookAndFeel;

  JuceUndoManager undoManager;

  JuceProjectRef project;

  std::unique_ptr<JuceEdit> edit;
  JuceTransport*            transport;


  static inline JuceXmlFile ensureValidStateFile(JuceEngine& engine);

  static inline State ensureValidState(JuceXmlFile& file);


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Context)
};


const JuceEngine& Context::getEngine() const noexcept
{
  return engine;
}

JuceEngine& Context::getEngine() noexcept
{
  return engine;
}


const State& Context::getState() const noexcept
{
  return state;
}

State& Context::getState() noexcept
{
  return state;
}


const JuceLookAndFeel& Context::getLookAndFeel() const noexcept
{
  return *lookAndFeel;
}

JuceLookAndFeel& Context::getLookAndFeel() noexcept
{
  return *lookAndFeel;
}


const JuceUndoManager& Context::getUndoManager() const noexcept
{
  return undoManager;
}

JuceUndoManager& Context::getUndoManager() noexcept
{
  return undoManager;
}


const JuceProject& Context::getProject() const noexcept
{
  return *project;
}

JuceProject& Context::getProject() noexcept
{
  return *project;
}

JuceProjectConstRef Context::getProjectRef() const noexcept
{
  return project;
}

JuceProjectRef Context::getProjectRef() noexcept
{
  return project;
}


const JuceEdit& Context::getEdit() const noexcept
{
  return *edit;
}

JuceEdit& Context::getEdit() noexcept
{
  return *edit;
}


const JuceTransport& Context::getTransport() const noexcept
{
  return *transport;
}

JuceTransport& Context::getTransport() noexcept
{
  return *transport;
}


JuceXmlFile Context::ensureValidStateFile(JuceEngine& engine)
{
  JuceXmlFile::Options options;
  options.millisecondsBeforeSaving = 2000;
  options.storageFormat = JuceXmlFile::storeAsXML;
  options.commonToAllUsers = false;

  auto stateFile =
      engine.getPropertyStorage()
          .getAppPrefsFolder()
          .getChildFile(stateFileName);

  // If not valid, just delete it and it will create a new one when needed.
  if (!JuceXmlFile(stateFile, options).isValidFile())
    stateFile.deleteFile();

  return JuceXmlFile(stateFile, options);
}

State Context::ensureValidState(JuceXmlFile& file)
{
  State state = State::fromXml(*file.getXmlValue(stateFileKey));

  if (!state.isValid() || !state.hasType(stateId))
  {
    state = State(stateId);
    file.setValue(stateFileKey, state.createXml().get());
  }

  return state;
}

BLOOPER_NAMESPACE_END

#endif // BLOOPER_CONTEXT_CLASS_HPP
