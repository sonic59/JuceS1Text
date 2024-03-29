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

#if (JUCE_MAC && defined (MAC_OS_X_VERSION_10_5) && MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_5 \
        && MAC_OS_X_VERSION_MAX_ALLOWED > MAC_OS_X_VERSION_10_5) \
     || (JUCE_IOS && defined (__IPHONE_3_0) && __IPHONE_OS_VERSION_MIN_REQUIRED >= __IPHONE_3_2)
 #define JUCE_CORETEXT_AVAILABLE 1
#endif

#if JUCE_CORETEXT_AVAILABLE

namespace CoreTextTypeLayout
{
    CFAttributedStringRef createCFAttributedString (const AttributedString& text)
    {
       #if JUCE_IOS
        CGColorSpaceRef rgbColourSpace = CGColorSpaceCreateDeviceRGB();
       #endif

        CFStringRef cfText = text.getText().toCFString();
        CFMutableAttributedStringRef attribString = CFAttributedStringCreateMutable (kCFAllocatorDefault, 0);
        CFAttributedStringReplaceString (attribString, CFRangeMake(0, 0), cfText);
        CFRelease (cfText);

        const int numCharacterAttributes = text.getNumAttributes();

        for (int i = 0; i < numCharacterAttributes; ++i)
        {
            const AttributedString::Attribute* const attr = text.getAttribute (i);

            if (attr->range.getStart() > CFAttributedStringGetLength (attribString))
                continue;

            Range<int> range (attr->range);
            range.setEnd (jmin (range.getEnd(), (int) CFAttributedStringGetLength (attribString)));

            if (attr->getFont() != nullptr)
            {
                // Apply fontHeightToCGSizeFactor to the font size since this is how glyphs are drawn
                CTFontRef ctFontRef = CTFontCreateWithName (attr->getFont()->getTypefaceName().toCFString(), 1024, nullptr);
                CGFontRef cgFontRef = CTFontCopyGraphicsFont (ctFontRef, nullptr);
                CFRelease (ctFontRef);

                const int totalHeight = abs (CGFontGetAscent (cgFontRef)) + abs (CGFontGetDescent (cgFontRef));
                float fontHeightToCGSizeFactor = CGFontGetUnitsPerEm (cgFontRef) / (float) totalHeight;
                CGFontRelease (cgFontRef);

                ctFontRef = CTFontCreateWithName (attr->getFont()->getTypefaceName().toCFString(),
                                                  attr->getFont()->getHeight() * fontHeightToCGSizeFactor, nullptr);

                CFAttributedStringSetAttribute (attribString, CFRangeMake (range.getStart(), range.getLength()),
                                                kCTFontAttributeName, ctFontRef);
                CFRelease (ctFontRef);
            }

            if (attr->getColour() != nullptr)
            {
               #if JUCE_IOS
                const CGFloat components[] = { attr->getColour()->getFloatRed(),
                                               attr->getColour()->getFloatGreen(),
                                               attr->getColour()->getFloatBlue(),
                                               attr->getColour()->getFloatAlpha() };
                CGColorRef colour = CGColorCreate (rgbColourSpace, components);
               #else
                CGColorRef colour = CGColorCreateGenericRGB (attr->getColour()->getFloatRed(),
                                                             attr->getColour()->getFloatGreen(),
                                                             attr->getColour()->getFloatBlue(),
                                                             attr->getColour()->getFloatAlpha());
               #endif

                CFAttributedStringSetAttribute (attribString,
                                                CFRangeMake (range.getStart(), range.getLength()),
                                                kCTForegroundColorAttributeName, colour);
                CGColorRelease (colour);
            }
        }

        // Paragraph Attributes
        CTTextAlignment ctTextAlignment = kCTLeftTextAlignment;
        CTLineBreakMode ctLineBreakMode = kCTLineBreakByWordWrapping;
        const CGFloat ctLineSpacing = text.getLineSpacing();

        switch (text.getJustification().getOnlyHorizontalFlags())
        {
            case Justification::left:                   break;
            case Justification::right:                  ctTextAlignment = kCTRightTextAlignment; break;
            case Justification::horizontallyCentred:    ctTextAlignment = kCTCenterTextAlignment; break;
            case Justification::horizontallyJustified:  ctTextAlignment = kCTJustifiedTextAlignment; break;
            default:                                    jassertfalse; break; // Illegal justification flags
        }

        switch (text.getWordWrap())
        {
            case AttributedString::byWord:      break;
            case AttributedString::none:        ctLineBreakMode = kCTLineBreakByClipping; break;
            case AttributedString::byChar:      ctLineBreakMode = kCTLineBreakByCharWrapping; break;
            default: break;
        }

        CTParagraphStyleSetting settings[] =
        {
            { kCTParagraphStyleSpecifierAlignment,     sizeof (CTTextAlignment), &ctTextAlignment },
            { kCTParagraphStyleSpecifierLineBreakMode, sizeof (CTLineBreakMode), &ctLineBreakMode },
            { kCTParagraphStyleSpecifierLineSpacing,   sizeof (CGFloat),         &ctLineSpacing }
        };

        CTParagraphStyleRef ctParagraphStyleRef = CTParagraphStyleCreate (settings, numElementsInArray (settings));
        CFAttributedStringSetAttribute (attribString, CFRangeMake (0, CFAttributedStringGetLength (attribString)),
                                        kCTParagraphStyleAttributeName, ctParagraphStyleRef);
        CFRelease (ctParagraphStyleRef);
       #if JUCE_IOS
        CGColorSpaceRelease (rgbColourSpace);
       #endif
        return attribString;
    }

