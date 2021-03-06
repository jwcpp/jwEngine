var ws = new WebSocket("ws://localhost:8080");
ws.onopen = function()
{
	var pos = 0;
	var msg = "你好 jwEngine";
	var buff = new ArrayBuffer(4 + (msg.length + 1) + 4);
	var dv = new DataView(buff);
	dv.setInt32(pos, 110, true);
	pos += 4;
	for (var i=0; i<msg.length; i++)
	{
		dv.setUint8(pos, msg[i].charCodeAt(), true)
		pos += 1;
	}
	// c++ char* end is 0
	dv.setUint8(pos, 0, true);
	pos += 1;
	dv.setInt32(pos, 666, true);

  	ws.send(buff);
};

ws.onmessage = function (evt) 
{ 
	var reader = new FileReader();
	reader.readAsArrayBuffer(evt.data)
	reader.onload = () => {
		var str = ""
    	var buff = reader.result;
    	var dv = new DataView(buff);
    	// c++ char* end is 0
    	for (var i=0; i < evt.data.size - 1; i++)
		{
			str += String.fromCharCode(dv.getUint8(i, true));
		}
		alert(str)
    };
};

ws.onclose = function()
{ 
	alert("close"); 
};
