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

#if defined (__JUCE_EVENTS_JUCEHEADER__) && ! defined (JUCE_AMALGAMATED_INCLUDE)
 /* When you add this cpp file to your project, you mustn't include it in a file where you've
    already included any other headers - just put it inside a file on its own, possibly with your config
    flags preceding it, but don't include anything else. That also includes avoiding any automatic prefix
    header files that the compiler may be using.
 */
 #error "Incorrect use of JUCE cpp file"
#endif

// Your project must contain an AppConfig.h file with your project-specific settings in it,
// and your header search path must make it accessible to the module's files.
#include "AppConfig.h"

#include "../juce_core/native/juce_BasicNativeHeaders.h"
#include "juce_events.h"

//==============================================================================
#if JUCE_MAC
 #import <IOKit/IOKitLib.h>
 #import <IOKit/IOCFPlugIn.h>
 #import <IOKit/hid/IOHIDLib.h>
 #import <IOKit/hid/IOHIDKeys.h>
 #import <IOKit/pwr_mgt/IOPMLib.h>

#elif JUCE_LINUX
 #include <X11/Xlib.h>
 #include <X11/Xresource.h>
 #include <X11/Xutil.h>
 #undef KeyPress
#endif

//==============================================================================
// START_AUTOINCLUDE messages/*.cpp, broadcasters/*.cpp,
// timers/*.cpp, interprocess/*.cpp
#include "messages/juce_ApplicationBase.cpp"
#include "messages/juce_DeletedAtShutdown.cpp"
#include "messages/juce_MessageListener.cpp"
#include "messages/juce_MessageManager.cpp"
#include "broadcasters/juce_ActionBroadcaster.cpp"
#include "broadcasters/juce_AsyncUpdater.cpp"
#include "broadcasters/juce_ChangeBroadcaster.cpp"
#include "timers/juce_MultiTimer.cpp"
#include "timers/juce_Timer.cpp"
#include "interprocess/juce_InterprocessConnection.cpp"
#include "interprocess/juce_InterprocessConnectionServer.cpp"
// END_AUTOINCLUDE

//==============================================================================
using namespace juce;

BEGIN_JUCE_NAMESPACE

#if JUCE_MAC
 #include "../juce_core/native/juce_osx_ObjCHelpers.h"
 #include "../juce_core/native/juce_mac_ObjCSuffix.h"
 #include "native/juce_osx_MessageQueue.h"
 #include "native/juce_mac_MessageManager.mm"

#elif JUCE_IOS
 #include "../juce_core/native/juce_osx_ObjCHelpers.h"
 #include "native/juce_osx_MessageQueue.h"
 #include "native/juce_ios_MessageManager.mm"

#elif JUCE_WINDOWS
 #include "native/juce_win32_HiddenMessageWindow.h"
 #include "native/juce_win32_Messaging.cpp"

#elif JUCE_LINUX
 #include "native/juce_ScopedXLock.h"
 #include "native/juce_linux_Messaging.cpp"

#elif JUCE_ANDROID
 #include "../juce_core/native/juce_android_JNIHelpers.h"
 #include "native/juce_android_Messaging.cpp"

#endif

END_JUCE_NAMESPACE
