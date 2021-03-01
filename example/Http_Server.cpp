#include <stdio.h>
#include <stdlib.h>

#include <string_view>
#include <functional>
#include <map>
#include "HttpServer.h"
#include "EventLoop.h"

#include <iostream>
#include "HttpParam.h"
#include "HttpConnect.h"

const char * html = R"(<html>
    <body>
        <h1>login</h1>
        <p>hello world!</p>
        <form action="login" method="post">
            <input type="text" name="user"/>
            <input type="password" name="pass"/>
            <input type="submit" value="login"/>
        </form>
    </body>
	</html>)";


const char * html2 = R"(<!DOCTYPE html>
	<html>
	<head>
	<script src="https://code.jquery.com/jquery-3.5.1.min.js"></script>
	<script>
	$(document).ready(function(){
	  $("button").click(function(){
		$.get("/test",function(data,status){
		  alert("data:" + data + "\nstate:" + status);
		});
	  });
	});
	</script>
	</head>
	<body>
	<button>send</button>
	</body>
	</html>)";

const char * succeed = ""
"<html>"
"<body>"
"<h1>login succeed</h1>"
"</body>"
"</html>";

const char * failing = ""
"<html>"
"<body>"
"<h1>login failing</h1>"
"</body>"
"</html>";


#include "XFile.h"

int main()
{
	EventLoop::Instance()->init();
	HttpServer server(EventLoop::Instance());
	server.listen("127.0.0.1", 80);

	server.addGet("/", [](HttpConnect *conn, std::string_view & data) {
		conn->autoMsg(html);
		/*
		std::string s;
		XFile::readFile("test.html", s);
		conn->sendMsg(s);
		*/
	});

	server.addGet("/test", [](HttpConnect *conn, std::string_view & data) {
		conn->autoMsg("hello world");
	});

	server.addPost("/login", [](HttpConnect *conn, std::string_view & data) {

		HttpParam hp(data);
		if (hp.getStr("user") == "jw" && hp.getStr("pass") == "1111")
		{
			conn->autoMsg(succeed);
		}
		else
		{
			conn->autoMsg(failing);
		}
	});

	EventLoop::Instance()->run();
	return 0;
}