    float getTextHeight (CTFrameRef frame, const CGRect& bounds)
    {
        CFArrayRef lines = CTFrameGetLines (frame);
        CFIndex numLines = CFArrayGetCount (lines);
        CFIndex lastLineIndex = numLines - 1;
        CGFloat descent;
        CTLineRef line = (CTLineRef) CFArrayGetValueAtIndex (lines, lastLineIndex);
        CTLineGetTypographicBounds (line, nullptr,  &descent, nullptr);
        CGPoint lastLineOrigin;
        CTFrameGetLineOrigins (frame, CFRangeMake (lastLineIndex, 1), &lastLineOrigin);
        return bounds.size.height - lastLineOrigin.y + descent;
    }

    void drawToCGContext (const AttributedString& text, const Rectangle<float>& area,
                          const CGContextRef& context, const float flipHeight)
    {
        CFAttributedStringRef attribString = CoreTextTypeLayout::createCFAttributedString (text);
        CTFramesetterRef framesetter = CTFramesetterCreateWithAttributedString (attribString);
        CFRelease (attribString);

        CGMutablePathRef path = CGPathCreateMutable();
        CGRect bounds = CGRectMake ((CGFloat) area.getX(), flipHeight - (CGFloat) area.getBottom(),
                                    (CGFloat) area.getWidth(), (CGFloat) area.getHeight());
        CGPathAddRect (path, nullptr, bounds);

        CTFrameRef frame = CTFramesetterCreateFrame (framesetter, CFRangeMake (0, 0), path, NULL);
        CFRelease (framesetter);
        CGPathRelease (path);

        float dy = 0;
        if (text.getJustification().getOnlyVerticalFlags() == Justification::bottom)
            dy = bounds.size.height - getTextHeight (frame, bounds);
        else if (text.getJustification().getOnlyVerticalFlags() == Justification::verticallyCentred)
            dy = (bounds.size.height - getTextHeight (frame, bounds)) / 2.0f;

        if (dy != 0)
        {
            CGContextSaveGState (context);
            CGContextTranslateCTM (context, 0, -dy);
        }

        CTFrameDraw (frame, context);

        if (dy != 0)
            CGContextRestoreGState (context);

        CFRelease (frame);
    }

