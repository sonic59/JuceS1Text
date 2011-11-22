/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  1 Oct 2011 12:03:25pm

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.12

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "WindowComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
WindowComponent::WindowComponent ()
    : counter (0),
      tbOne (0),
      tbTwo (0),
      tbThree (0),
      tbFour (0),
      tbFive (0),
      lblte11 (0),
      lblTE1Short (0),
      lblte21 (0),
      lblTE2Short (0),
      lblte12 (0),
      lblte22 (0),
      lblTE1Long (0),
      lblTE2Long (0),
      lblte13 (0),
      lblTE1Multi (0),
      lblTE2Multi (0),
      lblte23 (0),
      tbSix (0),
      tbSeven (0),
      tbEight (0),
      tbNine (0),
      tbTen (0)
{
    addAndMakeVisible (tbOne = new TextButton (L"one"));
    tbOne->addListener (this);

    addAndMakeVisible (tbTwo = new TextButton (L"two"));
    tbTwo->addListener (this);

    addAndMakeVisible (tbThree = new TextButton (L"three"));
    tbThree->addListener (this);

    addAndMakeVisible (tbFour = new TextButton (L"four"));
    tbFour->addListener (this);

    addAndMakeVisible (tbFive = new TextButton (L"five"));
    tbFive->addListener (this);

    addAndMakeVisible (lblte11 = new Label (L"te1 short",
                                            L"Text Engine 1:"));
    lblte11->setFont (Font (15.0000f, Font::plain));
    lblte11->setJustificationType (Justification::centredLeft);
    lblte11->setEditable (false, false, false);
    lblte11->setColour (TextEditor::textColourId, Colours::black);
    lblte11->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (lblTE1Short = new Label (L"te1 short label",
                                                L"short label"));
    lblTE1Short->setFont (Font (15.0000f, Font::plain));
    lblTE1Short->setJustificationType (Justification::centredLeft);
    lblTE1Short->setEditable (false, false, false);
    lblTE1Short->setColour (Label::outlineColourId, Colours::black);
    lblTE1Short->setColour (TextEditor::textColourId, Colours::black);
    lblTE1Short->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (lblte21 = new Label (L"te2 short",
                                            L"Text Engine 2:"));
    lblte21->setFont (Font (15.0000f, Font::plain));
    lblte21->setJustificationType (Justification::centredLeft);
    lblte21->setEditable (false, false, false);
    lblte21->setColour (TextEditor::textColourId, Colours::black);
    lblte21->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (lblTE2Short = new LayoutLabel (L"te2 short label",
                                                L"short label"));
    lblTE2Short->setFont (Font (15.0000f, Font::plain));
    lblTE2Short->setJustificationType (Justification::centredLeft);
    lblTE2Short->setEditable (false, false, false);
    lblTE2Short->setColour (Label::outlineColourId, Colours::black);
    lblTE2Short->setColour (TextEditor::textColourId, Colours::black);
    lblTE2Short->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (lblte12 = new Label (L"te1 long",
                                            L"Text Engine 1:"));
    lblte12->setFont (Font (15.0000f, Font::plain));
    lblte12->setJustificationType (Justification::centredLeft);
    lblte12->setEditable (false, false, false);
    lblte12->setColour (TextEditor::textColourId, Colours::black);
    lblte12->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (lblte22 = new Label (L"te2 long",
                                            L"Text Engine 2:"));
    lblte22->setFont (Font (15.0000f, Font::plain));
    lblte22->setJustificationType (Justification::centredLeft);
    lblte22->setEditable (false, false, false);
    lblte22->setColour (TextEditor::textColourId, Colours::black);
    lblte22->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (lblTE1Long = new Label (L"te1 long label",
                                               L"This is a one line label that is really long and can contain a lot of text."));
    lblTE1Long->setFont (Font (15.0000f, Font::plain));
    lblTE1Long->setJustificationType (Justification::centredLeft);
    lblTE1Long->setEditable (false, false, false);
    lblTE1Long->setColour (Label::outlineColourId, Colours::black);
    lblTE1Long->setColour (TextEditor::textColourId, Colours::black);
    lblTE1Long->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (lblTE2Long = new LayoutLabel (L"te2 long label",
                                               L"This is a one line label that is really long and can contain a lot of text."));
    lblTE2Long->setFont (Font (15.0000f, Font::plain));
    lblTE2Long->setJustificationType (Justification::centredLeft);
    lblTE2Long->setEditable (false, false, false);
    lblTE2Long->setColour (Label::outlineColourId, Colours::black);
    lblTE2Long->setColour (TextEditor::textColourId, Colours::black);
    lblTE2Long->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (lblte13 = new Label (L"te1 multi",
                                            L"Text Engine 1:"));
    lblte13->setFont (Font (15.0000f, Font::plain));
    lblte13->setJustificationType (Justification::centredLeft);
    lblte13->setEditable (false, false, false);
    lblte13->setColour (TextEditor::textColourId, Colours::black);
    lblte13->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (lblTE1Multi = new Label (L"te1 multi",
                                                L"Multi Line Text"));
    lblTE1Multi->setFont (Font (15.0000f, Font::plain));
    lblTE1Multi->setJustificationType (Justification::topLeft);
    lblTE1Multi->setEditable (false, false, false);
    lblTE1Multi->setColour (Label::outlineColourId, Colours::black);
    lblTE1Multi->setColour (TextEditor::textColourId, Colours::black);
    lblTE1Multi->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (lblTE2Multi = new LayoutLabel (L"te2 multi",
                                                L"Multi Line Text"));
    lblTE2Multi->setFont (Font (15.0000f, Font::plain));
    lblTE2Multi->setJustificationType (Justification::centredLeft);
    lblTE2Multi->setEditable (false, false, false);
    lblTE2Multi->setColour (Label::outlineColourId, Colours::black);
    lblTE2Multi->setColour (TextEditor::textColourId, Colours::black);
    lblTE2Multi->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (lblte23 = new Label (L"te2 multi",
                                            L"Text Engine 2:"));
    lblte23->setFont (Font (15.0000f, Font::plain));
    lblte23->setJustificationType (Justification::centredLeft);
    lblte23->setEditable (false, false, false);
    lblte23->setColour (TextEditor::textColourId, Colours::black);
    lblte23->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (tbSix = new TextButton (L"six"));
    tbSix->addListener (this);

    addAndMakeVisible (tbSeven = new TextButton (L"seven"));
    tbSeven->addListener (this);

    addAndMakeVisible (tbEight = new TextButton (L"eight"));
    tbEight->addListener (this);

    addAndMakeVisible (tbNine = new TextButton (L"nine"));
    tbNine->addListener (this);

    addAndMakeVisible (tbTen = new TextButton (L"ten"));
    tbTen->addListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (840, 530);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

WindowComponent::~WindowComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    deleteAndZero (tbOne);
    deleteAndZero (tbTwo);
    deleteAndZero (tbThree);
    deleteAndZero (tbFour);
    deleteAndZero (tbFive);
    deleteAndZero (lblte11);
    deleteAndZero (lblTE1Short);
    deleteAndZero (lblte21);
    deleteAndZero (lblTE2Short);
    deleteAndZero (lblte12);
    deleteAndZero (lblte22);
    deleteAndZero (lblTE1Long);
    deleteAndZero (lblTE2Long);
    deleteAndZero (lblte13);
    deleteAndZero (lblTE1Multi);
    deleteAndZero (lblTE2Multi);
    deleteAndZero (lblte23);
    deleteAndZero (tbSix);
    deleteAndZero (tbSeven);
    deleteAndZero (tbEight);
    deleteAndZero (tbNine);
    deleteAndZero (tbTen);


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void WindowComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::white);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void WindowComponent::resized()
{
    tbOne->setBounds (8, 8, 72, 24);
    tbTwo->setBounds (88, 8, 72, 24);
    tbThree->setBounds (168, 8, 72, 24);
    tbFour->setBounds (248, 8, 72, 24);
    tbFive->setBounds (328, 8, 72, 24);
    lblte11->setBounds (8, 40, 104, 24);
    lblTE1Short->setBounds (112, 40, 150, 24);
    lblte21->setBounds (8, 72, 104, 24);
    lblTE2Short->setBounds (112, 72, 150, 24);
    lblte12->setBounds (8, 104, 104, 24);
    lblte22->setBounds (8, 136, 104, 24);
    lblTE1Long->setBounds (112, 104, 720, 24);
    lblTE2Long->setBounds (112, 136, 720, 24);
    lblte13->setBounds (8, 168, 104, 24);
    lblTE1Multi->setBounds (112, 168, 720, 160);
    lblTE2Multi->setBounds (112, 344, 720, 160);
    lblte23->setBounds (8, 344, 104, 24);
    tbSix->setBounds (408, 8, 72, 24);
    tbSeven->setBounds (488, 8, 72, 24);
    tbEight->setBounds (568, 8, 72, 24);
    tbNine->setBounds (648, 8, 72, 24);
    tbTen->setBounds (728, 8, 72, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void WindowComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == tbOne)
    {
        //[UserButtonCode_tbOne] -- add your button handler code here..
        readXMLLayout("one.xml", lblTE1Short, lblTE2Short);
        //[/UserButtonCode_tbOne]
    }
    else if (buttonThatWasClicked == tbTwo)
    {
        //[UserButtonCode_tbTwo] -- add your button handler code here..
        readXMLLayout("two.xml", lblTE1Long, lblTE2Long);
        //[/UserButtonCode_tbTwo]
    }
    else if (buttonThatWasClicked == tbThree)
    {
        //[UserButtonCode_tbThree] -- add your button handler code here..
        //readXMLLayout("three.xml", lblTE1Multi, lblTE2Multi);
        counter = 0;
        //[/UserButtonCode_tbThree]
    }
    else if (buttonThatWasClicked == tbFour)
    {
        //[UserButtonCode_tbFour] -- add your button handler code here..
        readXMLFrame("four.xml", lblTE1Multi, lblTE2Multi);
        //[/UserButtonCode_tbFour]
    }
    else if (buttonThatWasClicked == tbFive)
    {
        //[UserButtonCode_tbFive] -- add your button handler code here..
        //[/UserButtonCode_tbFive]
    }
    else if (buttonThatWasClicked == tbSix)
    {
        //[UserButtonCode_tbSix] -- add your button handler code here..
        //[/UserButtonCode_tbSix]
    }
    else if (buttonThatWasClicked == tbSeven)
    {
        //[UserButtonCode_tbSeven] -- add your button handler code here..
        //[/UserButtonCode_tbSeven]
    }
    else if (buttonThatWasClicked == tbEight)
    {
        //[UserButtonCode_tbEight] -- add your button handler code here..
        //[/UserButtonCode_tbEight]
    }
    else if (buttonThatWasClicked == tbNine)
    {
        //[UserButtonCode_tbNine] -- add your button handler code here..
        //[/UserButtonCode_tbNine]
    }
    else if (buttonThatWasClicked == tbTen)
    {
        //[UserButtonCode_tbTen] -- add your button handler code here..
        //[/UserButtonCode_tbTen]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void WindowComponent::readXMLLayout(String xmlFile, Label* labelOne, LayoutLabel* labelTwo)
{
    String xmlPath = File::getSpecialLocation(File::userHomeDirectory).getFullPathName();
    xmlPath += "/Projects/JuceText/SampleText/";
    XmlDocument myDocument (File (xmlPath + xmlFile));
    ScopedPointer<XmlElement> xml (myDocument.getDocumentElement());
    if (xml != nullptr && xml->hasTagName ("textarray"))
    {
        int subCounter = 0;
        forEachXmlChildElement (*xml, e)
        {
            if (subCounter != counter)
            {
                subCounter++;
                continue;
            }
            if (e->hasTagName ("text"))
            {
                ScopedPointer<AttributedString> as1;
                as1 = new AttributedString(e->getAllSubText());
                // Test Colored Text
                //as1->setForegroundColour(3, 5, Colours::blue);
                // Test Single Font
                Font lucidiaGrande("Lucidia Grande", 13.0f, 0);
                Range<int> textRange(0, e->getAllSubText().length());
                as1->setFont(textRange, lucidiaGrande);
                // Test Multiple Fonts
                /*Font lucidiaGrande("Lucidia Grande", 13.0f, 0);
                as1->setFont(0, 5, lucidiaGrande);
                Font times("Times", 13.0f, 0);
                as1->setFont(6, e->getAllSubText().length(), times);*/
                // Test Paragraph Alignment
                //as1->setTextAlignment(AttributedString::center);
                labelOne->setText (e->getAllSubText(), false);
                labelTwo->setAttributedText (as1, false);
                counter++;
                break;
            }
        }
        if (subCounter == counter) counter = 0;
    }
}

void WindowComponent::readXMLFrame(String xmlFile, Label* labelOne, LayoutLabel* labelTwo)
{
    String xmlPath = File::getSpecialLocation(File::userHomeDirectory).getFullPathName();
    xmlPath += "/Projects/JulesText/SampleText/";
    XmlDocument myDocument (File (xmlPath + xmlFile));
    ScopedPointer<XmlElement> xml (myDocument.getDocumentElement());
    if (xml != nullptr && xml->hasTagName ("textarray"))
    {
        int subCounter = 0;
        forEachXmlChildElement (*xml, e)
        {
            if (counter % 2 == 0 && subCounter != counter)
            {
                subCounter++;
                continue;
            }
            if (e->hasTagName ("text"))
            {
                ScopedPointer<AttributedString> as1;
                as1 = new AttributedString(e->getAllSubText());
                Range<int> textRange1(100, 200);
                as1->setColour(textRange1, Colours::blue);
                //Font times("Times", 14.0f, 0);
                Font times("Times New Roman", 14.0f, 0);
                Range<int> textRange2(0, 301);
                as1->setFont(textRange2, times);
                //Font lucidiaGrande("Lucidia Grande", 15.0f, 0);
                Font lucidiaGrande("Verdana", 15.0f, 0);
                Range<int> textRange3(301, e->getAllSubText().length());
                as1->setFont(textRange3, lucidiaGrande);
                if ((counter > 13) && (counter < 19)) as1->setReadingDirection(AttributedString::rightToLeft);
                labelOne->setText (e->getAllSubText(), false);
                labelTwo->setAttributedText (as1, false);
                counter += 2;
                break;
            }
        }
        if (subCounter == counter)
        {
            counter = 0;
        }
    }
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Jucer information section --

    This is where the Jucer puts all of its metadata, so don't change anything in here!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="WindowComponent" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330000013"
                 fixedSize="0" initialWidth="840" initialHeight="530">
  <BACKGROUND backgroundColour="ffffffff"/>
  <TEXTBUTTON name="one" id="edbff00d0699edaf" memberName="tbOne" virtualName=""
              explicitFocusOrder="0" pos="8 8 72 24" buttonText="one" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="two" id="a9481d4510d1d9ab" memberName="tbTwo" virtualName=""
              explicitFocusOrder="0" pos="88 8 72 24" buttonText="two" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="three" id="f80bc769c44fb1aa" memberName="tbThree" virtualName=""
              explicitFocusOrder="0" pos="168 8 72 24" buttonText="three" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="four" id="137edb7d6f9ddd92" memberName="tbFour" virtualName=""
              explicitFocusOrder="0" pos="248 8 72 24" buttonText="four" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="five" id="60627e5022272254" memberName="tbFive" virtualName=""
              explicitFocusOrder="0" pos="328 8 72 24" buttonText="five" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <LABEL name="te1 short" id="9cfbf5f07e46404f" memberName="lblte11" virtualName=""
         explicitFocusOrder="0" pos="8 40 104 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Text Engine 1:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <LABEL name="te1 short label" id="5f4c36602d4693f6" memberName="lblTE1Short"
         virtualName="" explicitFocusOrder="0" pos="112 40 150 24" outlineCol="ff000000"
         edTextCol="ff000000" edBkgCol="0" labelText="short label" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <LABEL name="te2 short" id="8e0ef20f88f09558" memberName="lblte21" virtualName=""
         explicitFocusOrder="0" pos="8 72 104 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Text Engine 2:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <LABEL name="te2 short label" id="2c83ea6c9e8f8dfe" memberName="lblTE2Short"
         virtualName="" explicitFocusOrder="0" pos="112 72 150 24" outlineCol="ff000000"
         edTextCol="ff000000" edBkgCol="0" labelText="short label" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <LABEL name="te1 long" id="2a9314dfbfef9811" memberName="lblte12" virtualName=""
         explicitFocusOrder="0" pos="8 104 104 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Text Engine 1:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <LABEL name="te2 long" id="b47248fff22975be" memberName="lblte22" virtualName=""
         explicitFocusOrder="0" pos="8 136 104 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Text Engine 2:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <LABEL name="te1 long label" id="75887ab2af181bed" memberName="lblTE1Long"
         virtualName="" explicitFocusOrder="0" pos="112 104 720 24" outlineCol="ff000000"
         edTextCol="ff000000" edBkgCol="0" labelText="This is a one line label that is really long and can contain a lot of text."
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" bold="0" italic="0" justification="33"/>
  <LABEL name="te2 long label" id="18ef2d388fdb554e" memberName="lblTE2Long"
         virtualName="" explicitFocusOrder="0" pos="112 136 720 24" outlineCol="ff000000"
         edTextCol="ff000000" edBkgCol="0" labelText="This is a one line label that is really long and can contain a lot of text."
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" bold="0" italic="0" justification="33"/>
  <LABEL name="te1 multi" id="9e88ae5135f4ab61" memberName="lblte13" virtualName=""
         explicitFocusOrder="0" pos="8 168 104 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Text Engine 1:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <LABEL name="te1 multi" id="f0f838cc4d6f1780" memberName="lblTE1Multi"
         virtualName="" explicitFocusOrder="0" pos="112 168 720 160" outlineCol="ff000000"
         edTextCol="ff000000" edBkgCol="0" labelText="Lorem ipsum dolor sit amet, consectetur adipiscing elit. Mauris a libero in purus ultrices tristique sed eu libero. Nulla id nisl a libero commodo tincidunt id vitae nulla. Quisque neque diam, ultricies ac adipiscing ut, ultricies non magna. Pellentesque molestie, dui vel semper interdum, nulla arcu tincidunt dolor, id consequat libero urna id nulla. Etiam cursus neque eget dolor convallis placerat. Morbi commodo ultricies purus, vel consequat mauris mollis a. Donec luctus turpis sed magna gravida sed facilisis orci auctor. Duis vulputate nulla sit amet mauris blandit fringilla. Mauris sodales massa ut justo vehicula sagittis. Quisque sed arcu nisl, et dapibus felis. Nullam ultrices sodales magna quis malesuada. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Suspendisse non risus a tortor viverra vulputate. Proin nibh libero, varius ornare commodo consequat, cursus id mi. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Morbi sodales vehicula tortor, in euismod nulla luctus eget."
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" bold="0" italic="0" justification="33"/>
  <LABEL name="te2 multi" id="ad612c5db4e24022" memberName="lblTE2Multi"
         virtualName="" explicitFocusOrder="0" pos="112 344 720 160" outlineCol="ff000000"
         edTextCol="ff000000" edBkgCol="0" labelText="Lorem ipsum dolor sit amet, consectetur adipiscing elit. Mauris a libero in purus ultrices tristique sed eu libero. Nulla id nisl a libero commodo tincidunt id vitae nulla. Quisque neque diam, ultricies ac adipiscing ut, ultricies non magna. Pellentesque molestie, dui vel semper interdum, nulla arcu tincidunt dolor, id consequat libero urna id nulla. Etiam cursus neque eget dolor convallis placerat. Morbi commodo ultricies purus, vel consequat mauris mollis a. Donec luctus turpis sed magna gravida sed facilisis orci auctor. Duis vulputate nulla sit amet mauris blandit fringilla. Mauris sodales massa ut justo vehicula sagittis. Quisque sed arcu nisl, et dapibus felis. Nullam ultrices sodales magna quis malesuada. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Suspendisse non risus a tortor viverra vulputate. Proin nibh libero, varius ornare commodo consequat, cursus id mi. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Morbi sodales vehicula tortor, in euismod nulla luctus eget."
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" bold="0" italic="0" justification="33"/>
  <LABEL name="te2 multi" id="81416eb37d15c65d" memberName="lblte23" virtualName=""
         explicitFocusOrder="0" pos="8 344 104 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Text Engine 2:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="six" id="6b6849ca2b4d67ac" memberName="tbSix" virtualName=""
              explicitFocusOrder="0" pos="408 8 72 24" buttonText="six" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="seven" id="eb4c5b7d4a8ef7f2" memberName="tbSeven" virtualName=""
              explicitFocusOrder="0" pos="488 8 72 24" buttonText="seven" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="eight" id="3186a5eddbf77be" memberName="tbEight" virtualName=""
              explicitFocusOrder="0" pos="568 8 72 24" buttonText="eight" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="nine" id="3df033ca2e84cad0" memberName="tbNine" virtualName=""
              explicitFocusOrder="0" pos="648 8 72 24" buttonText="nine" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="ten" id="5a6b873707add2dc" memberName="tbTen" virtualName=""
              explicitFocusOrder="0" pos="728 8 72 24" buttonText="ten" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif
