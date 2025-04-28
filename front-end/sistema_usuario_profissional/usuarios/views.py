
from django.contrib.auth.views import LoginView, LogoutView, PasswordResetView, PasswordResetConfirmView
from django.urls import reverse_lazy
from django.views.generic import CreateView, TemplateView
from django.contrib.auth.models import User
from django.contrib.auth.decorators import login_required
from django.utils.decorators import method_decorator
from django import forms

class UserRegisterForm(forms.ModelForm):
    password = forms.CharField(widget=forms.PasswordInput)
    class Meta:
        model = User
        fields = ['username', 'email', 'password']

class RegisterView(CreateView):
    template_name = 'usuarios/cadastro.html'
    form_class = UserRegisterForm
    success_url = reverse_lazy('login')

    def form_valid(self, form):
        user = form.save(commit=False)
        user.set_password(form.cleaned_data['password'])
        user.save()
        return super().form_valid(form)

class CustomLoginView(LoginView):
    template_name = 'usuarios/login.html'

class CustomLogoutView(LogoutView):
    next_page = reverse_lazy('login')

@method_decorator(login_required, name='dispatch')
class HomeView(TemplateView):
    template_name = 'usuarios/home.html'

class CustomPasswordResetView(PasswordResetView):
    template_name = 'usuarios/password_reset.html'
    success_url = reverse_lazy('password_reset_done')

class CustomPasswordResetConfirmView(PasswordResetConfirmView):
    template_name = 'usuarios/password_reset_confirm.html'
    success_url = reverse_lazy('login')