    void createLayout (GlyphLayout& glyphLayout, const AttributedString& text)
    {
        CFAttributedStringRef attribString = CoreTextTypeLayout::createCFAttributedString (text);
        CTFramesetterRef framesetter = CTFramesetterCreateWithAttributedString (attribString);
        CFRelease (attribString);

        CGMutablePathRef path = CGPathCreateMutable();
        const CGRect bounds = CGRectMake (0, 0, glyphLayout.getWidth(), 1.0e6f);
        CGPathAddRect (path, nullptr, bounds);

        CTFrameRef frame = CTFramesetterCreateFrame (framesetter, CFRangeMake(0, 0), path, nullptr);
        CFRelease (framesetter);
        CGPathRelease (path);

        CFArrayRef lines = CTFrameGetLines (frame);
        const CFIndex numLines = CFArrayGetCount (lines);

        glyphLayout.ensureStorageAllocated (numLines);

        for (CFIndex i = 0; i < numLines; ++i)
        {
            CTLineRef line = (CTLineRef) CFArrayGetValueAtIndex (lines, i);

            CFArrayRef runs = CTLineGetGlyphRuns (line);
            const CFIndex numRuns = CFArrayGetCount (runs);

            const CFRange cfrlineStringRange = CTLineGetStringRange (line);
            const CFIndex lineStringEnd = cfrlineStringRange.location + cfrlineStringRange.length - 1;
            const Range<int> lineStringRange ((int) cfrlineStringRange.location, (int) lineStringEnd);

            CGPoint cgpLineOrigin;
            CTFrameGetLineOrigins (frame, CFRangeMake(i, 1), &cgpLineOrigin);

            Point<float> lineOrigin ((float) cgpLineOrigin.x, bounds.size.height - (float) cgpLineOrigin.y);

            CGFloat ascent, descent, leading;
            CTLineGetTypographicBounds (line, &ascent,  &descent, &leading);

            GlyphLayout::Line* const glyphLine = new GlyphLayout::Line (lineStringRange, lineOrigin,
                                                                        (float) ascent, (float) descent, (float) leading,
                                                                        (int) numRuns);
            glyphLayout.addLine (glyphLine);

            for (CFIndex j = 0; j < numRuns; ++j)
            {
                CTRunRef run = (CTRunRef) CFArrayGetValueAtIndex (runs, j);
                const CFIndex numGlyphs = CTRunGetGlyphCount (run);
                const CFRange runStringRange = CTRunGetStringRange (run);

                GlyphLayout::Run* const glyphRun = new GlyphLayout::Run (Range<int> ((int) runStringRange.location,
                                                                                     (int) (runStringRange.location + runStringRange.length - 1)),
                                                                         (int) numGlyphs);
                glyphLine->addRun (glyphRun);

                CFDictionaryRef runAttributes = CTRunGetAttributes (run);

                CTFontRef ctRunFont;
                if (CFDictionaryGetValueIfPresent (runAttributes, kCTFontAttributeName, (const void **) &ctRunFont))
                {
                    CFStringRef cfsFontName = CTFontCopyPostScriptName (ctRunFont);
                    const String fontName (String::fromCFString (cfsFontName));

                    CTFontRef ctFontRef = CTFontCreateWithName (cfsFontName, 1024, nullptr);
                    CFRelease (cfsFontName);
                    CGFontRef cgFontRef = CTFontCopyGraphicsFont (ctFontRef, nullptr);
                    CFRelease (ctFontRef);

                    const int totalHeight = std::abs (CGFontGetAscent (cgFontRef)) + std::abs (CGFontGetDescent (cgFontRef));
                    const float fontHeightToCGSizeFactor = CGFontGetUnitsPerEm (cgFontRef) / (float) totalHeight;
                    CGFontRelease (cgFontRef);

                    glyphRun->setFont (Font (fontName, CTFontGetSize (ctRunFont) / fontHeightToCGSizeFactor, 0));
                }

                CGColorRef cgRunColor;
                if (CFDictionaryGetValueIfPresent (runAttributes, kCTForegroundColorAttributeName, (const void**) &cgRunColor)
                     && CGColorGetNumberOfComponents (cgRunColor) == 4)
                {
                    const CGFloat* const components = CGColorGetComponents (cgRunColor);

                    glyphRun->setColour (Colour::fromFloatRGBA (components[0], components[1], components[2], components[3]));
                }

                const CGGlyph* glyphsPtr = CTRunGetGlyphsPtr (run);
                const CGPoint* posPtr = CTRunGetPositionsPtr (run);
                HeapBlock <CGGlyph> glyphBuffer;
                HeapBlock <CGPoint> positionBuffer;

                if (glyphsPtr == nullptr || posPtr == nullptr)
                {
                    // If we can't get a direct pointer, we have to copy the metrics to get them..
                    glyphBuffer.malloc (numGlyphs);
                    glyphsPtr = glyphBuffer;
                    CTRunGetGlyphs (run, CFRangeMake (0, 0), glyphBuffer);

                    positionBuffer.malloc (numGlyphs);
                    posPtr = positionBuffer;
                    CTRunGetPositions (run, CFRangeMake (0, 0), positionBuffer);
                }

                for (CFIndex k = 0; k < numGlyphs; ++k)
                    glyphRun->addGlyph (new GlyphLayout::Glyph (glyphsPtr[k], Point<float> (posPtr[k].x, posPtr[k].y)));
            }
        }

        CFRelease (frame);
    }
}


