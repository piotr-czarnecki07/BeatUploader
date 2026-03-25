# BeatUploader

VST audio plugin for uploading music on YouTube directly from Your DAW!

Written in C++ with the JUCE framework\
Backend written in Python with Django Rest Framework

## Table of Contents

- [How it Works](#how-it-works)
- [Contributing](#contributing)
- [VST File](#vst-file)
- [License](#license)
- [Credits](#credits)

## How it Works

VST plugin is to be opened as "a synth", which means that it can be open either as a standalone program or as a plugin in Digital Audio Workstation (for example in FL Studio it is opened with Channel Rack, not in the Mixer).

Plugin is used to upload individual's work to YouTube by using the Google's OAuth2.0 and YouTube Data API v3.\
Takes basic YouTube video data as input, e.g. title and description, as well as the video file, which is choosen using the file dialog.

> [!NOTE]
> It's planned to input user for image and audio file and make a video of these files instead.\
> Work on this approach can be a subject of later updates.

After the data is collected the plugin opens user's browser where they are requested to accept OAuth scopes.\
Plugin creates listening socket on another thread, which waits for response from Google on a host specified in "redirect_uri" parameter.\
When response is received, plugin extracts authorization code, responds with a message to the browser, sends the code to backend to process and after getting response - closes the thread.

When backend receives the request from plugin, it validates user data, exchanges authorization code to access token and sends request to YouTube Data API with metadata and video file.

## Contributing

To contribute to the project you have to set up JUCE framework on your machine properly.\
This is a rather laborious process, so the details may be explained in later updates.

For backend contribution, you need a `.env` file, that is made using [.env.example](Backend/BeatUploaderAPI/.env.example), as well as Python 3.13.3+ with requirements installed from [requirements.txt](Backend/BeatUploaderAPI/requirements.txt).

## VST File

See latest release.

## License

See [License](License) for information about project licensing

## Credits

JUCE framework: [https://github.com/juce-framework/JUCE](https://github.com/juce-framework/JUCE)\
Stainberg media VST3 SDK: [https://github.com/steinbergmedia/vst3sdk](https://github.com/steinbergmedia/vst3sdk)\
Fonts: Each font used in project GUI has it's OFL.txt file associated with `.ttf` file in `BeatUplodaer/Fonts/` directory.
