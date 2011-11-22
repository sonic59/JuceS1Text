/*
  ==============================================================================

   This file is part of the JUCE library - "Jules' Utility Class Extensions"
   Copyright 2004-11 by Raw Material Software Ltd.

  ------------------------------------------------------------------------------

   JUCE can be redistributed and/or modified under the terms of the GNU General
   Public License (Version 2), as published by the Free Software Foundation.
   A copy of the license is included in the JUCE distribution, or can be found
   online at www.gnu.org/licenses.

   JUCE is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

  ------------------------------------------------------------------------------

   To release a closed-source product which uses JUCE, commercial licenses are
   available: visit www.rawmaterialsoftware.com/juce for more information.

  ==============================================================================
*/

BEGIN_JUCE_NAMESPACE

//==============================================================================
FrameLabel::FrameLabel (const String& name,
              const String& labelText)
    : Component (name),
      textValue (labelText),
      lastTextValue (labelText),
      font (15.0f),
      justification (Justification::centredLeft),
      horizontalBorderSize (5),
      verticalBorderSize (1),
      minimumHorizontalScale (0.7f),
      editSingleClick (false),
      editDoubleClick (false),
      lossOfFocusDiscardsChanges (false)
{
    setColour (TextEditor::textColourId, Colours::black);
    setColour (TextEditor::backgroundColourId, Colours::transparentBlack);
    setColour (TextEditor::outlineColourId, Colours::transparentBlack);

    textValue.addListener (this);

    textValues = new OwnedArray<AttributedString>;
}

FrameLabel::~FrameLabel()
{
    textValue.removeListener (this);

    if (ownerComponent != nullptr)
        ownerComponent->removeComponentListener (this);

    editor = nullptr;
}

//==============================================================================
void FrameLabel::setText (const String& newText,
                     const bool broadcastChangeMessage)
{
    hideEditor (true);

    if (lastTextValue != newText)
    {
        lastTextValue = newText;
        textValue = newText;
        repaint();

        textWasChanged();

        if (ownerComponent != nullptr)
            componentMovedOrResized (*ownerComponent, true, true);

        if (broadcastChangeMessage)
            callChangeListeners();
    }
}

void FrameLabel::setParagraphs (ScopedPointer<OwnedArray<AttributedString> > newText,
                                const bool broadcastChangeMessage)
{
    hideEditor (true);

    textValues = newText;
    repaint();

    textWasChanged();

    if (ownerComponent != nullptr)
        componentMovedOrResized (*ownerComponent, true, true);

    if (broadcastChangeMessage)
        callChangeListeners();
}

String FrameLabel::getText (const bool returnActiveEditorContents) const
{
    return (returnActiveEditorContents && isBeingEdited())
                ? editor->getText()
                : textValue.toString();
}

OwnedArray<AttributedString>& FrameLabel::getParagraphs (const bool) const
{
    return *textValues;
}

void FrameLabel::valueChanged (Value&)
{
    if (lastTextValue != textValue.toString())
        setText (textValue.toString(), true);
}

//==============================================================================
void FrameLabel::setFont (const Font& newFont)
{
    if (font != newFont)
    {
        font = newFont;
        repaint();
    }
}

const Font& FrameLabel::getFont() const noexcept
{
    return font;
}

void FrameLabel::setEditable (const bool editOnSingleClick,
                         const bool editOnDoubleClick,
                         const bool lossOfFocusDiscardsChanges_)
{
    editSingleClick = editOnSingleClick;
    editDoubleClick = editOnDoubleClick;
    lossOfFocusDiscardsChanges = lossOfFocusDiscardsChanges_;

    setWantsKeyboardFocus (editOnSingleClick || editOnDoubleClick);
    setFocusContainer (editOnSingleClick || editOnDoubleClick);
}

void FrameLabel::setJustificationType (const Justification& newJustification)
{
    if (justification != newJustification)
    {
        justification = newJustification;
        repaint();
    }
}

void FrameLabel::setBorderSize (int h, int v)
{
    if (horizontalBorderSize != h || verticalBorderSize != v)
    {
        horizontalBorderSize = h;
        verticalBorderSize = v;
        repaint();
    }
}