//==============================================================================
class OSXTypeface  : public Typeface
{
public:
    OSXTypeface (const Font& font)
        : Typeface (font.getTypefaceName()),
          fontRef (nullptr),
          fontHeightToCGSizeFactor (1.0f),
          renderingTransform (CGAffineTransformIdentity),
          ctFontRef (nullptr),
          attributedStringAtts (nullptr),
          ascent (0.0f),
          unitsToHeightScaleFactor (0.0f)
    {
        CFStringRef cfName = font.getTypefaceName().toCFString();
        ctFontRef = CTFontCreateWithName (cfName, 1024, nullptr);
        CFRelease (cfName);

        if (ctFontRef != nullptr)
        {
            bool needsItalicTransform = false;

            if (font.isItalic())
            {
                CTFontRef newFont = CTFontCreateCopyWithSymbolicTraits (ctFontRef, 0.0f, nullptr,
                                                                        kCTFontItalicTrait, kCTFontItalicTrait);

                if (newFont != nullptr)
                {
                    CFRelease (ctFontRef);
                    ctFontRef = newFont;
                }
                else
                {
                    needsItalicTransform = true; // couldn't find a proper italic version, so fake it with a transform..
                }
            }

            if (font.isBold())
            {
                CTFontRef newFont = CTFontCreateCopyWithSymbolicTraits (ctFontRef, 0.0f, nullptr,
                                                                        kCTFontBoldTrait, kCTFontBoldTrait);
                if (newFont != nullptr)
                {
                    CFRelease (ctFontRef);
                    ctFontRef = newFont;
                }
            }

            ascent = std::abs ((float) CTFontGetAscent (ctFontRef));
            const float totalSize = ascent + std::abs ((float) CTFontGetDescent (ctFontRef));
            ascent /= totalSize;

            pathTransform = AffineTransform::identity.scale (1.0f / totalSize, 1.0f / totalSize);

            if (needsItalicTransform)
            {
                pathTransform = pathTransform.sheared (-0.15f, 0.0f);
                renderingTransform.c = 0.15f;
            }

            fontRef = CTFontCopyGraphicsFont (ctFontRef, nullptr);

            const int totalHeight = abs (CGFontGetAscent (fontRef)) + abs (CGFontGetDescent (fontRef));
            const float ctTotalHeight = abs (CTFontGetAscent (ctFontRef)) + abs (CTFontGetDescent (ctFontRef));
            unitsToHeightScaleFactor = 1.0f / ctTotalHeight;
            fontHeightToCGSizeFactor = CGFontGetUnitsPerEm (fontRef) / (float) totalHeight;

            const short zero = 0;
            CFNumberRef numberRef = CFNumberCreate (0, kCFNumberShortType, &zero);

            CFStringRef keys[] = { kCTFontAttributeName, kCTLigatureAttributeName };
            CFTypeRef values[] = { ctFontRef, numberRef };
            attributedStringAtts = CFDictionaryCreate (nullptr, (const void**) &keys, (const void**) &values, numElementsInArray (keys),
                                                       &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
            CFRelease (numberRef);
        }
    }

    ~OSXTypeface()
    {
        if (attributedStringAtts != nullptr)
            CFRelease (attributedStringAtts);

        if (fontRef != nullptr)
            CGFontRelease (fontRef);

        if (ctFontRef != nullptr)
            CFRelease (ctFontRef);
    }

    float getAscent() const     { return ascent; }
    float getDescent() const    { return 1.0f - ascent; }

    float getStringWidth (const String& text)
    {
        float x = 0;

        if (ctFontRef != nullptr && text.isNotEmpty())
        {
            CFStringRef cfText = text.toCFString();
            CFAttributedStringRef attribString = CFAttributedStringCreate (kCFAllocatorDefault, cfText, attributedStringAtts);
            CFRelease (cfText);

            CTLineRef line = CTLineCreateWithAttributedString (attribString);
            CFArrayRef runArray = CTLineGetGlyphRuns (line);

            for (CFIndex i = 0; i < CFArrayGetCount (runArray); ++i)
            {
                CTRunRef run = (CTRunRef) CFArrayGetValueAtIndex (runArray, i);
                CFIndex length = CTRunGetGlyphCount (run);
                HeapBlock <CGSize> advances (length);
                CTRunGetAdvances (run, CFRangeMake (0, 0), advances);

                for (int j = 0; j < length; ++j)
                    x += (float) advances[j].width;
            }

            CFRelease (line);
            CFRelease (attribString);

            x *= unitsToHeightScaleFactor;
        }

        return x;
    }

    void getGlyphPositions (const String& text, Array <int>& resultGlyphs, Array <float>& xOffsets)
    {
        xOffsets.add (0);

        if (ctFontRef != nullptr && text.isNotEmpty())
        {
            float x = 0;

            CFStringRef cfText = text.toCFString();
            CFAttributedStringRef attribString = CFAttributedStringCreate (kCFAllocatorDefault, cfText, attributedStringAtts);
            CFRelease (cfText);

            CTLineRef line = CTLineCreateWithAttributedString (attribString);
            CFArrayRef runArray = CTLineGetGlyphRuns (line);

            for (CFIndex i = 0; i < CFArrayGetCount (runArray); ++i)
            {
                CTRunRef run = (CTRunRef) CFArrayGetValueAtIndex (runArray, i);
                CFIndex length = CTRunGetGlyphCount (run);
                HeapBlock <CGSize> advances (length);
                CTRunGetAdvances (run, CFRangeMake (0, 0), advances);
                HeapBlock <CGGlyph> glyphs (length);
                CTRunGetGlyphs (run, CFRangeMake (0, 0), glyphs);

                for (int j = 0; j < length; ++j)
                {
                    x += (float) advances[j].width;
                    xOffsets.add (x * unitsToHeightScaleFactor);
                    resultGlyphs.add (glyphs[j]);
                }
            }

            CFRelease (line);
            CFRelease (attribString);
        }
    }

    EdgeTable* getEdgeTableForGlyph (int glyphNumber, const AffineTransform& transform)
    {
        Path path;

        if (getOutlineForGlyph (glyphNumber, path) && ! path.isEmpty())
            return new EdgeTable (path.getBoundsTransformed (transform).getSmallestIntegerContainer().expanded (1, 0),
                                  path, transform);

        return nullptr;
    }

    bool getOutlineForGlyph (int glyphNumber, Path& path)
    {
        jassert (path.isEmpty());  // we might need to apply a transform to the path, so this must be empty

        CGPathRef pathRef = CTFontCreatePathForGlyph (ctFontRef, (CGGlyph) glyphNumber, &renderingTransform);
        if (pathRef == 0)
            return false;

        CGPathApply (pathRef, &path, pathApplier);
        CFRelease (pathRef);

        if (! pathTransform.isIdentity())
            path.applyTransform (pathTransform);

        return true;
    }

    //==============================================================================
    CGFontRef fontRef;

    float fontHeightToCGSizeFactor;
    CGAffineTransform renderingTransform;

private:
    CTFontRef ctFontRef;
    CFDictionaryRef attributedStringAtts;
    float ascent, unitsToHeightScaleFactor;
    AffineTransform pathTransform;

    static void pathApplier (void* info, const CGPathElement* const element)
    {
        Path& path = *static_cast<Path*> (info);
        const CGPoint* const p = element->points;

        switch (element->type)
        {
            case kCGPathElementMoveToPoint:         path.startNewSubPath ((float) p[0].x, (float) -p[0].y); break;
            case kCGPathElementAddLineToPoint:      path.lineTo          ((float) p[0].x, (float) -p[0].y); break;
            case kCGPathElementAddQuadCurveToPoint: path.quadraticTo     ((float) p[0].x, (float) -p[0].y,
                                                                          (float) p[1].x, (float) -p[1].y); break;
            case kCGPathElementAddCurveToPoint:     path.cubicTo         ((float) p[0].x, (float) -p[0].y,
                                                                          (float) p[1].x, (float) -p[1].y,
                                                                          (float) p[2].x, (float) -p[2].y); break;
            case kCGPathElementCloseSubpath:        path.closeSubPath(); break;
            default:                                jassertfalse; break;
        }
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OSXTypeface);
};

#else

//==============================================================================
// The stuff that follows is a mash-up that supports pre-OSX 10.5 and pre-iOS 3.2 APIs.
// (Hopefully all of this can be ditched at some point in the future).

//==============================================================================
#if MAC_OS_X_VERSION_MIN_REQUIRED < MAC_OS_X_VERSION_10_5
  #define SUPPORT_10_4_FONTS 1
  #define NEW_CGFONT_FUNCTIONS_UNAVAILABLE (CGFontCreateWithFontName == 0)

