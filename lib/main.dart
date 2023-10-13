
import 'package:flutter/material.dart';
import 'login_page.dart';
import 'package:http/http.dart' as http;


void main() => runApp(const MyApp());

class MyApp extends StatelessWidget {
  const MyApp({super.key});







  @override
  Widget build(BuildContext context) {
    //这个是正确的
  //   Widget titleSection = Container(
  // padding: const EdgeInsets.all(32),
  // child: Row(
  //   children: [
  //     Expanded(
  //       /*1*/
  //       child: Column(
  //         crossAxisAlignment: CrossAxisAlignment.start,
  //         children: [
  //           /*2*/
  //           Container(
  //             padding: const EdgeInsets.only(bottom: 8),
  //             child: const Text(
  //               'Oeschinen Lake Campground',
  //               style: TextStyle(
  //                 fontWeight: FontWeight.bold,
  //               ),
  //             ),
  //           ),
  //           Text(
  //             'Kandersteg, Switzerland',
  //             style: TextStyle(
  //               color: Colors.grey[500],
  //             ),
  //           ),
  //         ],
  //       ),
  //     ),
  // 到这里为止，都是正确的





  Widget titleSection = Container(
  padding: const EdgeInsets.all(32),
  child: Column(
    crossAxisAlignment: CrossAxisAlignment.start,
    children: [
      Container(
        padding: const EdgeInsets.only(bottom: 8),
        child: const Text(
          '希望赫赫快乐每一天',
          style: TextStyle(
            fontWeight: FontWeight.bold,
          ),
        ),
      ),
      Text(
        '혁이가 매일 행복하기를 바랍니다."',
        style: TextStyle(
          color: Colors.grey[500],
        ),
      ),
      SizedBox(height: 20.0), // <-- Add this line
      TextField(
        decoration: InputDecoration(
          labelText: 'ID',
          prefixIcon: Icon(Icons.person),
        ),
      ),
      SizedBox(height: 20.0),
      TextField(
        obscureText: true,
        decoration: InputDecoration(
          labelText: '비밀번호',
          prefixIcon: Icon(Icons.lock),
        ),
      ),
    ],
  ),
);

Color color = Theme.of(context).primaryColor;

//这是添加登陆的选项
Widget loginButton = ElevatedButton(
  //下面这几行是最初的代码，先屏蔽，我测试下登陆
//   onPressed: () {
//     // Add your login logic here
//   },
//   child: Text('로그인'),
// );
////////////////////到这里为止
// ... (以下为原有代码)



  onPressed: () async {
    var url = Uri.parse('http://127.0.0.1:8000/');// 替换成你的Django API的URL

    var response = await http.post(
      url,
      body: {
        'username': 'aichidami065', // 替换成实际的用户名
        'password': '123456789', // 替换成实际的密码

      },
    );

    if (response.statusCode == 200) {
      // 登录成功
      print('登录成功');
      print('Response data: ${response.body}');
    } else {
      // 登录失败
      print('登录失败');
      print('Response status code: ${response.statusCode}');
    }
  },
  child: Text('登录'),
);
 




      /*3*/
      // Icon(
      //   Icons.star,
      //   color: Colors.red[500],
      // ),
      // const Text('41'),
//     ],
//   ),
// );
//     Color color = Theme.of(context).primaryColor;
    //下面是布局设置




Widget buttonSection = Row(
  mainAxisAlignment: MainAxisAlignment.spaceEvenly,
  children: [
    _buildButtonColumn(color, Icons.call, '연락방식'),
    _buildButtonColumn(color, Icons.near_me, '비밀 잊음'),
    _buildButtonColumn(color, Icons.share, '링크'),
  ],
);






Widget textSection = Container(


  padding: const EdgeInsets.all(32),
  child: const Text(
    '장해 어린이 전용 앱, 해피 데이를 기록하는 소프트웨어입니다. 부모님도 해피의 성장과 생활을 기록할 수 있습니다.',
    softWrap: true,
  ),
);









//又一个登陆页面
// Widget loginButton = ElevatedButton(
//   onPressed: () {
//     showDialog(
//       context: context,
//       builder: (BuildContext context) {
//         return AlertDialog(
//           title: Text('登录'),
//           content: Column(
//             children: [
//               TextField(
//                 decoration: InputDecoration(labelText: 'ID'),
//               ),
//               SizedBox(height: 20),
//               TextField(
//                 obscureText: true,
//                 decoration: InputDecoration(labelText: '密码'),
//               ),
//             ],
//           ),
//           actions: <Widget>[
//             ElevatedButton(
//               onPressed: () {
//                 // 处理登录逻辑
//                 Navigator.of(context).pop(); // 关闭对话框
//               },
//               child: Text('登录'),
//             ),
//           ],
//         );
//       },
//     );
//   },
//   child: Text('登录'),
// );






    return MaterialApp(
      title: "先空着",
      home:Scaffold(
        appBar: AppBar(
          title:const Text("장혁 어린이 개인 app"),
        ),
        //body:ListView(
          body:ListView(
          //因为App的运行需要ListView来支持
          children: [
            Image.asset(
              "assets/heh.png",
              width:500,
              height:400,
              fit: BoxFit.cover,
            ),
            titleSection,
            buttonSection,
            textSection,
            loginButton,
          ],
        ),
      ),
    );

    // ···
  }
}

  Column _buildButtonColumn(Color color, IconData icon, String label) {
    return Column(
      mainAxisSize: MainAxisSize.min,
      mainAxisAlignment: MainAxisAlignment.center,
      children: [
        Icon(icon, color: color),
        Container(
          margin: const EdgeInsets.only(top: 8),
          child: Text(
            label,
            style: TextStyle(
              fontSize: 12,
              fontWeight: FontWeight.w400,
              color: color,
            ),
          ),
        ),
      ],
    );
  }

















// //下面是布局设置
// Widget titleSection = Container(
//   padding: const EdgeInsets.all(32),
//   child: Row(
//     children: [
//       Expanded(
//         /*1*/
//         child: Column(
//           crossAxisAlignment: CrossAxisAlignment.start,
//           children: [
//             /*2*/
//             Container(
//               padding: const EdgeInsets.only(bottom: 8),
//               child: const Text(
//                 'Oeschinen Lake Campground',
//                 style: TextStyle(
//                   fontWeight: FontWeight.bold,
//                 ),
//               ),
//             ),
//             Text(
//               'Kandersteg, Switzerland',
//               style: TextStyle(
//                 color: Colors.grey[500],
//               ),
//             ),
//           ],
//         ),
//       ),
//       /*3*/
//       Icon(
//         Icons.star,
//         color: Colors.red[500],
//       ),
//       const Text('41'),
//     ],
//   ),
// );