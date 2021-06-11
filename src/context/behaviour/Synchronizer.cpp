#include <blooper/blooper.hpp>

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
      120);

  this->currentTick.referTo(
      this->getState(),
      Synchronizer::currentTickId,
      this->getContext().getUndoManagerPtr(),
      0);


  // There should be one message per track and I doubt
  // anyone will have more than 256 tracks.
  this->messages.reserve(256);


  this->restartTimer();
}

Synchronizer::~Synchronizer()
{
  this->stopTimer();
}


void Synchronizer::everyAsync(
    Interval               interval,
    Synchronizer::Callback callback,
    std::weak_ptr<Token>   tokenOut)
{
  juce::MessageManager::callAsync(
      [sync = juce::WeakReference<Synchronizer>(this),
       interval = move(interval),
       callback = std::move(callback),
       tokenOut = std::move(tokenOut)]() mutable {
        if (sync.wasObjectDeleted()) return;

        auto token = sync->every(move(interval), std::move(callback));
        if (auto sharedToken = tokenOut.lock())
        {
          *sharedToken = move(token);
        }
      });
}

void Synchronizer::onAsync(
    Delay                  delay,
    Synchronizer::Callback callback,
    std::weak_ptr<Token>   tokenOut)
{
  juce::MessageManager::callAsync(
      [sync = juce::WeakReference<Synchronizer>(this),
       delay = move(delay),
       callback = std::move(callback),
       tokenOut = std::move(tokenOut)]() mutable {
        if (sync.wasObjectDeleted()) return;

        auto token = sync->on(move(delay), std::move(callback));
        if (auto sharedToken = tokenOut.lock())
        {
          *sharedToken = move(token);
        }
      });
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
  ScopedLock scopedLock(this->getLock());
  const auto token = generateToken();

  // emplace_back is not working correctly here for some reason
  this->messages.emplace_back(
      Synchronizer::Message{
          token,
          Synchronizer::Message::Type::interval,
          static_cast<Tick>(interval),
          std::move(callback)});

  return token;
}

Token Synchronizer::on(
    Delay                  delay,
    Synchronizer::Callback callback)
{
  ScopedLock scopedLock(this->getLock());
  const auto token = generateToken();

  // emplace_back is not working correctly here for some reason
  this->messages.emplace_back(
      Synchronizer::Message{
          token,
          Synchronizer::Message::Type::delay,
          static_cast<Tick>(delay),
          std::move(callback)});

  return token;
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
      this->restartTimer();
      this->sendChangeMessage();
    }
  }
}

BLOOPER_NAMESPACE_END
