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
    songTitle.setFont(componentFont.withHeight(18.0f));
    songTitle.setTextToShowWhenEmpty("Song title...", textEditorEmptyFg);
    songTitle.setColour(juce::TextEditor::backgroundColourId, textEditorBg);
    songTitle.setColour(juce::TextEditor::textColourId, textEditorContentFg);
    songTitle.setColour(juce::TextEditor::highlightColourId, textEditorHighlight);
    songTitle.setColour(juce::TextEditor::outlineColourId, textEditorBg);
    songTitle.setColour(juce::TextEditor::focusedOutlineColourId, textEditorHighlight);
    addAndMakeVisible(songTitle);

    // Song description entrybox
    songDesc.setFont(componentFont.withHeight(18.0f));
    songDesc.setTextToShowWhenEmpty("Song description...", textEditorEmptyFg);
    songDesc.setColour(juce::TextEditor::backgroundColourId, textEditorBg);
    songDesc.setColour(juce::TextEditor::textColourId, textEditorContentFg);
    songDesc.setColour(juce::TextEditor::highlightColourId, textEditorHighlight);
    songDesc.setColour(juce::TextEditor::outlineColourId, textEditorBg);
    songDesc.setColour(juce::TextEditor::focusedOutlineColourId, textEditorHighlight);
    songDesc.setMultiLine(true);
    songDesc.setReturnKeyStartsNewLine(true);
    songDesc.setScrollbarsShown(true);
    addAndMakeVisible(songDesc);

    // Audio chooser
    //audioSel.setColour(juce::TextButton::buttonColourId);
    //audioSel.setColour(juce::TextButton::buttonOnColourId);
    //audioSel.setColour(juce::TextButton::textColourOnId);
    //audioSel.setColour(juce::TextButton::textColourOffId);
    /*audioSel.setCustom
    audioSel.setButtonText("Choose audio");
    addAndMakeVisible(audioSel);
    audioSel.onClick = [this] { audioClicked(); };*/

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

    // Draw plugin title
    g.setColour(pluginFg);
    g.setFont(titleFont);
    g.setFont(47.5f);
    g.drawText("BeatUploader", sideMargin, 20, screenWidth - (2 * sideMargin), 50, juce::Justification::centredTop);
}

void BeatUploaderAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    int yPostion = 20 + (2 * sideMargin) + 55; // position on y axis where to draw the next element

    songTitle.setBounds(sideMargin, yPostion, screenWidth - (2 * sideMargin), 27);
    yPostion += (27 + elementMargin);

    songDesc.setBounds(sideMargin, yPostion, screenWidth - (2 * sideMargin), 130);
    yPostion += (130 + elementMargin);
}
