#ifndef BLOOPER_SYNC_MANAGER_HPP
#define BLOOPER_SYNC_MANAGER_HPP

#include <blooper/internal/internal.hpp>

BLOOPER_NAMESPACE_BEGIN

class Synchronizer :
    public ContextualBase,
    public StatefulBase,

    private juce::Timer
{
 public:
  BLOOPER_STATE_ID(Synchronizer);

  BLOOPER_ID(currentTickId);


  using Callback = std::function<void()>;


  explicit Synchronizer(
      AbstractContext& context,
      State            state);

  ~Synchronizer() override;


  Token every(Interval _, Callback _do);

  Token on(Delay _, Callback _do);

  void cancel(Token messageToken);


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

  // This scenario is perfect for a spinlock
  using Lock = juce::SpinLock;
  using ScopedLock = Lock::ScopedLockType;


  JuceCached<double> bpm;
  JuceCached<Tick>   currentTick;

  MessageCollection messages;
  Lock              lock;


  [[nodiscard]] inline const Lock&
  getLock() const noexcept;


  [[nodiscard]] inline static bool
  shouldSend(const Message& message, Tick currentTick) noexcept;

  [[nodiscard]] inline static bool
  shouldRemove(const Message& message, Tick currentTick) noexcept;

  [[nodiscard]] inline static bool
  shouldRemove(const Message& message, Token token) noexcept;


  inline void cycle() noexcept;


  // Timer

 private:
  void
  timerCallback() override;


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Synchronizer)
  JUCE_DECLARE_WEAK_REFERENCEABLE(Synchronizer)
};


const Synchronizer::Lock& Synchronizer::getLock() const noexcept
{
  return this->lock;
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
}

BLOOPER_NAMESPACE_END

#endif // BLOOPER_SYNC_MANAGER_HPP
