#ifndef BLOOPER_EDIT_MANAGER_HPP
#define BLOOPER_EDIT_MANAGER_HPP
#pragma once

#include <blooper/internal/macros/namespaces.hpp>
#include <blooper/internal/abstract/juceTraits.hpp>
#include <blooper/internal/abstract/visitors.hpp>
#include <blooper/internal/abstract/stateful.hpp>
#include <blooper/internal/abstract/contextual.hpp>
#include <blooper/internal/utils/FlaggedAsyncUpdater.hpp>
#include <blooper/internal/utils/EditTrack.hpp>

BLOOPER_NAMESPACE_BEGIN

class EditManager :
    public ContextualBase,
    public StatefulBase,

    private util::FlaggedAsyncUpdater
{
 public:
  BLOOPER_STATE_ID(EditManager);


  explicit EditManager(
      AbstractContext& context,
      State            state);

  ~EditManager() override;


  JuceCached<int> focusedEdit;

  [[maybe_unused, nodiscard]] inline bool
  isFocusedEdit(const JuceEdit&) const noexcept;

  [[maybe_unused, nodiscard]] inline JuceEditConstRef
  getFocusedEdit() const noexcept;

  [[maybe_unused, nodiscard]] inline JuceEditRef
  getFocusedEdit() noexcept;

  [[maybe_unused, nodiscard]] inline JuceEditRef
  setFocusedEdit(const JuceEdit& edit) noexcept;


  [[maybe_unused, nodiscard]] inline const JuceEdit&
  getMasterEdit() const noexcept;

  [[maybe_unused, nodiscard]] inline JuceEdit&
  getMasterEdit() noexcept;


  [[maybe_unused, nodiscard]] inline const JuceTransport&
  getMasterTransport() const noexcept;

  [[maybe_unused, nodiscard]] inline JuceTransport&
  getMasterTransport() noexcept;


  [[maybe_unused, nodiscard]] inline const JuceTempo&
  getMasterTempo() const noexcept;

  [[maybe_unused, nodiscard]] inline JuceTempo&
  getMasterTempo() noexcept;


  [[maybe_unused, nodiscard]] inline const JuceTempoSetting&
  getMasterBpmSetting() const noexcept;

  [[maybe_unused, nodiscard]] inline JuceTempoSetting&
  getMasterBpmSetting() noexcept;


  [[maybe_unused, nodiscard]] inline JuceEditRef operator[](int id);

  [[maybe_unused, nodiscard]] inline JuceEditConstRef operator[](int id) const;

  [[maybe_unused, nodiscard]] JuceEditRef get(int id);

  [[maybe_unused, nodiscard]] JuceEditConstRef get(int id) const;


  template<typename TOnEditTrack>
  [[maybe_unused]] inline void visit(TOnEditTrack onEditTrack) const;


  JuceEditRef add();

  void remove(int id);


  inline void addListener(JuceStateListener* listener);

  inline void removeListener(JuceStateListener* listener);


 private:
  using EditCollection =
      std::unordered_map<
          int,
          JuceEditRef>;

  EditCollection edits;


  JuceCached<double> bpm;

  JuceEditRef           masterEdit;
  JuceTransportRef      masterTransport{};
  JuceTempoRef          masterTempo{};
  JuceTempoSettingRef   masterBpmSetting;
  JuceTimeSigSettingRef masterTimeSigSetting;

  void ensureMasterEdit();

  [[nodiscard]] bool isMasterEdit(int id) const;


  bool inputsUpdate;

  void updateInputs() const;

  void updateInputs(JuceEdit& edit) const;


  bool bpmUpdate{};

  void updateBpm() const;

  void updateBpm(JuceEdit& edit) const;


  JuceEditRef add(
      JuceProjectItem& item,
      JuceUndoManager* undoManager = nullptr);


  // ValueTreeListener

 private:
  void valueTreePropertyChanged(
      juce::ValueTree&        tree,
      const juce::Identifier& id) override;

  void valueTreeChildAdded(
      juce::ValueTree& tree,
      juce::ValueTree& value_tree) override;

  void valueTreeChildRemoved(
      juce::ValueTree& tree,
      juce::ValueTree& value_tree,
      int              i) override;

  void valueTreeChildOrderChanged(
      juce::ValueTree& tree,
      int              i,
      int              i1) override;


  // FlaggedAsyncUpdater

 private:
  void handleAsyncUpdate() override;


  // Declarations

 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EditManager)
};


