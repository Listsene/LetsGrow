var SerialPort = require('serialport');
var port = new SerialPort('/dev/ttyACM0', {
  baudRate: 9600
});
port.on("open", function () { 
  console.log('open'); 
  port.write('1', function(err) {
	if(err)
		return console.log(err);
	console.log('1 written');
  });
});

port.on('data', function(data){
	console.log('Read and Send Data : ' + data);
	port.write(data);
});

port.on('error', function(err){
	console.log('Error : ' + err);
});
