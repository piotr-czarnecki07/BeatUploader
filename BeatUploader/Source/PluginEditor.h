/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"


//==============================================================================

class BeatUploaderAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    BeatUploaderAudioProcessorEditor (BeatUploaderAudioProcessor&);
    ~BeatUploaderAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    BeatUploaderAudioProcessor& audioProcessor;

    // Constants
    int const screenWidth = 375;
    int const screenHeight = 550;
    int const sideMargin = 25;
    int const elementMargin = 10;

    // Custom fonts
    juce::Font titleFont;
    juce::Font componentFont;
    juce::Font outputFont;

    // Graifc components
    juce::TextEditor songTitle; // enter song title
    juce::TextEditor songDesc; // enter song description

    juce::TextButton videoSel; // video file dialog
    std::unique_ptr<juce::FileChooser> videoChooser;
    juce::MemoryBlock videoFileData;
    void videoClicked();

    struct buttonFont : public juce::LookAndFeel_V4 { // look and feel structure for textbuttons custom font
        buttonFont(juce::Font f) : fontToUse(f) {}

        juce::Font getTextButtonFont(juce::TextButton&, int buttonHeight) override
        {
            return fontToUse.withHeight(12.5f);
        }

        juce::Font fontToUse;
    };

    buttonFont buttonLookAndFeel{ componentFont };

    juce::TextButton uploadBtn;
    void uploadClicked();

    juce::Label operationOutput; // displays if operation was successful and if not, then what went wrong

    // Colors definitions
    juce::Colour const pluginBg = juce::Colour::fromString("ff1e1e1e");
    juce::Colour const pluginFg = juce::Colour::fromString("fdececec");

    juce::Colour const elementBg = juce::Colour::fromString("ff171717");
    
    juce::Colour const textEditorFg = juce::Colour::fromString("fde9e9e9");
    juce::Colour const textEditorEmptyFg = juce::Colour::fromString("ffb0b0b0");
    juce::Colour const textEditorHighlight = juce::Colour::fromString("b52874ed");

    juce::Colour const buttonFg = juce::Colour::fromString("fefafafa");

    juce::Colour const operationSuccessColor = juce::Colour::fromString("fd1de03e");
    juce::Colour const operationFailColor = juce::Colour::fromString("fde32619");

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BeatUploaderAudioProcessorEditor)
};
