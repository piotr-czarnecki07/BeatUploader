/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================

BeatUploaderAudioProcessorEditor::BeatUploaderAudioProcessorEditor (BeatUploaderAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    // Initialize custom fonts
    auto titleFontData = BinaryData::MichromaRegular_ttf;
    auto titleFontSize = BinaryData::MichromaRegular_ttfSize;
    titleFont = juce::Font(juce::Typeface::createSystemTypefaceFor(titleFontData, titleFontSize));

    auto componentFontData = BinaryData::LexendRegular_ttf;
    auto componentFontSize = BinaryData::LexendRegular_ttfSize;
    componentFont = juce::Font(juce::Typeface::createSystemTypefaceFor(componentFontData, componentFontSize));

    // Song title entrybox
    /*beatTitle.setTextToShowWhenEmpty("Song title...", );
    addAndMakeVisible(beatTitle);*/

    // Song description entrybox
    //beatDesc.setTextToShowWhenEmpty("Enter song description...", textEditorEmptyFg);
    //beatDesc.setMultiLine(true);
    //beatDesc.setReturnKeyStartsNewLine(true);
    //beatDesc.setScrollbarsShown(true);
    //addAndMakeVisible(beatDesc);

    //// Audio chooser button
    //audioSel.setButtonText("Choose audio");
    //addAndMakeVisible(audioSel);
    //audioSel.onClick = [this] { audioClicked(); };

    //// Image chooser button
    //imageSel.setButtonText("Choose image");
    //addAndMakeVisible(imageSel);
    //imageSel.onClick = [this] { imageClicked(); };

    //// Email entrybox
    //emailInput.setTextToShowWhenEmpty("Enter email address associated with YouTube channel", textEditorEmptyFg);
    //addAndMakeVisible(emailInput);

    //// Upload button
    //uploadBtn.setButtonText("Upload");
    //addAndMakeVisible(uploadBtn);

    // Output info label
    //addAndMakeVisible(operationOutput);

    setSize (screenWidth, screenHeight);
}

void BeatUploaderAudioProcessorEditor::audioClicked()
{
    audioChooser = std::make_unique<juce::FileChooser>(
        "Choose audio file",
        juce::File::getSpecialLocation(juce::File::userDocumentsDirectory),
        "*.wav;*.mp3;*.flac;*.ogg"
    );

    auto chooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;

    audioChooser->launchAsync(chooserFlags, [this](const juce::FileChooser& fc){
        auto file = fc.getResult();

        if (file.existsAsFile())
            file.loadFileAsData(audioFileData);
    });
}

void BeatUploaderAudioProcessorEditor::imageClicked()
{
    imageChooser = std::make_unique<juce::FileChooser>(
        "Choose image/gif file",
        juce::File::getSpecialLocation(juce::File::userDocumentsDirectory),
        "*.png;*.jpg;*.gif"
    );

    auto chooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;

    imageChooser->launchAsync(chooserFlags, [this](const juce::FileChooser& fc) {
        auto file = fc.getResult();

        if (file.existsAsFile())
            file.loadFileAsData(imageFileData);
    });
}

BeatUploaderAudioProcessorEditor::~BeatUploaderAudioProcessorEditor()
{
}

void BeatUploaderAudioProcessorEditor::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(pluginBg);

    g.setColour(pluginFg);
    g.setFont(titleFont);
    g.setFont(47.5f);
    g.drawText("BeatUploader", sideMargin, sideMargin, screenWidth - (2 * sideMargin), 85, juce::Justification::centredTop);
}

void BeatUploaderAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
