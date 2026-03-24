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

    auto outputFontData = BinaryData::KantumruyProMediumItalic_ttf;
    auto outputFontSize = BinaryData::KantumruyProMediumItalic_ttfSize;
    outputFont = juce::Font(juce::Typeface::createSystemTypefaceFor(outputFontData, outputFontSize));

    // Song title entrybox
    songTitle.setFont(componentFont.withHeight(18.0f));
    songTitle.setTextToShowWhenEmpty("Song title...", textEditorEmptyFg);
    songTitle.setColour(juce::TextEditor::backgroundColourId, elementBg);
    songTitle.setColour(juce::TextEditor::textColourId, textEditorFg);
    songTitle.setColour(juce::TextEditor::highlightColourId, textEditorHighlight);
    songTitle.setColour(juce::TextEditor::outlineColourId, elementBg);
    songTitle.setColour(juce::TextEditor::focusedOutlineColourId, textEditorHighlight);
    addAndMakeVisible(songTitle);

    // Song description entrybox
    songDesc.setFont(componentFont.withHeight(18.0f));
    songDesc.setTextToShowWhenEmpty("Song description...", textEditorEmptyFg);
    songDesc.setColour(juce::TextEditor::backgroundColourId, elementBg);
    songDesc.setColour(juce::TextEditor::textColourId, textEditorFg);
    songDesc.setColour(juce::TextEditor::highlightColourId, textEditorHighlight);
    songDesc.setColour(juce::TextEditor::outlineColourId, elementBg);
    songDesc.setColour(juce::TextEditor::focusedOutlineColourId, textEditorHighlight);
    songDesc.setMultiLine(true);
    songDesc.setReturnKeyStartsNewLine(true);
    songDesc.setScrollbarsShown(true);
    addAndMakeVisible(songDesc);

    // Video chooser
    videoSel.setColour(juce::TextButton::buttonColourId, elementBg);
    videoSel.setColour(juce::TextButton::textColourOffId, buttonFg);
    videoSel.setLookAndFeel(&buttonLookAndFeel);
    videoSel.setButtonText("Choose video");
    videoSel.onClick = [this] { videoClicked(); };
    addAndMakeVisible(videoSel);

    // Upload button
    uploadBtn.setColour(juce::TextButton::buttonColourId, elementBg);
    uploadBtn.setColour(juce::TextButton::textColourOffId, buttonFg);
    uploadBtn.setLookAndFeel(&buttonLookAndFeel);
    uploadBtn.setButtonText("Upload");
    uploadBtn.onClick = [this] { uploadClicked(); };
    addAndMakeVisible(uploadBtn);

    // Output info label
    operationOutput.setFont(outputFont.withHeight(19.0f));
    operationOutput.setJustificationType(juce::Justification::centred);
    operationOutput.setColour(juce::Label::backgroundColourId, pluginBg);
    operationOutput.setColour(juce::Label::textColourId, pluginFg);
    operationOutput.setColour(juce::Label::outlineColourId, pluginBg);
    addAndMakeVisible(operationOutput);

    setSize(screenWidth, screenHeight);
}

BeatUploaderAudioProcessorEditor::~BeatUploaderAudioProcessorEditor()
{
    videoSel.setLookAndFeel(nullptr);
}

void BeatUploaderAudioProcessorEditor::videoClicked()
{
    videoChooser = std::make_unique<juce::FileChooser>(
        "Choose video",
        juce::File::getSpecialLocation(juce::File::userDesktopDirectory),
        "*.mov;*.mp4;*.avi"
    );

    auto chooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;

    videoChooser->launchAsync(chooserFlags, [this](const juce::FileChooser& fc){
        auto file = fc.getResult();

        if (file.existsAsFile()) {
            file.loadFileAsData(videoFileData);
            operationOutput.setColour(juce::Label::textColourId, operationSuccessColor);
            operationOutput.setText("Video uploaded", juce::NotificationType::dontSendNotification);
        }
        else {
            operationOutput.setColour(juce::Label::textColourId, operationFailColor);
            operationOutput.setText("Video was not uploaded", juce::NotificationType::dontSendNotification);
        }
    });
}

// check user input and sent oauth request
void BeatUploaderAudioProcessorEditor::uploadClicked()
{
    operationOutput.setColour(juce::Label::textColourId, operationFailColor); // set color to fail in case any condition below is true

    // check title
    if (songTitle.getText().length() == 0) {
        operationOutput.setText("Title cannot be empty", juce::NotificationType::dontSendNotification);
        return;
    }
    if (songTitle.getText().length() > 100) {
        operationOutput.setText("Title can't be longer than 100 chars", juce::NotificationType::dontSendNotification);
        return;
    }

    // check description
    if (songDesc.getText().length() > 5000) {
        operationOutput.setText("Desc. can't be longer than 5000 chars", juce::NotificationType::dontSendNotification);
        return;
    }

    // check audio & image
    if (videoFileData.getSize() == 0) {
        operationOutput.setText("Video was not provided", juce::NotificationType::dontSendNotification);
        return;
    }

    // begin oauth
    juce::String title = songTitle.getText();
    juce::String desc = songDesc.getText();

    operationOutput.setColour(juce::Label::ColourIds::textColourId, pluginFg);
    operationOutput.setText("Opening browser...", juce::NotificationType::dontSendNotification);

    audioProcessor.startUpload(title, desc, videoFileData);
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

    int yPosition = 20 + (2 * sideMargin) + 55; // position on y axis where to draw the next element

    songTitle.setBounds(sideMargin, yPosition, screenWidth - (2 * sideMargin), 27);
    yPosition += (27 + elementMargin);

    songDesc.setBounds(sideMargin, yPosition, screenWidth - (2 * sideMargin), 130);
    yPosition += (130 + elementMargin);

    videoSel.setBounds(sideMargin, yPosition, screenWidth - (2 * sideMargin), 24);

    uploadBtn.setBounds(screenWidth - (sideMargin + 110), screenHeight - (32 + elementMargin + 24), 110, 24);

    operationOutput.setBounds(sideMargin, screenHeight - 32, screenWidth - (2 * sideMargin), 32);
}
