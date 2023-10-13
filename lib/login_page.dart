import 'package:flutter/material.dart';



class LoginPage extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text('登录'),
      ),
      body: Padding(
        padding: EdgeInsets.all(16.0),
        child: Column(
          children: [
            TextField(
              decoration: InputDecoration(labelText: 'ID'),
            ),
            SizedBox(height: 20.0),
            TextField(
              obscureText: true,
              decoration: InputDecoration(labelText: '密码'),
            ),
            SizedBox(height: 20.0),
            ElevatedButton(
              onPressed: () {
                // 处理登录逻辑
              },
              child: Text('登录'),
            ),
          ],
        ),
      ),
    );
  }
}