//==============================================================================
Component* FrameLabel::getAttachedComponent() const
{
    return static_cast<Component*> (ownerComponent);
}

void FrameLabel::attachToComponent (Component* owner, const bool onLeft)
{
    if (ownerComponent != nullptr)
        ownerComponent->removeComponentListener (this);

    ownerComponent = owner;

    leftOfOwnerComp = onLeft;

    if (ownerComponent != nullptr)
    {
        setVisible (owner->isVisible());
        ownerComponent->addComponentListener (this);
        componentParentHierarchyChanged (*ownerComponent);
        componentMovedOrResized (*ownerComponent, true, true);
    }
}

void FrameLabel::componentMovedOrResized (Component& component, bool /*wasMoved*/, bool /*wasResized*/)
{
    if (leftOfOwnerComp)
    {
        setSize (jmin (getFont().getStringWidth (textValue.toString()) + 8, component.getX()),
                 component.getHeight());

        setTopRightPosition (component.getX(), component.getY());
    }
    else
    {
        setSize (component.getWidth(),
                 8 + roundToInt (getFont().getHeight()));

        setTopLeftPosition (component.getX(), component.getY() - getHeight());
    }
}

void FrameLabel::componentParentHierarchyChanged (Component& component)
{
    if (component.getParentComponent() != nullptr)
        component.getParentComponent()->addChildComponent (this);
}

void FrameLabel::componentVisibilityChanged (Component& component)
{
    setVisible (component.isVisible());
}

//==============================================================================
void FrameLabel::textWasEdited() {}
void FrameLabel::textWasChanged() {}
void FrameLabel::editorShown (TextEditor*) {}
void FrameLabel::editorAboutToBeHidden (TextEditor*) {}

void FrameLabel::showEditor()
{
    if (editor == nullptr)
    {
        addAndMakeVisible (editor = createEditorComponent());
        editor->setText (getText(), false);
        editor->addListener (this);
        editor->grabKeyboardFocus();
        editor->setHighlightedRegion (Range<int> (0, textValue.toString().length()));

        resized();
        repaint();

        editorShown (editor);

        enterModalState (false);
        editor->grabKeyboardFocus();
    }
}

bool FrameLabel::updateFromTextEditorContents (TextEditor& ed)
{
    const String newText (ed.getText());

    if (textValue.toString() != newText)
    {
        lastTextValue = newText;
        textValue = newText;
        repaint();

        textWasChanged();

        if (ownerComponent != nullptr)
            componentMovedOrResized (*ownerComponent, true, true);

        return true;
    }

    return false;
}

void FrameLabel::hideEditor (const bool discardCurrentEditorContents)
{
    if (editor != nullptr)
    {
        WeakReference<Component> deletionChecker (this);

        ScopedPointer<TextEditor> outgoingEditor (editor);

        editorAboutToBeHidden (outgoingEditor);

        const bool changed = (! discardCurrentEditorContents)
                               && updateFromTextEditorContents (*outgoingEditor);
        outgoingEditor = nullptr;
        repaint();

        if (changed)
            textWasEdited();

        if (deletionChecker != nullptr)
            exitModalState (0);

        if (changed && deletionChecker != nullptr)
            callChangeListeners();
    }
}

void FrameLabel::inputAttemptWhenModal()
{
    if (editor != nullptr)
    {
        if (lossOfFocusDiscardsChanges)
            textEditorEscapeKeyPressed (*editor);
        else
            textEditorReturnKeyPressed (*editor);
    }
}

bool FrameLabel::isBeingEdited() const noexcept
{
    return editor != nullptr;
}

TextEditor* FrameLabel::createEditorComponent()
{
    TextEditor* const ed = new TextEditor (getName());
    ed->setFont (font);

    // copy these colours from our own settings..
    const int cols[] = { TextEditor::backgroundColourId,
                         TextEditor::textColourId,
                         TextEditor::highlightColourId,
                         TextEditor::highlightedTextColourId,
                         TextEditor::outlineColourId,
                         TextEditor::focusedOutlineColourId,
                         TextEditor::shadowColourId,
                         CaretComponent::caretColourId };

    for (int i = 0; i < numElementsInArray (cols); ++i)
        ed->setColour (cols[i], findColour (cols[i]));

    return ed;
}

