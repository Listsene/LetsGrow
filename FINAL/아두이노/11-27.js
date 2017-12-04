var status = 1;

var Serialport = require('serialport');
var sp = new Serialport('/dev/ttyACM0',{
    parser : new Serialport.parsers.Readline('\n'),
    baudRate : 500000
});

var value;
var soil1 = 0;
var soil2 = 0;
var soil3 = 0;
var tempAndHumi = 0;
var phValue = 0;

setInterval(function(){

    callfunc(status);
    if(status==1){
        soil1 = value;
        console.log("Value of"+" soil"+status+" :: "+soil1);
    } else if(status==2){
        soil2 = value;
        console.log("Value of"+" soil"+status+" :: "+soil2);
    } else if(status==3){
        soil3 = value;
        console.log("Value of"+" soil"+status+" :: "+soil3);
    } else if(status==4){
        tempAndHumi = value;
        console.log("Value of temperature and humidity :: "+tempAndHumi);
    } else if(status==5){
        phValue=value;
        console.log("Value of PHSensor :: "+phValue);
    }
    if(status==6){
        status = 0;
    }
    status++;
}, 2000);
function isValue(value){
    while(value == null){
    }
    return;
}

function callfunc(status) { // 1 2 3 
    var fs = require('fs');
    console.log(status + '번째 callfuinction...');
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