  #if MAC_OS_X_VERSION_MAX_ALLOWED < MAC_OS_X_VERSION_10_5
    #define SUPPORT_ONLY_10_4_FONTS 1
  #endif

  END_JUCE_NAMESPACE
  @interface NSFont (PrivateHack)
    - (NSGlyph) _defaultGlyphForChar: (unichar) theChar;
  @end
  BEGIN_JUCE_NAMESPACE
#endif

//==============================================================================
class OSXTypeface  : public Typeface
{
public:
    OSXTypeface (const Font& font)
        : Typeface (font.getTypefaceName())
    {
        JUCE_AUTORELEASEPOOL
        renderingTransform = CGAffineTransformIdentity;

        bool needsItalicTransform = false;

#if JUCE_IOS
        NSString* fontName = juceStringToNS (font.getTypefaceName());

        if (font.isItalic() || font.isBold())
        {
            NSArray* familyFonts = [UIFont fontNamesForFamilyName: juceStringToNS (font.getTypefaceName())];

            for (NSString* i in familyFonts)
            {
                const String fn (nsStringToJuce (i));
                const String afterDash (fn.fromFirstOccurrenceOf ("-", false, false));

                const bool probablyBold = afterDash.containsIgnoreCase ("bold") || fn.endsWithIgnoreCase ("bold");
                const bool probablyItalic = afterDash.containsIgnoreCase ("oblique")
                                             || afterDash.containsIgnoreCase ("italic")
                                             || fn.endsWithIgnoreCase ("oblique")
                                             || fn.endsWithIgnoreCase ("italic");

                if (probablyBold == font.isBold()
                     && probablyItalic == font.isItalic())
                {
                    fontName = i;
                    needsItalicTransform = false;
                    break;
                }
                else if (probablyBold && (! probablyItalic) && probablyBold == font.isBold())
                {
                    fontName = i;
                    needsItalicTransform = true; // not ideal, so carry on in case we find a better one
                }
            }

            if (needsItalicTransform)
                renderingTransform.c = 0.15f;
        }

        fontRef = CGFontCreateWithFontName ((CFStringRef) fontName);

        if (fontRef == 0)
        {
            // Sometimes, UIFont manages to handle names that CGFontCreateWithFontName fails on...
            UIFont* uiFont = [UIFont fontWithName: fontName size: 12];
            fontRef = CGFontCreateWithFontName ((CFStringRef) uiFont.fontName);
        }

        const int ascender = abs (CGFontGetAscent (fontRef));
        const float totalHeight = ascender + abs (CGFontGetDescent (fontRef));
        ascent = ascender / totalHeight;
        unitsToHeightScaleFactor = 1.0f / totalHeight;
        fontHeightToCGSizeFactor = CGFontGetUnitsPerEm (fontRef) / totalHeight;
#else
        nsFont = [NSFont fontWithName: juceStringToNS (font.getTypefaceName()) size: 1024];

        if (font.isItalic())
        {
            NSFont* newFont = [[NSFontManager sharedFontManager] convertFont: nsFont
                                                                 toHaveTrait: NSItalicFontMask];

            if (newFont == nsFont)
                needsItalicTransform = true; // couldn't find a proper italic version, so fake it with a transform..

            nsFont = newFont;
        }

        if (font.isBold())
            nsFont = [[NSFontManager sharedFontManager] convertFont: nsFont toHaveTrait: NSBoldFontMask];

        [nsFont retain];

        ascent = std::abs ((float) [nsFont ascender]);
        float totalSize = ascent + std::abs ((float) [nsFont descender]);
        ascent /= totalSize;

        pathTransform = AffineTransform::identity.scale (1.0f / totalSize, 1.0f / totalSize);

        if (needsItalicTransform)
        {
            pathTransform = pathTransform.sheared (-0.15f, 0.0f);
            renderingTransform.c = 0.15f;
        }

      #if SUPPORT_ONLY_10_4_FONTS
        ATSFontRef atsFont = ATSFontFindFromName ((CFStringRef) [nsFont fontName], kATSOptionFlagsDefault);

        if (atsFont == 0)
            atsFont = ATSFontFindFromPostScriptName ((CFStringRef) [nsFont fontName], kATSOptionFlagsDefault);

        fontRef = CGFontCreateWithPlatformFont (&atsFont);

        const float totalHeight = std::abs ([nsFont ascender]) + std::abs ([nsFont descender]);
        unitsToHeightScaleFactor = 1.0f / totalHeight;
        fontHeightToCGSizeFactor = 1024.0f / totalHeight;
      #else
       #if SUPPORT_10_4_FONTS
        if (NEW_CGFONT_FUNCTIONS_UNAVAILABLE)
        {
            ATSFontRef atsFont = ATSFontFindFromName ((CFStringRef) [nsFont fontName], kATSOptionFlagsDefault);

            if (atsFont == 0)
                atsFont = ATSFontFindFromPostScriptName ((CFStringRef) [nsFont fontName], kATSOptionFlagsDefault);

            fontRef = CGFontCreateWithPlatformFont (&atsFont);

            const float totalHeight = std::abs ([nsFont ascender]) + std::abs ([nsFont descender]);
            unitsToHeightScaleFactor = 1.0f / totalHeight;
            fontHeightToCGSizeFactor = 1024.0f / totalHeight;
        }
        else
       #endif
        {
            fontRef = CGFontCreateWithFontName ((CFStringRef) [nsFont fontName]);

            const int totalHeight = abs (CGFontGetAscent (fontRef)) + abs (CGFontGetDescent (fontRef));
            unitsToHeightScaleFactor = 1.0f / totalHeight;
            fontHeightToCGSizeFactor = CGFontGetUnitsPerEm (fontRef) / (float) totalHeight;
        }
      #endif

#endif
    }

