from rest_framework.response import Response
from rest_framework.decorators import api_view
from rest_framework import status as s

from moviepy import ImageClip, AudioFileClip

@api_view(['POST'])
def videos(request):
    if request.method == "POST":
        title = request.data.get('title')
        desc = request.data.get('desc') # can be empty (None or empty string)
        auth_code = request.data.get('auth_code')

        image_file = request.FILES.get('image_file')
        audio_file = request.FILES.get('audio_file')

        if None in (title, auth_code, image_file, audio_file):
            return Response({'error': '"title", "auth_code", "image_file" or "audio_file" parameter is missing'}, s.HTTP_400_BAD_REQUEST)

        # 1 stworz plik video


        # 2 zdobadz tokeny

        # 3 wykonaj zapytanie do YouTube Data API

        return Response(status=s.HTTP_200_OK)
