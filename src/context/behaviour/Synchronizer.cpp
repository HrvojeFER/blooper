#include <blooper/context/behaviour/Synchronizer.hpp>

#include <blooper/internal/abstract/id.hpp>
#include <blooper/internal/abstract/const.hpp>

BLOOPER_NAMESPACE_BEGIN

Synchronizer::Synchronizer(
    AbstractContext& context,
    State            state)
    : ContextualBase(context),
      StatefulBase(move(state))
{
  this->bpm.referTo(
      this->getContext().getProjectSettings(),
      id::bpm,
      this->getContext().getUndoManagerPtr(),
      defaultBpm);


  // There should be one message per track and I doubt
  // anyone will have more than 256 tracks.
  this->messages.reserve(256);


  this->currentTick =
      static_cast<Tick>(
          static_cast<int>(
              this->getState().getProperty(
                  Synchronizer::currentTickId,
                  0)));

  this->lastHiResTick = juce::Time::getHighResolutionTicks();
  this->hiResTicksPerTick = this->getHiResTicksPerTick();


  this->restartTimer();
}

Synchronizer::~Synchronizer()
{
  this->stopTimer();

  this->getState().setProperty(
      Synchronizer::currentTickId,
      this->currentTick,
      nullptr);
}


Token Synchronizer::everyAsync(
    Interval               interval,
    Synchronizer::Callback callback)
{
  auto token = generateToken();

  auto message =
      Synchronizer::createMessage(
          token,
          move(interval),
          std::move(callback));

  this->addMessageAsync(move(message));

  return move(token);
}

Token Synchronizer::onAsync(
    Delay                  delay,
    Synchronizer::Callback callback)
{
  auto token = generateToken();

  auto message =
      Synchronizer::createMessage(
          token,
          move(delay),
          std::move(callback));

  this->addMessageAsync(move(message));

  return move(token);
}

void Synchronizer::cancelAsync(Token token)
{
  juce::MessageManager::callAsync(
      [sync = juce::WeakReference<Synchronizer>(this),
       token = move(token)]() mutable {
        if (sync.wasObjectDeleted()) return;

        sync->cancel(move(token));
      });
}


Token Synchronizer::every(
    Interval               interval,
    Synchronizer::Callback callback)
{
  auto token = generateToken();

  auto message =
      Synchronizer::createMessage(
          token,
          move(interval),
          std::move(callback));

  this->addMessage(move(message));

  return move(token);
}

Token Synchronizer::on(
    Delay                  delay,
    Synchronizer::Callback callback)
{
  auto token = generateToken();

  auto message =
      Synchronizer::createMessage(
          token,
          move(delay),
          std::move(callback));

  this->addMessage(move(message));

  return move(token);
}

void Synchronizer::cancel(Token token)
{
  ScopedLock scopedLock(this->getLock());
  const auto begin = this->messages.begin();
  const auto end = this->messages.end();

  this->messages.erase(
      std::remove_if(
          begin,
          end,
          [token](auto& message) {
            return Synchronizer::shouldRemove(message, token);
          }),
      end);
}


// Timer

void Synchronizer::timerCallback()
{
  ScopedLock scopedLock(this->getLock());

  const auto begin = this->messages.begin();
  const auto end = this->messages.end();

  std::for_each(
      begin,
      end,
      [this](auto& message) {
        if (Synchronizer::shouldSend(
                message,
                this->currentTick))
          message.callback();
      });

  this->messages.erase(
      std::remove_if(
          begin,
          end,
          [this](auto& message) {
            return Synchronizer::shouldRemove(
                message,
                this->currentTick);
          }),
      end);

  this->cycle();
}


// ValueTreeListener

void Synchronizer::valueTreePropertyChanged(
    juce::ValueTree&        tree,
    const juce::Identifier& id)
{
  if (tree == this->getContext().getProjectSettings())
  {
    if (id == id::bpm)
    {
      this->hiResTicksPerTick = this->getHiResTicksPerTick();
      this->restartTimer();
      this->sendChangeMessage();
    }
  }
}

BLOOPER_NAMESPACE_END
