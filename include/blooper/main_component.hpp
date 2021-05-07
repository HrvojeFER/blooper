#ifndef BLOOPER_MAIN_COMPONENT_HPP
#define BLOOPER_MAIN_COMPONENT_HPP


namespace blooper
{
class main_component : public audio::Component
{
public:
    main_component();

    void paint(audio::Graphics&) override;
    void resized() override;

private:
    audio::Label _welcome_label{{}, {"Welcome!"}};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(main_component);
};
} // namespace blooper


#endif //BLOOPER_MAIN_COMPONENT_HPP
