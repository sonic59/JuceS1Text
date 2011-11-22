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

#ifndef __JUCER_HEADER_WINDOWCOMPONENT_WINDOWCOMPONENT_A5742CF__
#define __JUCER_HEADER_WINDOWCOMPONENT_WINDOWCOMPONENT_A5742CF__

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class WindowComponent  : public Component,
                         public ButtonListener
{
public:
    //==============================================================================
    WindowComponent ();
    ~WindowComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void readXMLLayout(String xmlFile, Label* labelOne, LayoutLabel* labelTwo);
    void readXMLFrame(String xmlFile, Label* labelOne, FrameLabel* labelTwo);
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);



    //==============================================================================
    juce_UseDebuggingNewOperator

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    int counter;
    //[/UserVariables]

    //==============================================================================
    TextButton* tbOne;
    TextButton* tbTwo;
    TextButton* tbThree;
    TextButton* tbFour;
    TextButton* tbFive;
    Label* lblte11;
    Label* lblTE1Short;
    Label* lblte21;
    LayoutLabel* lblTE2Short;
    Label* lblte12;
    Label* lblte22;
    Label* lblTE1Long;
    LayoutLabel* lblTE2Long;
    Label* lblte13;
    Label* lblTE1Multi;
    FrameLabel* lblTE2Multi;
    Label* lblte23;
    TextButton* tbSix;
    TextButton* tbSeven;
    TextButton* tbEight;
    TextButton* tbNine;
    TextButton* tbTen;


    //==============================================================================
    // (prevent copy constructor and operator= being generated..)
    WindowComponent (const WindowComponent&);
    const WindowComponent& operator= (const WindowComponent&);
};


#endif   // __JUCER_HEADER_WINDOWCOMPONENT_WINDOWCOMPONENT_A5742CF__
