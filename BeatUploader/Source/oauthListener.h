#include <JuceHeader.h>

#pragma once

class OAuthListenerThread : public juce::Thread
{
public:
    OAuthListenerThread(juce::String clientId, juce::String title, juce::String desc, juce::MemoryBlock audio, juce::MemoryBlock image)
        : juce::Thread("OAuthListener"),
          googleClientId(clientId),
          songTitle(title),
          songDesc(desc),
          audioData(audio),
          imageData(image) {}

    void run() override
    {
        juce::StreamingSocket serverSocket;
        int port = 8080;

        // if 127.0.0.1:8080 is occupied
        if (!serverSocket.createListener(port, "127.0.0.1")) {
            return;
        }

        // wait for response from Google Authorization Server
        while (!threadShouldExit()) {
            std::unique_ptr<juce::StreamingSocket> clientSocket(serverSocket.waitForNextConnection());

            // response has been sent
            if (clientSocket != nullptr) {
                if (clientSocket->waitUntilReady(true, 1000) > 0) {

                    char buffer[2048] = { 0 };
                    int readBytes = clientSocket->read(buffer, sizeof(buffer), false);

                    if (readBytes > 0) {
                        juce::String request(buffer);

                        if (request.contains("GET /favicon.ico")) {
                            continue;
                        }

                        juce::String authCode = extractCodeFromRequest(request);

                        if (authCode.isNotEmpty()) {
                            sendHttpResponse(clientSocket.get(), "Your video is being processed now<br>You can close this window", "Success"); // inform browser
                            sendDataToProcess(authCode); // send user data and auth_code to backend
                        }
                        else if (request.contains("error=")) {
                            sendHttpResponse(clientSocket.get(), "Authorization faild<br>Please try again or choose different account", "Fail");
                        }

                        // slight delay to not end the connection too early
                        juce::Thread::sleep(200);

                        signalThreadShouldExit();
                    }
                }
            }
        }

        serverSocket.close();
    }

private:
    juce::String googleClientId;
    juce::String songTitle;
    juce::String songDesc;
    juce::MemoryBlock audioData;
    juce::MemoryBlock imageData;

    juce::String extractCodeFromRequest(const juce::String& request)
    {
        int codeIndex = request.indexOf("code=");
        if (codeIndex != -1) {
            int endIndex = request.indexOfChar(codeIndex, '&');
            if (endIndex == -1) endIndex = request.indexOfChar(codeIndex, ' ');
            
            if (endIndex != -1)
                return request.substring(codeIndex + 5, endIndex);
        }
        return {};
    }

    void sendHttpResponse(juce::StreamingSocket* socket, const juce::String responseMessage, const juce::String responseTitle)
    {
        juce::String header = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=utf-8\r\n\r\n";

        juce::String htmlBody = R"html(
        <!DOCTYPE html>
        <html>
        <head>
            <meta charset="utf-8">
            <title>)html" + responseTitle + R"html(</title>
            <link rel="preconnect" href="https://fonts.googleapis.com">
            <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
            <link href="https://fonts.googleapis.com/css2?family=Lexend:wght@300;400;700&family=Michroma&display=swap" rel="stylesheet">
        </head>
        <body style="text-align: center; margin-top: 20px; background-color: #1e1e1e;">
            <div style="font-family: 'Michroma', sans-serif; font-weight: 400; font-style: normal; color: #ececec; font-size: 32px;">BeatUploader</div>
            <div style="margin-top: 30px; font-family: 'Lexend', sans-serif; font-optical-sizing: auto; font-weight: normal; font-style: normal; color: #e9e9e9; font-size: 14px;">)html" + responseMessage + R"html(</div>
        </body>
        </html>
        )html";

        juce::String fullResponse = header + htmlBody;
        socket->write(fullResponse.toRawUTF8(), fullResponse.getNumBytesAsUTF8());
    }

    void sendDataToProcess(const juce::String& authCode)
    {
        juce::URL backendUrl("http://127.0.0.1:5500/api/videos/"); // backend server URI

        backendUrl = backendUrl.withParameter("auth_code", authCode) // add text data and auth code to request
            .withParameter("title", songTitle)
            .withParameter("desc", songDesc);

        // convert MemoryBlocks to temporary files
        juce::File tempAudio = juce::File::getSpecialLocation(juce::File::tempDirectory).getChildFile("temp_audio.wav");
        tempAudio.replaceWithData(audioData.getData(), audioData.getSize());

        juce::File tempImage = juce::File::getSpecialLocation(juce::File::tempDirectory).getChildFile("temp_image.png");
        tempImage.replaceWithData(imageData.getData(), imageData.getSize());

        // add these files to request
        backendUrl = backendUrl.withFileToUpload("audio_file", tempAudio, "audio/wav")
            .withFileToUpload("image_file", tempImage, "image/png");

        // make request
        juce::URL::InputStreamOptions options(juce::URL::ParameterHandling::inPostData);
        std::unique_ptr<juce::InputStream> stream(backendUrl.createInputStream(options));

        if (stream != nullptr) {
            juce::String response = stream->readEntireStreamAsString();
        }

        tempAudio.deleteFile();
        tempImage.deleteFile();
    }
};
