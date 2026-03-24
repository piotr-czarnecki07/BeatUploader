from rest_framework.response import Response
from rest_framework.decorators import api_view
from rest_framework import status as s

from google.oauth2.credentials import Credentials
from googleapiclient.discovery import build
from googleapiclient.http import MediaIoBaseUpload

from BeatUploaderAPI.settings import env
import requests

@api_view(['POST'])
def videos(request):
    if request.method == "POST":
        # validate received data
        title = request.data.get('title')
        desc = request.data.get('desc') # can be empty (None or empty string)
        if desc is None:
            desc = ""

        auth_code = request.data.get('auth_code')

        video_file = request.FILES.get('video_file.mp4')

        if None in (title, auth_code, video_file):
            return Response({'error': '"title", "auth_code" or "video_file" parameter is missing'}, s.HTTP_400_BAD_REQUEST)

        # obtain access token
        token_response = requests.post(url='https://oauth2.googleapis.com/token', params={
            'code': auth_code,
            'client_id': '666993375983-simteqq6ufsp23fsmnern1k1kb4v6oam.apps.googleusercontent.com',
            'client_secret': env('CLIENT_SECRET'),
            'redirect_uri': 'http://127.0.0.1:8080/',
            'grant_type': 'authorization_code'
        })

        if token_response.status_code == 200:
            access_token = token_response.json().get('access_token')
        else:
            return Response({'error': 'Authorization code is invalid'}, s.HTTP_401_UNAUTHORIZED) # authorizatoin code is invalid - Google did not provide the access token

        # make request to YouTube Data Api
        try:
            credentials = Credentials(token=access_token)
            youtube = build('youtube', 'v3', credentials=credentials)

            body = {
                'snippet': {
                    'title': title,
                    'description': desc,
                    'tags': ['type beat', 'music', 'music production'],
                    'categoryId': 10 # music categoryid
                },
                'status': {
                    'privacyStatus': 'private' # can be changed to 'public' when app is authorised at Google
                }
            }

            media = MediaIoBaseUpload(
                video_file,
                mimetype='video/mp4',
                chunksize=-1,
                resumable=True
            )

            upload_request = youtube.videos().insert(
                part=','.join(body.keys()),
                body=body,
                media_body=media
            )

            youtube_response = None
            while youtube_response is None:
                status, youtube_response = upload_request.next_chunk()
                if not status:
                    return Response({'error': 'Upload has been interrupted'}, s.HTTP_500_INTERNAL_SERVER_ERROR)

            return Response({'id': str(youtube_response['id'])}, s.HTTP_200_OK)

        except Exception as e:
            return Response({'error': f'Error occured while uploading: {e}'}, s.HTTP_500_INTERNAL_SERVER_ERROR)

    return Response({'error': f"'{request.method}' method not allowed"}, s.HTTP_405_METHOD_NOT_ALLOWED)
