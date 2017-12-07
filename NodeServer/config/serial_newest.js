var status = 1;

var Serialport = require('serialport');
var sp = new Serialport('/dev/ttyACM0',{
    parser : new Serialport.parsers.Readline('\n'),
    baudRate : 500000
});

var dataStack = [];
var index=0;
var value;

setInterval(function(){

    if(status<8){
        callfunc(status);
        dataStack[index]=value;
        console.log("Value of Sensor is ..."+dataStack[index++]);
    }  else if(status==8) status=0;
    status++;
}, 2000);

function isValue(value){
    while(value == null){
    }
    return;
}

function callfunc(status) { // 1 2 3 
    var fs = require('fs');
    //console.log(status + '번째 callfuinction...');
    console.log("아두이노에서 값을 읽어옵니다....");
    // Appending 모드로 /dev/ttyACM0 장치를 Open
    const fd = fs.openSync('/dev/ttyACM0', 'a', 66666);
    var length = fs.writeSync(fd, status, null, null, null);

    sp.on('data', function(data){
        value = data;
    });
    if(length == 0){
        console.log("파일 전송 에러")
    }
}
