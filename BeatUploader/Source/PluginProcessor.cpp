/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

BeatUploaderAudioProcessor::BeatUploaderAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

BeatUploaderAudioProcessor::~BeatUploaderAudioProcessor()
{
}

const juce::String BeatUploaderAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BeatUploaderAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool BeatUploaderAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool BeatUploaderAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double BeatUploaderAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BeatUploaderAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int BeatUploaderAudioProcessor::getCurrentProgram()
{
    return 0;
}

void BeatUploaderAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String BeatUploaderAudioProcessor::getProgramName (int index)
{
    return {};
}

void BeatUploaderAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

void BeatUploaderAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void BeatUploaderAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool BeatUploaderAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void BeatUploaderAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }
}

bool BeatUploaderAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* BeatUploaderAudioProcessor::createEditor()
{
    return new BeatUploaderAudioProcessorEditor (*this);
}

void BeatUploaderAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void BeatUploaderAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BeatUploaderAudioProcessor();
}

void BeatUploaderAudioProcessor::startUpload(const juce::String& title,
                                             const juce::String& desc,
                                             const juce::MemoryBlock& audio,
                                             const juce::MemoryBlock& image)
{
    if (oauthThread != nullptr) {
        oauthThread->stopThread(2000);
        oauthThread.reset();
    }

    juce::String clientId = "666993375983-simteqq6ufsp23fsmnern1k1kb4v6oam.apps.googleusercontent.com";
    juce::String redirectUri = "http://127.0.0.1:8080/"; // address, on which the listener thread will wait for Google response (NOT backend server host)
    juce::String scope = "https://www.googleapis.com/auth/youtube.upload";

    oauthThread = std::make_unique<OAuthListenerThread>(clientId, title, desc, audio, image);
    oauthThread->startThread();

    juce::URL authUrl("https://accounts.google.com/o/oauth2/v2/auth");
    authUrl = authUrl.withParameter("client_id", clientId)
        .withParameter("redirect_uri", redirectUri)
        .withParameter("response_type", "code")
        .withParameter("scope", scope);

    authUrl.launchInDefaultBrowser();
}
