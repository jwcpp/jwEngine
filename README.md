# jwEngine
一个跨平台的c++<->lua服务器快速解决方案，该框架即可快速响应服务器开发工作，设计思想：“让事情变得更简单”

网络底层采用libuv（node.js底层库），异步io强势助力，单线程也能释放澎湃动力，跨平台支持epoll、iocp、ipv6。框架支持tcp、udp/kcp、websocket、http，使用了sol2将所有接口都导出到lua，可以选择用lua开发逻辑。

使用modern c++开发，尽可能的使用std::move、std::string_view减少内存复制。

该框架使用异步事件，不建议使用多线程，避免多线程带来的上下文切换和不完美代码，网络部分和逻辑部分使用一个主事件循环驱动。建议的方案是多进程单线程的横向扩展，按照业务控制各个进程的粒度，当然mysql和redis可以加入到线程池中。

## 创建一个tcp服务器
只需要简单几行代码即可创建一个tcp高性能服务器，并自动处理数据包头和粘包，构建一个完好的NetPacket交给你。

```cpp
class INetEvent : public NetEvent
{
public:
  virtual void onAccept(NetConnect * conn){}
  virtual void onClose(NetConnect * conn){}
  virtual void onMsg(NetConnect * conn, int msgtype, NetPacket * pack){}
};

int main()
{
  CommPool::init<NetPacket>(100);

  EventLoop::Instance()->init();

  INetEvent eve;
  NetServer server(EventLoop::Instance(), &eve);
  server.listen("127.0.0.1", 3001);

  EventLoop::Instance()->Run();

  return 0;
}
```
同样，你可以创建kcpServer和websocketServer，该框架保证了接口的一致性。

## 创建一个http服务器
http仅支持简单的get post请求

```cpp
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

int main()
{
  CommPool::init<HttpConnect>(10);

  EventLoop::Instance()->init();
  HttpServer server(EventLoop::Instance());
  server.listen("127.0.0.1", 80);

  server.addGet("/", [](HttpConnect *conn, std::string_view & data) {
    conn->autoMsg(html);
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

  EventLoop::Instance()->Run();
  return 0;
}
```
## mysql和线程池
这次我们用lua示例：
```lua
local config = DBConfig:new()
config.device = "mysql"
config.ip = "127.0.0.1"
config.dbname = "jw_test"
config.user = "root"
config.pswd = "1111"
config.port = 3306

pool = DBThreadPool:new(config)
pool:create(1)

func = function(result)
  while(result:fetch())
  do
    local id = result:getInt32()
    local num = result:getInt32()
    local name = result:getString()

    local str = "id:" .. id .. ", num:" .. num .. ", name:" .. name
    print(str)
  end
end

function exec()
  local sql = SqlCommand:new("select * from test where id = ?")
  sql:pushInt32(1)
  sql:addToPool(pool, func)
end

event_init()
exec()
event_run()
```

## 任意扩展进程节点
你可以任意扩展你的进程，示例：

| base进程                       | cell进程                  |db进程                    |
| ------------------------------ |:-------------------------:| -----------------------:|
|    start engine.exe base.lua   | start engine.exe cell.lua | start engine.exe db.lua |


## 提供一个serialization序列化工具
类似于c++的语法，写起来非常简单，示例：
```
struct testmsg
{
  int32 x
  int32 y
  int32 z
  int8 state
  vector<int32> vec
  
  read{
    [x, y, z, state]
    if(state == 1)
    {
      [vec]
    }
  }
  write{
    [x, y, z, state, vec]
  }
}
```
通过serialization工具进行字节解析、提取词法、语法匹配、错误定位，最终可以生成c++或者lua代码。倘若该工具无法满足你，你也可以非常轻松的接入protobuff。

## 构建
你需要一个modern c++17编译器
- vs2017 测试通过
- gcc version 9.3.0 测试通过

## 进一步了解
更多使用示例请参考[example](https://github.com/jwcpp/jwEngine/tree/master/example). 同时欢迎fork和贡献代码

## 建议
有任何意见或框架调整的建议，请联系QQ:754420038
