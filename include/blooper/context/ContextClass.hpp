#ifndef BLOOPER_CONTEXT_CLASS_HPP
#define BLOOPER_CONTEXT_CLASS_HPP


#include <blooper/context/core/core.hpp>


BLOOPER_NAMESPACE_BEGIN

class Context : public CoreContext
{
public:
    template<
            typename OnInitSuccess,
            typename OnInitFailure,
            decltype(std::declval<OnInitSuccess>()(),
                     std::declval<OnInitFailure>()())* = nullptr>
    inline Context(OnInitSuccess onInitSuccess,
                   OnInitFailure onInitFailure);

    ~Context() override;


    [[maybe_unused, nodiscard]] inline const te::Edit&
    getEdit() const noexcept;

    [[maybe_unused, nodiscard]] inline te::Edit&
    getEdit() noexcept;


    [[maybe_unused, nodiscard]] inline const te::TransportControl&
    getTransport() const noexcept;

    [[maybe_unused, nodiscard]] inline te::TransportControl&
    getTransport() noexcept;


    [[maybe_unused, nodiscard]] inline const te::Project&
    getProject() const noexcept;

    [[maybe_unused, nodiscard]] inline te::Project&
    getProject() noexcept;


private:
    te::Project::Ptr project;

    std::unique_ptr<te::Edit> edit;
    te::TransportControl*     transport;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Context)
};


const te::Edit& Context::getEdit() const noexcept
{
    return *edit;
}

te::Edit& Context::getEdit() noexcept
{
    return *edit;
}


const te::TransportControl& Context::getTransport() const noexcept
{
    return *transport;
}

te::TransportControl& Context::getTransport() noexcept
{
    return *transport;
}


const te::Project& Context::getProject() const noexcept
{
    return *project;
}

te::Project& Context::getProject() noexcept
{
    return *project;
}


template<
        typename OnInitSuccess,
        typename OnInitFailure,
        decltype(std::declval<OnInitSuccess>()(),
                 std::declval<OnInitFailure>()())*>
Context::Context(OnInitSuccess onInitSuccess,
                 OnInitFailure onInitFailure)
    : CoreContext(),

      project(),

      edit(),
      transport()
{
    auto& ui = dynamic_cast<blooper::UIBehaviour&>(
            engine->getUIBehaviour());

    ui.showProjectScreen(
            [this, onInitSuccess](auto projectRef) {
                this->project = std::move(projectRef);

                this->edit = ext::project::ensureEdit(
                        this->getProject(),
                        this->getEngine());

                this->transport = &getEdit().getTransport();

                onInitSuccess();
            },
            [this, onInitFailure] {
                onInitFailure();
            });
}

BLOOPER_NAMESPACE_END


#endif // BLOOPER_CONTEXT_CLASS_HPP