//==============================================================================
void FrameLabel::paint (Graphics& g)
{
    getLookAndFeel().drawFrameLabel (g, *this);
}

void FrameLabel::mouseUp (const MouseEvent& e)
{
    if (editSingleClick
         && e.mouseWasClicked()
         && contains (e.getPosition())
         && ! e.mods.isPopupMenu())
    {
        showEditor();
    }
}

void FrameLabel::mouseDoubleClick (const MouseEvent& e)
{
    if (editDoubleClick && ! e.mods.isPopupMenu())
        showEditor();
}

void FrameLabel::resized()
{
    if (editor != nullptr)
        editor->setBoundsInset (BorderSize<int> (0));
}

void FrameLabel::focusGained (FocusChangeType cause)
{
    if (editSingleClick && cause == focusChangedByTabKey)
        showEditor();
}

void FrameLabel::enablementChanged()
{
    repaint();
}

void FrameLabel::colourChanged()
{
    repaint();
}

void FrameLabel::setMinimumHorizontalScale (const float newScale)
{
    if (minimumHorizontalScale != newScale)
    {
        minimumHorizontalScale = newScale;
        repaint();
    }
}

//==============================================================================
// We'll use a custom focus traverser here to make sure focus goes from the
// text editor to another component rather than back to the label itself.
class FrameLabelKeyboardFocusTraverser   : public KeyboardFocusTraverser
{
public:
    FrameLabelKeyboardFocusTraverser() {}

    Component* getNextComponent (Component* current)
    {
        return KeyboardFocusTraverser::getNextComponent (dynamic_cast <TextEditor*> (current) != nullptr
                                                            ? current->getParentComponent() : current);
    }

    Component* getPreviousComponent (Component* current)
    {
        return KeyboardFocusTraverser::getPreviousComponent (dynamic_cast <TextEditor*> (current) != nullptr
                                                                ? current->getParentComponent() : current);
    }
};

KeyboardFocusTraverser* FrameLabel::createFocusTraverser()
{
    return new FrameLabelKeyboardFocusTraverser();
}

//==============================================================================
void FrameLabel::addListener (FrameLabelListener* const listener)
{
    listeners.add (listener);
}

void FrameLabel::removeListener (FrameLabelListener* const listener)
{
    listeners.remove (listener);
}

void FrameLabel::callChangeListeners()
{
    Component::BailOutChecker checker (this);
    listeners.callChecked (checker, &FrameLabelListener::labelTextChanged, this);  // (can't use FrameLabel::Listener due to idiotic VC2005 bug)
}

//==============================================================================
void FrameLabel::textEditorTextChanged (TextEditor& ed)
{
    if (editor != nullptr)
    {
        jassert (&ed == editor);

        if (! (hasKeyboardFocus (true) || isCurrentlyBlockedByAnotherModalComponent()))
        {
            if (lossOfFocusDiscardsChanges)
                textEditorEscapeKeyPressed (ed);
            else
                textEditorReturnKeyPressed (ed);
        }
    }
}

void FrameLabel::textEditorReturnKeyPressed (TextEditor& ed)
{
    if (editor != nullptr)
    {
        jassert (&ed == editor);

        const bool changed = updateFromTextEditorContents (ed);
        hideEditor (true);

        if (changed)
        {
            WeakReference<Component> deletionChecker (this);
            textWasEdited();

            if (deletionChecker != nullptr)
                callChangeListeners();
        }
    }
}

void FrameLabel::textEditorEscapeKeyPressed (TextEditor& ed)
{
    if (editor != nullptr)
    {
        jassert (&ed == editor);
        (void) ed;

        editor->setText (textValue.toString(), false);
        hideEditor (true);
    }
}

void FrameLabel::textEditorFocusLost (TextEditor& ed)
{
    textEditorTextChanged (ed);
}


END_JUCE_NAMESPACE
