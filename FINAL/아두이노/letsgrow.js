var status = 0;

var serialport = require('serialport');
var Serialport = serialport.SerialPort;

var sp = new Serialport('/dev/ttyACM0',{
    parser : serialport.parsers.raw
});

var soil = 0;
/*
serialPort.on('data', function(data)){
    sys.puts('Value : ' + data);
}*/

setInterval(function(){
    console.log(status+'번째를 실행합니다...');
    callfunc(status);
    status++;
    if(status==4) {status=0};
}, 3000);

function callfunc(status) {
    var fs = require('fs');
    console.log(status + '번째 callfunction...');
    // Appending 모드로 /dev/tty-usbserial1 장치를 Open
    fs.open('/dev/ttyACM0', 'a', 666, function( e, fd ) {
        fs.write( fd, status, null, null, null, function(){
            fs.close(fd, function(){
                console.log('Sending ', status);
            });
        });
        /*fs.on('data',function(data){
            console.log('Read and Send Data : '+data);
        });
    */
        /*serialPort.on('data', function(data)){
            sys.puts('Value : ' + data);
        });*/
        fs.read('/dev/ttyACM0','a',666,function(err,data){
            if(status==0){
                soil = data;
                console.log(soil);
            }
        })
    });
}