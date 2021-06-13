#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

// Modified from juce::LookAndFeel_V4

class DocumentWindowButton : public juce::Button
{
 public:
  DocumentWindowButton(
      const JuceString& name,
      JuceColour        c,
      juce::Path        normal,
      juce::Path        toggled)
      : juce::Button(name),
        colour(c),
        normalShape(move(normal)),
        toggledShape(move(toggled))
  {
  }

  void paintButton(
      JuceGraphics& g,
      bool          shouldDrawButtonAsHighlighted,
      bool          shouldDrawButtonAsDown) override
  {
    auto backgroundColor =
        this->getLookAndFeel()
            .findColour(ColourId::background);
    g.fillAll(backgroundColor);

    g.setColour(
        (!isEnabled() || shouldDrawButtonAsDown) ?
            colour.withAlpha(0.6f) :
            colour);

    if (shouldDrawButtonAsHighlighted)
    {
      g.fillAll();
      g.setColour(backgroundColor);
    }

    auto& shape = getToggleState() ? toggledShape : normalShape;

    auto reducedRect =
        juce::Justification(juce::Justification::centred)
            .appliedToRectangle(
                juce::Rectangle<int>(
                    getHeight(),
                    getHeight()),
                getLocalBounds())
            .toFloat()
            .reduced((float) getHeight() * 0.3f);

    g.fillPath(
        shape,
        shape.getTransformToScaleToFit(
            reducedRect,
            true));
  }

 private:
  JuceColour colour;
  juce::Path normalShape, toggledShape;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DocumentWindowButton)
};

juce::Button* LookAndFeel::createDocumentWindowButton(
    int buttonType)
{
  juce::Path shape;
  auto       crossThickness = 0.15f;

  if (buttonType == juce::DocumentWindow::closeButton)
  {
    shape.addLineSegment(
        {0.0f, 0.0f, 1.0f, 1.0f},
        crossThickness);
    shape.addLineSegment(
        {1.0f, 0.0f, 0.0f, 1.0f},
        crossThickness);

    return new DocumentWindowButton(
        "close",
        this->findColour(ColourId::red),
        shape,
        shape);
  }

  if (buttonType == juce::DocumentWindow::minimiseButton)
  {
    shape.addLineSegment(
        {0.0f, 0.5f, 1.0f, 0.5f},
        crossThickness);

    return new DocumentWindowButton(
        "minimise",
        this->findColour(ColourId::yellow),
        shape,
        shape);
  }

  if (buttonType == juce::DocumentWindow::maximiseButton)
  {
    shape.addLineSegment(
        {0.5f, 0.0f, 0.5f, 1.0f},
        crossThickness);
    shape.addLineSegment(
        {0.0f, 0.5f, 1.0f, 0.5f},
        crossThickness);

    juce::Path fullscreenShape;
    fullscreenShape.startNewSubPath(45.0f, 100.0f);
    fullscreenShape.lineTo(0.0f, 100.0f);
    fullscreenShape.lineTo(0.0f, 0.0f);
    fullscreenShape.lineTo(100.0f, 0.0f);
    fullscreenShape.lineTo(100.0f, 45.0f);
    fullscreenShape.addRectangle(
        45.0f, 45.0f, 100.0f, 100.0f);
    juce::PathStrokeType(30.0f)
        .createStrokedPath(fullscreenShape, fullscreenShape);

    return new DocumentWindowButton(
        "maximise",
        this->findColour(ColourId::cyan),
        shape,
        fullscreenShape);
  }

  BLOOPER_ASSERT(false);
  return nullptr;
}

