#ifndef BLOOPER_SYNC_MANAGER_HPP
#define BLOOPER_SYNC_MANAGER_HPP
#pragma once

#include <blooper/internal/macros/namespaces.hpp>
#include <blooper/internal/abstract/time.hpp>
#include <blooper/internal/abstract/stateful.hpp>
#include <blooper/internal/abstract/contextual.hpp>

BLOOPER_NAMESPACE_BEGIN

class Synchronizer final :
    public StatefulBase,
    public ContextualBase,

    public juce::ChangeBroadcaster,

    private juce::Timer
{
 public:
  BLOOPER_STATE_ID(Synchronizer);


  using Callback = std::function<void()>;


  explicit Synchronizer(
      AbstractContext& context,
      State            state);

  ~Synchronizer() override;


  [[maybe_unused, nodiscard]] inline double
  getBpm() const noexcept;

  [[maybe_unused, nodiscard]] inline double
  getMilliseconds(Interval of = Interval::quarterOfBeat) const noexcept;

  [[maybe_unused, nodiscard]] inline double
  getProgress(Interval of) const noexcept;

  // doesn't lock the Synchronizer - use only for speedy UI stuff
  [[maybe_unused, nodiscard]] inline double
  getProgressUnsafe(Interval of) const noexcept;


  [[maybe_unused]] Token everyAsync(Interval _, Callback _do);

  [[maybe_unused]] Token onAsync(Delay _, Callback _do);

  [[maybe_unused]] void cancelAsync(Token messageToken);


  [[maybe_unused]] Token every(Interval _, Callback _do);

  [[maybe_unused]] Token on(Delay _, Callback _do);

  [[maybe_unused]] void cancel(Token messageToken);


 private:
  struct Message
  {
    enum Type
    {
      interval,
      delay
    };

    Token    token;
    Type     type;
    Tick     tick;
    Callback callback;
  };

  using MessageCollection = std::vector<Message>;

  using Lock = juce::SpinLock;
  using ScopedLock = Lock::ScopedLockType;


  JuceCached<double> bpm;

  BLOOPER_ID(currentTickId);
  Tick currentTick;

  HiResTick lastHiResTick;
  HiResTick hiResTicksPerTick;

  MessageCollection messages;
  Lock              lock;


  [[nodiscard]] inline const Lock&
  getLock() const noexcept;


  [[nodiscard]] inline static Message
  createMessage(
      Token    token,
      Interval interval,
      Callback callback) noexcept;

  [[nodiscard]] inline static Message
  createMessage(
      Token    token,
      Delay    delay,
      Callback callback) noexcept;


  inline void addMessage(Message message);

  inline void addMessageAsync(Message message);


  [[nodiscard]] inline static bool
  shouldSend(const Message& message, Tick currentTick) noexcept;

  [[nodiscard]] inline static bool
  shouldRemove(const Message& message, Tick currentTick) noexcept;

  [[nodiscard]] inline static bool
  shouldRemove(const Message& message, Token token) noexcept;


  inline void cycle() noexcept;


  inline void restartTimer();


  inline HiResTick getHiResTicksPerTick() const noexcept;


  // ValueTreeListener

 private:
  void valueTreePropertyChanged(
      juce::ValueTree&        tree,
      const juce::Identifier& id) final;


  // Timer

 private:
  void timerCallback() final;


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Synchronizer)
  JUCE_DECLARE_WEAK_REFERENCEABLE(Synchronizer)
};


double Synchronizer::getBpm() const noexcept
{
  return this->bpm;
}

double Synchronizer::getMilliseconds(Interval of) const noexcept
{
  return static_cast<double>(static_cast<int>(of) * (1000 * 60)) /
         static_cast<double>(this->bpm * 4);
}

double Synchronizer::getProgressUnsafe(Interval of) const noexcept
{
  const auto currentHiResTick =
      juce::Time::getHighResolutionTicks() -
      this->lastHiResTick;

  const auto tickInInterval =
      this->currentTick %
      static_cast<Tick>(of);

  const auto hiResTickInInterval =
      tickInInterval * this->hiResTicksPerTick +
      currentHiResTick;

  const auto hiResTicksPerInterval =
      static_cast<Tick>(of) *
      this->hiResTicksPerTick;

  // this is kinda bad because the tick count could be high,
  // but I believe its not going to affect the quantization of double
  // values too much and clamping the result helps as well
  return std::clamp(
      static_cast<double>(hiResTickInInterval) /
          static_cast<double>(hiResTicksPerInterval),
      0.0,
      1.0);
}

double Synchronizer::getProgress(Interval of) const noexcept
{
  ScopedLock scopedLock(this->getLock());
  return this->getProgressUnsafe(move(of));
}


const Synchronizer::Lock& Synchronizer::getLock() const noexcept
{
  return this->lock;
}

Synchronizer::Message Synchronizer::createMessage(
    Token    token,
    Interval interval,
    Callback callback) noexcept
{
  return Synchronizer::Message{
      token,
      Synchronizer::Message::Type::interval,
      static_cast<Tick>(interval),
      std::move(callback)};
}

Synchronizer::Message Synchronizer::createMessage(
    Token    token,
    Delay    delay,
    Callback callback) noexcept
{
  return Synchronizer::Message{
      token,
      Synchronizer::Message::Type::interval,
      static_cast<Tick>(delay),
      std::move(callback)};
}


void Synchronizer::addMessage(Message message)
{
  ScopedLock scopedLock(this->getLock());
  this->messages.emplace_back(move(message));
}

void Synchronizer::addMessageAsync(Message message)
{
  juce::MessageManager::callAsync(
      [sync = juce::WeakReference<Synchronizer>(this),
       message = move(message)]() mutable {
        if (sync.wasObjectDeleted()) return;

        sync->addMessage(move(message));
      });
}


bool Synchronizer::shouldSend(
    const Message& message,
    Tick           tick) noexcept
{
  // Hopefully the compiler notices what I'm trying to do and optimizes this.
  // If it's a problem, try one of these hacks:
  // https://graphics.stanford.edu/~seander/bithacks.html
  return ((message.tick == 0) || ((tick % message.tick) == 0)) &&
         message.callback;
}

bool Synchronizer::shouldRemove(
    const Message& message,
    Tick           currentTick) noexcept
{
  return Synchronizer::shouldSend(message, currentTick) &&
         message.type == Synchronizer::Message::Type::delay;
}

bool Synchronizer::shouldRemove(
    const Message& message,
    Token          token) noexcept
{
  return message.token == token;
}


void Synchronizer::cycle() noexcept
{
  // ticks go from 0 to 63
  constexpr Tick mask = 63;

  this->currentTick = (this->currentTick + 1) & mask;
  this->lastHiResTick = juce::Time::getHighResolutionTicks();
}


void Synchronizer::restartTimer()
{
  this->stopTimer();
  this->startTimer(
      static_cast<int>(
          this->getMilliseconds()));
}


HiResTick Synchronizer::getHiResTicksPerTick() const noexcept
{
  return juce::Time::secondsToHighResolutionTicks(
      60 / (this->bpm * 4));
}

BLOOPER_NAMESPACE_END

#endif // BLOOPER_SYNC_MANAGER_HPP