    ~OSXTypeface()
    {
       #if ! JUCE_IOS
        [nsFont release];
       #endif

        if (fontRef != 0)
            CGFontRelease (fontRef);
    }

    float getAscent() const    { return ascent; }
    float getDescent() const   { return 1.0f - ascent; }

    float getStringWidth (const String& text)
    {
        if (fontRef == 0 || text.isEmpty())
            return 0;

        const int length = text.length();
        HeapBlock <CGGlyph> glyphs;
        createGlyphsForString (text.getCharPointer(), length, glyphs);

        float x = 0;

#if SUPPORT_ONLY_10_4_FONTS
        HeapBlock <NSSize> advances (length);
        [nsFont getAdvancements: advances forGlyphs: reinterpret_cast <NSGlyph*> (glyphs.getData()) count: length];

        for (int i = 0; i < length; ++i)
            x += advances[i].width;
#else
       #if SUPPORT_10_4_FONTS
        if (NEW_CGFONT_FUNCTIONS_UNAVAILABLE)
        {
            HeapBlock <NSSize> advances (length);
            [nsFont getAdvancements: advances forGlyphs: reinterpret_cast<NSGlyph*> (glyphs.getData()) count: length];

            for (int i = 0; i < length; ++i)
                x += advances[i].width;
        }
        else
       #endif
        {
            HeapBlock <int> advances (length);

            if (CGFontGetGlyphAdvances (fontRef, glyphs, length, advances))
                for (int i = 0; i < length; ++i)
                    x += advances[i];
        }
#endif

        return x * unitsToHeightScaleFactor;
    }

    void getGlyphPositions (const String& text, Array <int>& resultGlyphs, Array <float>& xOffsets)
    {
        xOffsets.add (0);

        if (fontRef == 0 || text.isEmpty())
            return;

        const int length = text.length();
        HeapBlock <CGGlyph> glyphs;
        createGlyphsForString (text.getCharPointer(), length, glyphs);

#if SUPPORT_ONLY_10_4_FONTS
        HeapBlock <NSSize> advances (length);
        [nsFont getAdvancements: advances forGlyphs: reinterpret_cast <NSGlyph*> (glyphs.getData()) count: length];

        int x = 0;
        for (int i = 0; i < length; ++i)
        {
            x += advances[i].width;
            xOffsets.add (x * unitsToHeightScaleFactor);
            resultGlyphs.add (reinterpret_cast <NSGlyph*> (glyphs.getData())[i]);
        }

#else
       #if SUPPORT_10_4_FONTS
        if (NEW_CGFONT_FUNCTIONS_UNAVAILABLE)
        {
            HeapBlock <NSSize> advances (length);
            NSGlyph* const nsGlyphs = reinterpret_cast<NSGlyph*> (glyphs.getData());
            [nsFont getAdvancements: advances forGlyphs: nsGlyphs count: length];

            float x = 0;
            for (int i = 0; i < length; ++i)
            {
                x += advances[i].width;
                xOffsets.add (x * unitsToHeightScaleFactor);
                resultGlyphs.add (nsGlyphs[i]);
            }
        }
        else
       #endif
        {
            HeapBlock <int> advances (length);

            if (CGFontGetGlyphAdvances (fontRef, glyphs, length, advances))
            {
                int x = 0;
                for (int i = 0; i < length; ++i)
                {
                    x += advances [i];
                    xOffsets.add (x * unitsToHeightScaleFactor);
                    resultGlyphs.add (glyphs[i]);
                }
            }
        }
#endif
    }

