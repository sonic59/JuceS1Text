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
LayoutLabel::LayoutLabel (const String& name,
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

    attributedTextValue = new AttributedString (String::empty);
}

LayoutLabel::~LayoutLabel()
{
    textValue.removeListener (this);

    if (ownerComponent != nullptr)
        ownerComponent->removeComponentListener (this);

    editor = nullptr;
}

//==============================================================================
void LayoutLabel::setText (const String& newText,
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

void LayoutLabel::setAttributedText (ScopedPointer<AttributedString> newText,
                           const bool broadcastChangeMessage)
{
    hideEditor (true);

    attributedTextValue = newText;
    repaint();

    textWasChanged();

    if (ownerComponent != nullptr)
        componentMovedOrResized (*ownerComponent, true, true);

    if (broadcastChangeMessage)
        callChangeListeners();
}

String LayoutLabel::getText (const bool returnActiveEditorContents) const
{
    return (returnActiveEditorContents && isBeingEdited())
                ? editor->getText()
                : textValue.toString();
}

AttributedString& LayoutLabel::getAttributedText (const bool) const
{
    return *attributedTextValue;
}

void LayoutLabel::valueChanged (Value&)
{
    if (lastTextValue != textValue.toString())
        setText (textValue.toString(), true);
}

//==============================================================================
void LayoutLabel::setFont (const Font& newFont)
{
    if (font != newFont)
    {
        font = newFont;
        repaint();
    }
}

const Font& LayoutLabel::getFont() const noexcept
{
    return font;
}

void LayoutLabel::setEditable (const bool editOnSingleClick,
                         const bool editOnDoubleClick,
                         const bool lossOfFocusDiscardsChanges_)
{
    editSingleClick = editOnSingleClick;
    editDoubleClick = editOnDoubleClick;
    lossOfFocusDiscardsChanges = lossOfFocusDiscardsChanges_;

    setWantsKeyboardFocus (editOnSingleClick || editOnDoubleClick);
    setFocusContainer (editOnSingleClick || editOnDoubleClick);
}

void LayoutLabel::setJustificationType (const Justification& newJustification)
{
    if (justification != newJustification)
    {
        justification = newJustification;
        repaint();
    }
}

void LayoutLabel::setBorderSize (int h, int v)
{
    if (horizontalBorderSize != h || verticalBorderSize != v)
    {
        horizontalBorderSize = h;
        verticalBorderSize = v;
        repaint();
    }
}

//==============================================================================
Component* LayoutLabel::getAttachedComponent() const
{
    return static_cast<Component*> (ownerComponent);
}

void LayoutLabel::attachToComponent (Component* owner, const bool onLeft)
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

void LayoutLabel::componentMovedOrResized (Component& component, bool /*wasMoved*/, bool /*wasResized*/)
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

void LayoutLabel::componentParentHierarchyChanged (Component& component)
{
    if (component.getParentComponent() != nullptr)
        component.getParentComponent()->addChildComponent (this);
}

void LayoutLabel::componentVisibilityChanged (Component& component)
{
    setVisible (component.isVisible());
}

//==============================================================================
void LayoutLabel::textWasEdited() {}
void LayoutLabel::textWasChanged() {}
void LayoutLabel::editorShown (TextEditor*) {}
void LayoutLabel::editorAboutToBeHidden (TextEditor*) {}

void LayoutLabel::showEditor()
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

bool LayoutLabel::updateFromTextEditorContents (TextEditor& ed)
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

void LayoutLabel::hideEditor (const bool discardCurrentEditorContents)
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

void LayoutLabel::inputAttemptWhenModal()
{
    if (editor != nullptr)
    {
        if (lossOfFocusDiscardsChanges)
            textEditorEscapeKeyPressed (*editor);
        else
            textEditorReturnKeyPressed (*editor);
    }
}

bool LayoutLabel::isBeingEdited() const noexcept
{
    return editor != nullptr;
}

TextEditor* LayoutLabel::createEditorComponent()
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
void LayoutLabel::paint (Graphics& g)
{
    getLookAndFeel().drawLayoutLabel (g, *this);
}

void LayoutLabel::mouseUp (const MouseEvent& e)
{
    if (editSingleClick
         && e.mouseWasClicked()
         && contains (e.getPosition())
         && ! e.mods.isPopupMenu())
    {
        showEditor();
    }
}

void LayoutLabel::mouseDoubleClick (const MouseEvent& e)
{
    if (editDoubleClick && ! e.mods.isPopupMenu())
        showEditor();
}

void LayoutLabel::resized()
{
    if (editor != nullptr)
        editor->setBoundsInset (BorderSize<int> (0));
}

void LayoutLabel::focusGained (FocusChangeType cause)
{
    if (editSingleClick && cause == focusChangedByTabKey)
        showEditor();
}

void LayoutLabel::enablementChanged()
{
    repaint();
}

void LayoutLabel::colourChanged()
{
    repaint();
}

void LayoutLabel::setMinimumHorizontalScale (const float newScale)
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
class LayoutLabelKeyboardFocusTraverser   : public KeyboardFocusTraverser
{
public:
    LayoutLabelKeyboardFocusTraverser() {}

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

KeyboardFocusTraverser* LayoutLabel::createFocusTraverser()
{
    return new LayoutLabelKeyboardFocusTraverser();
}

//==============================================================================
void LayoutLabel::addListener (LayoutLabelListener* const listener)
{
    listeners.add (listener);
}

void LayoutLabel::removeListener (LayoutLabelListener* const listener)
{
    listeners.remove (listener);
}

void LayoutLabel::callChangeListeners()
{
    Component::BailOutChecker checker (this);
    listeners.callChecked (checker, &LayoutLabelListener::labelTextChanged, this);  // (can't use LayoutLabel::Listener due to idiotic VC2005 bug)
}

//==============================================================================
void LayoutLabel::textEditorTextChanged (TextEditor& ed)
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

void LayoutLabel::textEditorReturnKeyPressed (TextEditor& ed)
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

void LayoutLabel::textEditorEscapeKeyPressed (TextEditor& ed)
{
    if (editor != nullptr)
    {
        jassert (&ed == editor);
        (void) ed;

        editor->setText (textValue.toString(), false);
        hideEditor (true);
    }
}

void LayoutLabel::textEditorFocusLost (TextEditor& ed)
{
    textEditorTextChanged (ed);
}


END_JUCE_NAMESPACE
