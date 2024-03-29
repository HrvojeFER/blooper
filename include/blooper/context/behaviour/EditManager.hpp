#ifndef BLOOPER_EDIT_MANAGER_HPP
#define BLOOPER_EDIT_MANAGER_HPP

#include <blooper/internal/internal.hpp>

BLOOPER_NAMESPACE_BEGIN

class EditManager :
    public ContextualBase,
    public StatefulBase
{
 public:
  explicit EditManager(AbstractContext& context);

  ~EditManager() override;


  EditTrackRef add();

  void remove(EditTrack::Id id);


  inline EditTrackRef operator[](EditTrack::Id id);

  EditTrackRef get(EditTrack::Id id);

  template<typename TOnEditTrack>
  inline void visit(TOnEditTrack onEditTrack) const;


  inline void addListener(JuceStateListener* listener);

  inline void removeListener(JuceStateListener* listener);


  [[maybe_unused, nodiscard]] inline bool isAnyTrackSoloed() const noexcept;

  JuceCached<EditTrack::Id> soloed;


 private:
  using EditCollection = std::unordered_map<EditTrack::Id, EditTrackRef>;

  EditCollection edits;


  EditTrackRef add(
      JuceProjectItemRef item,
      JuceUndoManager*   undoManager = nullptr);


  // ValueTreeListener

 private:
  void valueTreePropertyChanged(
      juce::ValueTree&        tree,
      const juce::Identifier& id) override;


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EditManager);
};


EditTrackRef EditManager::operator[](EditTrack::Id id)
{
  return this->get(id);
}

template<typename TOnEditTrack>
void EditManager::visit(TOnEditTrack onEditTrack) const
{
  static_assert(
      isInvokeable(meta::typeid_(onEditTrack), meta::type_c<EditTrackRef>),
      "onEditTrack passed to visit must be an Invokable with EditTrack.");

  // Tried destructuring, but Clion complains about unused variables...
  for (const auto& pair : this->edits)
    onEditTrack(move(pair.second));
}


void EditManager::addListener(JuceStateListener* listener)
{
  this->getState().addListener(listener);
}

void EditManager::removeListener(JuceStateListener* listener)
{
  this->getState().removeListener(listener);
}


[[maybe_unused]] bool EditManager::isAnyTrackSoloed() const noexcept
{
  return this->soloed != EditTrack::invalidId;
}

BLOOPER_NAMESPACE_END

#endif // BLOOPER_EDIT_MANAGER_HPP