    EdgeTable* getEdgeTableForGlyph (int glyphNumber, const AffineTransform& transform)
    {
        Path path;

        if (getOutlineForGlyph (glyphNumber, path) && ! path.isEmpty())
            return new EdgeTable (path.getBoundsTransformed (transform).getSmallestIntegerContainer().expanded (1, 0),
                                  path, transform);

        return nullptr;
    }

    bool getOutlineForGlyph (int glyphNumber, Path& path)
    {
       #if JUCE_IOS
        return false;
       #else
        if (nsFont == nil)
            return false;

        // we might need to apply a transform to the path, so it mustn't have anything else in it
        jassert (path.isEmpty());

        JUCE_AUTORELEASEPOOL

        NSBezierPath* bez = [NSBezierPath bezierPath];
        [bez moveToPoint: NSMakePoint (0, 0)];
        [bez appendBezierPathWithGlyph: (NSGlyph) glyphNumber
                                inFont: nsFont];

        for (int i = 0; i < [bez elementCount]; ++i)
        {
            NSPoint p[3];
            switch ([bez elementAtIndex: i associatedPoints: p])
            {
                case NSMoveToBezierPathElement:     path.startNewSubPath ((float) p[0].x, (float) -p[0].y); break;
                case NSLineToBezierPathElement:     path.lineTo  ((float) p[0].x, (float) -p[0].y); break;
                case NSCurveToBezierPathElement:    path.cubicTo ((float) p[0].x, (float) -p[0].y,
                                                                  (float) p[1].x, (float) -p[1].y,
                                                                  (float) p[2].x, (float) -p[2].y); break;
                case NSClosePathBezierPathElement:  path.closeSubPath(); break;
                default:                            jassertfalse; break;
            }
        }

        path.applyTransform (pathTransform);
        return true;
       #endif
    }

    //==============================================================================
    CGFontRef fontRef;
    float fontHeightToCGSizeFactor;
    CGAffineTransform renderingTransform;

private:
    float ascent, unitsToHeightScaleFactor;

   #if ! JUCE_IOS
    NSFont* nsFont;
    AffineTransform pathTransform;
   #endif

    void createGlyphsForString (String::CharPointerType text, const int length, HeapBlock <CGGlyph>& glyphs)
    {
      #if SUPPORT_10_4_FONTS
       #if ! SUPPORT_ONLY_10_4_FONTS
        if (NEW_CGFONT_FUNCTIONS_UNAVAILABLE)
       #endif
        {
            glyphs.malloc (sizeof (NSGlyph) * length, 1);
            NSGlyph* const nsGlyphs = reinterpret_cast<NSGlyph*> (glyphs.getData());

            for (int i = 0; i < length; ++i)
                nsGlyphs[i] = (NSGlyph) [nsFont _defaultGlyphForChar: text.getAndAdvance()];

            return;
        }
      #endif

       #if ! SUPPORT_ONLY_10_4_FONTS
        if (charToGlyphMapper == nullptr)
            charToGlyphMapper = new CharToGlyphMapper (fontRef);

        glyphs.malloc (length);

        for (int i = 0; i < length; ++i)
            glyphs[i] = (CGGlyph) charToGlyphMapper->getGlyphForCharacter (text.getAndAdvance());
       #endif
    }

#if ! SUPPORT_ONLY_10_4_FONTS
    // Reads a CGFontRef's character map table to convert unicode into glyph numbers
    class CharToGlyphMapper
    {
    public:
        CharToGlyphMapper (CGFontRef fontRef)
            : segCount (0), endCode (0), startCode (0), idDelta (0),
              idRangeOffset (0), glyphIndexes (0)
        {
            CFDataRef cmapTable = CGFontCopyTableForTag (fontRef, 'cmap');

            if (cmapTable != 0)
            {
                const int numSubtables = getValue16 (cmapTable, 2);

                for (int i = 0; i < numSubtables; ++i)
                {
                    if (getValue16 (cmapTable, i * 8 + 4) == 0) // check for platform ID of 0
                    {
                        const int offset = getValue32 (cmapTable, i * 8 + 8);

                        if (getValue16 (cmapTable, offset) == 4) // check that it's format 4..
                        {
                            const int length = getValue16 (cmapTable, offset + 2);
                            const int segCountX2 =  getValue16 (cmapTable, offset + 6);
                            segCount = segCountX2 / 2;
                            const int endCodeOffset = offset + 14;
                            const int startCodeOffset = endCodeOffset + 2 + segCountX2;
                            const int idDeltaOffset = startCodeOffset + segCountX2;
                            const int idRangeOffsetOffset = idDeltaOffset + segCountX2;
                            const int glyphIndexesOffset = idRangeOffsetOffset + segCountX2;

                            endCode       = CFDataCreate (kCFAllocatorDefault, CFDataGetBytePtr (cmapTable) + endCodeOffset, segCountX2);
                            startCode     = CFDataCreate (kCFAllocatorDefault, CFDataGetBytePtr (cmapTable) + startCodeOffset, segCountX2);
                            idDelta       = CFDataCreate (kCFAllocatorDefault, CFDataGetBytePtr (cmapTable) + idDeltaOffset, segCountX2);
                            idRangeOffset = CFDataCreate (kCFAllocatorDefault, CFDataGetBytePtr (cmapTable) + idRangeOffsetOffset, segCountX2);
                            glyphIndexes  = CFDataCreate (kCFAllocatorDefault, CFDataGetBytePtr (cmapTable) + glyphIndexesOffset, offset + length - glyphIndexesOffset);
                        }

                        break;
                    }
                }

                CFRelease (cmapTable);
            }
        }

