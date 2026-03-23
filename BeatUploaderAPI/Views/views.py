from rest_framework.response import Response
from rest_framework.decorators import api_view
from rest_framework import status as s

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

        return Response(status=s.HTTP_200_OK)
