# jwEngine  
一个跨平台的c++<->lua服务器快速解决方案，该框架即可快速响应服务器开发工作，设计思想：“让事情变得更简单”  
A cross platform C + + < - > Lua server fast solution, the framework can quickly respond to the server development work, the design idea: "make things easier"

## c++ tcp server  
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
    EventLoop::Instance()->init();

    INetEvent eve;
    NetServer server(EventLoop::Instance(), &eve);
    server.listen("127.0.0.1", 3001);

    return EventLoop::Instance()->run();
}
```

## lua tcp server  
```lua
event_init()

server = NetServer:new()
server.on_accept = function(conn)
end

server.on_close = function(conn)
end

server.on_msg = function(conn, msgtype, pack)
end

server:listen("127.0.0.1", 3001, false)

event_run()
```

## start run  

| base process                   | cell process              |db process               |
| ------------------------------ |:-------------------------:| -----------------------:|
|    start engine.exe base.lua   | start engine.exe cell.lua | start engine.exe db.lua |


## serialization  
The serialization tool provided is experimental and may be fragile. It is recommended to use more powerful protobuf


## Integrated open source library  
[libuv](https://github.com/libuv/libuv)  
[sol2](https://github.com/ThePhD/sol2)  
[concurrentqueue](https://github.com/cameron314/concurrentqueue)  
[lua](https://github.com/lua/lua)  
[hiredis](https://github.com/redis/hiredis)  
[kcp](https://github.com/skywind3000/kcp)  
[lua-protobuf](https://github.com/starwing/lua-protobuf)  
[http-parser](https://github.com/nodejs/http-parser)  
[llhttp](https://github.com/nodejs/llhttp)  

## Learn more  
For more examples, please refer to [example](https://github.com/jwcpp/jwEngine/tree/master/example)

更多项目：[jwServer](https://github.com/jwcpp/jwServer) | [jwNet](https://github.com/jwcpp/jwNet)

有任何建议、BUG、疑问，统一解决QQ群:458421068  
My email: jw_cpp@163.com