void LookAndFeel::positionDocumentWindowButtons(
    juce::DocumentWindow& window,
    int                   titleBarX,
    int                   titleBarY,
    int                   titleBarW,
    int                   titleBarH,
    juce::Button*         minimiseButton,
    juce::Button*         maximiseButton,
    juce::Button*         closeButton,
    bool                  positionTitleBarButtonsOnLeft)
{
  titleBarH = juce::jmin(titleBarH, titleBarH - titleBarY);

  auto buttonW = static_cast<int>(titleBarH * 1.2);

  auto x =
      positionTitleBarButtonsOnLeft ?
          titleBarX :
          titleBarX + titleBarW - buttonW;

  if (closeButton != nullptr)
  {
    closeButton->setBounds(x, titleBarY, buttonW, titleBarH);
    x += positionTitleBarButtonsOnLeft ? buttonW : -buttonW;
  }

  if (positionTitleBarButtonsOnLeft)
    std::swap(minimiseButton, maximiseButton);

  if (maximiseButton != nullptr)
  {
    maximiseButton->setBounds(x, titleBarY, buttonW, titleBarH);
    x += positionTitleBarButtonsOnLeft ? buttonW : -buttonW;
  }

  if (minimiseButton != nullptr)
    minimiseButton->setBounds(x, titleBarY, buttonW, titleBarH);
}

void LookAndFeel::drawDocumentWindowTitleBar(
    juce::DocumentWindow& window,
    juce::Graphics&       g,
    int                   w,
    int                   h,
    int                   titleSpaceX,
    int                   titleSpaceW,
    const juce::Image*    icon,
    bool                  drawTitleTextOnLeft)
{
  if (w * h == 0)
    return;

  auto isActive = window.isActiveWindow();

  g.setColour(this->findColour(ColourId::background));

  g.fillAll();

  juce::Font font(
      (float) h * 0.65f,
      juce::Font::plain);
  g.setFont(font);

  auto textW = font.getStringWidth(window.getName());
  auto iconW = 0;
  auto iconH = 0;
  maybeUnused(iconH);

  if (icon != nullptr)
  {
    iconH = static_cast<int>(font.getHeight());
    iconW = icon->getWidth() * iconH / icon->getHeight() + 4;
  }

  textW = juce::jmin(titleSpaceW, textW + iconW);
  auto textX =
      drawTitleTextOnLeft ?
          titleSpaceX :
          juce::jmax(
              titleSpaceX,
              (w - textW) / 2);

  if (textX + textW > titleSpaceX + titleSpaceW)
    textX = titleSpaceX + titleSpaceW - textW;

  if (icon != nullptr)
  {
    g.setOpacity(isActive ? 1.0f : 0.6f);
    g.drawImageWithin(
        *icon,
        static_cast<int>(textX),
        (h - iconH) / 2,
        iconW,
        iconH,
        juce::RectanglePlacement::centred,
        false);

    textX += iconW;
    textW -= iconW;
  }

  if (window.isColourSpecified(
          juce::DocumentWindow::textColourId))
    g.setColour(
        window.findColour(
            juce::DocumentWindow::textColourId));
  else
    g.setColour(this->findColour(ColourId::white));

  g.drawText(
      window.getName(),
      static_cast<int>(textX),
      0,
      textW,
      h,
      juce::Justification::centredLeft,
      true);
}


void LookAndFeel::drawResizableWindowBorder(
    juce::Graphics&              graphics,
    int                          w,
    int                          h,
    const juce::BorderSize<int>& border,
    juce::ResizableWindow&       window)
{
  LookAndFeel_V2::drawResizableWindowBorder(
      graphics, w, h, border, window);
}

void LookAndFeel::drawResizableFrame(
    juce::Graphics&              graphics,
    int                          w,
    int                          h,
    const juce::BorderSize<int>& size)
{
  LookAndFeel_V2::drawResizableFrame(
      graphics, w, h, size);
}

void LookAndFeel::fillResizableWindowBackground(
    juce::Graphics&              graphics,
    int                          w,
    int                          h,
    const juce::BorderSize<int>& size,
    juce::ResizableWindow&       window)
{
  LookAndFeel_V2::fillResizableWindowBackground(
      graphics, w, h, size, window);
}

void LookAndFeel::drawCornerResizer(
    juce::Graphics& graphics,
    int             w,
    int             h,
    bool            isMouseOver,
    bool            isMouseDragging)
{
  LookAndFeel_V2::drawCornerResizer(
      graphics, w, h, isMouseOver, isMouseDragging);
}

BLOOPER_NAMESPACE_END
