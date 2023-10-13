#from django.shortcuts import render

# Create your views here.

from rest_framework.views import APIView
from rest_framework.response import Response
from django.contrib.auth.decorators import login_required
from django.http import JsonResponse

class LoginAPIView(APIView):
        def post(self,request):
                #处理用户登陆逻辑
            return Response({"message:""登陆成功"})




# api/views.py

# from django.contrib.auth.decorators import login_required
# from django.http import JsonResponse

@login_required
def my_api_view(request):
    # 这里是需要登录的 API 视图逻辑
    data = {'message': '这是一个需要登录的 API'}
    return JsonResponse(data)


        

