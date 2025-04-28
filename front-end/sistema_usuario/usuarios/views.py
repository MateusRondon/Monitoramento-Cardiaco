
from django.shortcuts import render, redirect
from django.contrib.auth.models import User
from django.contrib.auth import authenticate, login, logout
from django.contrib import messages

def cadastro(request):
    if request.method == 'POST':
        username = request.POST['username']
        password = request.POST['password']
        if User.objects.filter(username=username).exists():
            messages.error(request, 'Usuário já existente!')
            return redirect('cadastro')
        user = User.objects.create_user(username=username, password=password)
        user.save()
        messages.success(request, 'Usuário cadastrado com sucesso!')
        return redirect('login')
    return render(request, 'cadastro.html')

def logar(request):
    if request.method == 'POST':
        username = request.POST['username']
        password = request.POST['password']
        user = authenticate(request, username=username, password=password)
        if user is not None:
            login(request, user)
            return redirect('home')
        else:
            messages.error(request, 'Usuário ou senha inválidos.')
            return redirect('login')
    return render(request, 'login.html')

def sair(request):
    logout(request)
    return redirect('login')

def home(request):
    return render(request, 'home.html')
