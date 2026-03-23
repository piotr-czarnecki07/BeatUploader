from rest_framework.response import Response
from rest_framework.decorators import api_view
from rest_framework import status as s

from BeatUploaderAPI.settings import env
import requests

@api_view(['POST'])
def videos(request):
    if request.method == "POST":
        title = request.data.get('title')
        desc = request.data.get('desc') # can be empty (None or empty string)
        auth_code = request.data.get('auth_code')

        video_file = request.FILES.get('video_file')

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
            return Response(status=s.HTTP_401_UNAUTHORIZED) # authorizatoin code is invalid - Google did not provide the access token

        # make request to YouTube Data Api
        

        return Response(status=s.HTTP_200_OK)
