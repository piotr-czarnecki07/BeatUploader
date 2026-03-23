from rest_framework.response import Response
from rest_framework.decorators import api_view
from rest_framework import status as s

@api_view(['POST'])
def videos(request):
    if request.method == "POST":
        title = request.data['title']
        desc = request.data['desc']
        auth_code = request.data['auth_code']

        image = request.data['image_file']
        audio = request.data['audio_file']

        with open('test.txt') as f:
            f.write(f'title: {title}\ndesc: {desc}\nauth_code: {auth_code}\nimage: {image}\naudio: {audio}')

        return Response(status=s.HTTP_200_OK)
