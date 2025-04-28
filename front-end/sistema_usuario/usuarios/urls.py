
from django.urls import path
from . import views

urlpatterns = [
    path('cadastro/', views.cadastro, name='cadastro'),
    path('login/', views.logar, name='login'),
    path('logout/', views.sair, name='logout'),
    path('', views.home, name='home'),
]
