var status = 1;

var Serialport = require('serialport');
var sp = new Serialport('/dev/ttyACM0',{
    parser : new Serialport.parsers.Readline('\n'),
    baudRate : 500000
});
var index = 0;  
sp.on('data', function(data){
    var res = data.split(":");
    switch(res[0]){
        case '1':
            console.log('soil 1 : ' + res[1]);
            break;
        case '2':
            console.log('soil 2 : ' + res[1]);
            break;    
        case '3':
            console.log('soil 3 : ' + res[1]);
            break;    
        case '4':
            console.log('sun : ' + res[1]);
            break;    
        case '5':
            console.log('weight : ' + res[1]);
            break;    
        case '6':
            console.log('temp/moist : ' + res[1]);
            break;    
        case '7':
            console.log('ph : ' + res[1]);
            break;    
        default:
            break;  
    }
});

var value;
var soil = 0;
var tempAndHumi = 0;
var phValue = 0;

setInterval(function(){
    
    callfunc(status);
    if(status==7){
        status = 0;
    }
    status++;
}, 4000);
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

    if(length == 0){
        console.log("파일 전송 에러")
    }
}