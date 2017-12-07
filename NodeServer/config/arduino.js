var Serialport = require('serialport');
var fs = require('fs');

var flower = require('../model/flower');
var plant = require('../model/plant');
var sensor = require('../model/sensor');


var conf_status = 1;

var serialObj = new Serialport('/dev/ttyACM0',{
		parser : new Serialport.parsers.Readline('\n'),
		baudRate : 500000
});

var dataStack = [];
var index=0;
var value;
var sensors = [];

var tmp = {};

var max_size = 4; //42-> 588초 <- 9.8분 걸린다.

setInterval(function(){
	if(conf_status<8){
		callfunc(conf_status);
		dataStack[conf_status-1]=value;
		console.log("Value of Sensor is ..."+dataStack[conf_status-1]);
	}else if(conf_status==8){
		if(sensors.length >= max_size){
			// 평균화 작업
			sensors = normalize(sensors);
			for(var io in sensors){
				console.log(sensor[io])
				console.log(sensor[io].temp);
				console.log(sensor[io].humi);
			}
			// DB 저장 <- DEFAULT section1, section2, section3(선택이 필요하다.)
			// Section을 선택할 때까지 기다린다
			sensors = [];
		}
		var temp;
		tmp = {};
		for(var each in dataStack){ // each 0~7
			if(dataStack[each].charAt(1) == ':'){
		    	t = dataStack[each].split(':');
		    	// Memory <- max_size개 싸이면 작업 처리 맞기고 DB에 쌓고
		    	switch(t[0]){
		    		case '1':
		    			tmp.soil1 = t[1];
		    			break;
		        	case '2':
		        		tmp.soil2 = t[1];
		    			break;
		        	case '3':
		        		tmp.soil3 = t[1];
		    			break;
		        	case '4':
		        		if(t[1].includes('/')){
		        			var t_re = t[1].split('/');
		        			tmp.temp = t_re[0];
		        			tmp.humi = t_re[1];	
		        		}
			        	break;
		        	case '5':
		        		tmp.ph = t[1];
		    			break;
		        	case '6':
		        		tmp.weight = t[1];
		        		break;
		        	case '7':
		        		tmp.sun = t[1];
		        		break;
			        default:
			        	break;
			    }
		    }
		}
		sensors.push(tmp);
		// DB 저장하기 수행
		conf_status=0;	
	} 
	conf_status++;
}, 2000);

function normalize(data){
	var res = [];
	var spoon = [[],[],[],[],[],[],[],[]];
	for(var each in data){
		if(each.soil1){
			spoon[0].push(parseFloat(each.soil1));
		}if(each.soil2){
			spoon[1].push(parseFloat(each.soil2));
		}if(each.soil3){
			spoon[2].push(parseFloat(each.soil3));
		}

		if(each.temp){
			spoon[3].push(parseFloat(each.temp));
		}if(each.humi){
			spoon[4].push(parseFloat(each.humi));
		}

		if(each.ph){
			spoon[5].push(parseFloat(each.ph));
		}

		if(each.weight){
			spoon[6].push(parseFloat(each.weight));
		}

		if(each.sun){
			spoon[7].push(parseInt(each.sun));
		}
	}
	for(var i=0; i<7; i++){ // 조도 센서 제외
		if(spoon[i].length > 0)
			spoon[i] = array_normalize(spoon[i]);
	}
	// 조도센서 처리
	var ee = 0;
	for(var each in spoon[7]){
		if(each)
			ee += each;
	}

	if(ee >= (spoon[7].length/2) && spoon[7].length != 0){
		spoon[7] = [1];
	}else{
		spoon[7] = [0];
	}

	for(var l=0; l<(max_size/2); l++){
		var item = {};
		if(spoon[0].length > 0)
			item.soil1 = spoon[0][l];
		if(spoon[1].length > 0)
			item.soil2 = spoon[1][l];
		if(spoon[2].length > 0)
			item.soil3 = spoon[2][l];
		if(spoon[3].length > 0)
			item.temp = spoon[3][l];
		if(spoon[4].length > 0)
			item.humi = spoon[4][l];
		if(spoon[5].length > 0)
			item.ph = spoon[5][l];
		if(spoon[6].length > 0)
			item.weight = spoon[6][l];
		if(spoon[7].length > 0)
			item.sun = spoon[7][0];
		res.push(item);
	}


	return res;
}

function array_normalize(array){
	var arr = array;
	var devi;
	var half = parseInt(arr.length * 0.5);

	var sum;
	var aver;
	var indexList;
	while(half > 0){
		sum = 0;
		devi = [];
		for(var each in arr)
			sum += each;
		aver = sum / arr.length;
		for(var i=0; i<arr.length; i++)
			devi[i] = Math.abs(arr[i] - aver);

		if(half >= 10){
			indexList = sel_BigN(devi, 10);
			indexList.sort(function(a, b){return b - a});
			for(var l=0; l<indexList.length;l++){
				arr.splice(indexList[l], 1);
			}
			half -= 10;
			// find index 10 count

		}else{
			indexList = sel_BigN(devi, half);
			indexList.sort(function(a, b){return b - a});
			for(var l=0; l<indexList.length;l++){
				arr.splice(indexList[l], 1);
			}
			half = 0;
		}
	}
	for(var each in arr)
		sum += each;
	aver = sum / arr.length;
	for(var ll = arr.length; ll<(max_size/2); ll++){
		arr[ll] = aver;
	}


	// 평준화 작업
	return arr;
}

function sel_BigN(data, count){
	var index = Array.apply(null, new Array(count)).map(Number.prototype.valueOf,0);
	var cal;
	for(var i = 0; i < data.length; i++){
		for(var j = 0; j < count; j++){
			if(j == 0){
				if(data[i] > data[index[0]]){
					index[0] = i;
				}
			}else{
				if(data[index[j-1]] > data[index[j]]){
					cal = index[j];
					index[j] = index[j-1];
					index[j-1] = cal;
				}
			}
		}	
	}
	return index;
}

function callfunc(conf_status) { // 1 2 3 
	const fd = fs.openSync('/dev/ttyACM0', 'a', 66666);
	var length = fs.writeSync(fd, conf_status, null, null, null);

	serialObj.on('data', function(data){
		value = data;
	});
	if(length == 0){
		console.log("파일 전송 에러")
	}
}