bool EditManager::isFocusedEdit(const JuceEdit& edit) const noexcept
{
  return edit.getProjectItemID().getItemID() == this->focusedEdit.get();
}

JuceEditConstRef EditManager::getFocusedEdit() const noexcept
{
  return this->get(this->focusedEdit.get());
}

JuceEditRef EditManager::getFocusedEdit() noexcept
{
  return this->get(this->focusedEdit.get());
}

JuceEditRef EditManager::setFocusedEdit(const JuceEdit& edit) noexcept
{
  this->focusedEdit = edit.getProjectItemID().getItemID();
  return this->getFocusedEdit();
}


const JuceEdit& EditManager::getMasterEdit() const noexcept
{
  return *this->masterEdit;
}

JuceEdit& EditManager::getMasterEdit() noexcept
{
  return *this->masterEdit;
}


const JuceTransport& EditManager::getMasterTransport() const noexcept
{
  return *this->masterTransport;
}

JuceTransport& EditManager::getMasterTransport() noexcept
{
  return *this->masterTransport;
}


const JuceTempo& EditManager::getMasterTempo() const noexcept
{
  return *this->masterTempo;
}

JuceTempo& EditManager::getMasterTempo() noexcept
{
  return *this->masterTempo;
}


const JuceTempoSetting& EditManager::getMasterBpmSetting() const noexcept
{
  return *this->masterBpmSetting;
}

JuceTempoSetting& EditManager::getMasterBpmSetting() noexcept
{
  return *this->masterBpmSetting;
}


JuceEditRef EditManager::operator[](int id)
{
  return this->get(id);
}

JuceEditConstRef EditManager::operator[](int id) const
{
  return this->get(id);
}


template<typename TOnEditTrack>
void EditManager::visit(TOnEditTrack onEditTrack) const
{
  constexpr auto visitorOf =
      meta::typeid_(onEditTrack) ^ isVisitorOf;

  static_assert(
      meta::or_(
          visitorOf(meta::type_c<JuceEditRef>),
          visitorOf(meta::type_c<JuceEdit*>),
          visitorOf(meta::type_c<JuceEdit&>)),
      "onEditTrack passed to visit must be a Visitor of JuceEditRef, "
      "JuceEdit* or JuceEdit&.");


  // Tried destructuring, but Clion complains about unused variables...
  for (auto [id, edit] : this->edits)
  {
    if (!this->isMasterEdit(id))
    {
      if constexpr (isAnyStoppingVisitor(meta::typeid_(onEditTrack)))
      {
        if constexpr (visitorOf(meta::type_c<JuceEditRef>))
        {
          if (!onEditTrack(std::move(edit))) break;
        }
        else if constexpr (visitorOf(meta::type_c<JuceEdit*>))
        {
          if (!onEditTrack(edit.get())) break;
        }
        else if constexpr (visitorOf(meta::type_c<JuceEdit&>))
        {
          if (!onEditTrack(*edit)) break;
        }
      }
      else
      {
        if constexpr (visitorOf(meta::type_c<JuceEditRef>))
        {
          onEditTrack(std::move(edit));
        }
        else if constexpr (visitorOf(meta::type_c<JuceEdit*>))
        {
          onEditTrack(edit.get());
        }
        else if constexpr (visitorOf(meta::type_c<JuceEdit&>))
        {
          onEditTrack(*edit);
        }
      }
    }
  }
}


void EditManager::addListener(JuceStateListener* listener)
{
  this->getState().addListener(listener);
}

void EditManager::removeListener(JuceStateListener* listener)
{
  this->getState().removeListener(listener);
}

BLOOPER_NAMESPACE_END

#endif // BLOOPER_EDIT_MANAGER_HPP
