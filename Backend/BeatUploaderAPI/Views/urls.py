from django.urls import path
from Views import views

urlpatterns = [
    path('api/videos/', views.videos)
]