        ~CharToGlyphMapper()
        {
            if (endCode != 0)
            {
                CFRelease (endCode);
                CFRelease (startCode);
                CFRelease (idDelta);
                CFRelease (idRangeOffset);
                CFRelease (glyphIndexes);
            }
        }

        int getGlyphForCharacter (const juce_wchar c) const
        {
            for (int i = 0; i < segCount; ++i)
            {
                if (getValue16 (endCode, i * 2) >= c)
                {
                    const int start = getValue16 (startCode, i * 2);
                    if (start > c)
                        break;

                    const int delta = getValue16 (idDelta, i * 2);
                    const int rangeOffset = getValue16 (idRangeOffset, i * 2);

                    if (rangeOffset == 0)
                        return delta + c;
                    else
                        return getValue16 (glyphIndexes, 2 * ((rangeOffset / 2) + (c - start) - (segCount - i)));
                }
            }

            // If we failed to find it "properly", this dodgy fall-back seems to do the trick for most fonts!
            return jmax (-1, (int) c - 29);
        }

    private:
        int segCount;
        CFDataRef endCode, startCode, idDelta, idRangeOffset, glyphIndexes;

        static uint16 getValue16 (CFDataRef data, const int index)
        {
            return CFSwapInt16BigToHost (*(UInt16*) (CFDataGetBytePtr (data) + index));
        }

        static uint32 getValue32 (CFDataRef data, const int index)
        {
            return CFSwapInt32BigToHost (*(UInt32*) (CFDataGetBytePtr (data) + index));
        }
    };

    ScopedPointer <CharToGlyphMapper> charToGlyphMapper;
#endif

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OSXTypeface);
};

#endif

//==============================================================================
Typeface::Ptr Typeface::createSystemTypefaceFor (const Font& font)
{
    return new OSXTypeface (font);
}

StringArray Font::findAllTypefaceNames()
{
    StringArray names;

    JUCE_AUTORELEASEPOOL

   #if JUCE_IOS
    NSArray* fonts = [UIFont familyNames];
   #else
    NSArray* fonts = [[NSFontManager sharedFontManager] availableFontFamilies];
   #endif

    for (unsigned int i = 0; i < [fonts count]; ++i)
        names.add (nsStringToJuce ((NSString*) [fonts objectAtIndex: i]));

    names.sort (true);
    return names;
}

struct DefaultFontNames
{
    DefaultFontNames()
       #if JUCE_IOS
        : defaultSans  ("Helvetica"),
          defaultSerif ("Times New Roman"),
          defaultFixed ("Courier New"),
       #else
        : defaultSans  ("Lucida Grande"),
          defaultSerif ("Times New Roman"),
          defaultFixed ("Monaco"),
       #endif
          defaultFallback ("Arial Unicode MS")
    {
    }

    String defaultSans, defaultSerif, defaultFixed, defaultFallback;
};

Typeface::Ptr Font::getDefaultTypefaceForFont (const Font& font)
{
    static DefaultFontNames defaultNames;

    String faceName (font.getTypefaceName());

    if (faceName == Font::getDefaultSansSerifFontName())       faceName = defaultNames.defaultSans;
    else if (faceName == Font::getDefaultSerifFontName())      faceName = defaultNames.defaultSerif;
    else if (faceName == Font::getDefaultMonospacedFontName()) faceName = defaultNames.defaultFixed;

    Font f (font);
    f.setTypefaceName (faceName);
    return Typeface::createSystemTypefaceFor (f);
}

bool GlyphLayout::createNativeLayout (const AttributedString& text)
{
   #if JUCE_CORETEXT_AVAILABLE
    CoreTextTypeLayout::createLayout (*this, text);
    return true;
   #else
    (void) text;
    return false;
   #endif
}
