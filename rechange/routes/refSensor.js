var flower = require('../model/flower');
var plant = require('../model/plant');
var sensor = require('../model/sensor');
function rank_soil(val){
	if(val > 950)
		return 1;
	else if(val > 850)
		return 2;
	else if(val > 750)
		return 3;
	else if(val > 650)
		return 4;
	else if(val > 550)
		return 5;
	else if(val > 450)
		return 6;
	else if(val > 400)
		return 7;
	else if(val > 350)
		return 8;
	else if(val > 300)
		return 9;
	else
		return 10;
}
var o = 0;
module.exports = function(app){
	app.get('/ref', function(req,res){
		// 0~8 까지 숫자 전달 <- 결과물 Busled 발광
		o++;
		console.log(app.sensorData)
		console.log(app.sections)
		app.sections.section1 = o;
		res.send('3'); 
	});

	app.get('/busled_sun', function(req,res){
	// 0~8 까지 숫자 전달 <- 결과물 Busled 발광
		
	});

	app.get('/dot_soil/:id', function(req,res){
		var id = req.params.id;
		if(id > 3){
			res.send(id);
		}
		var len = data.length;
		if(len > 0){
			while(len >= 1){
				if(data[len-1]['soil'+id]){
					res.send(rank_soil(data[len-1]['soil'+id]));
				}
				len--;
			}

		} // 1024 멕스 900 촉촉 -> 290까지 떨어짐
		// DB에서 id 넘겨주고 상태 물어보기
		// 0~10 까지 숫자 전달 <- 결과물 dot matrix에 1~9 하트를 출력함으로써 각 화분 토양 상태 파악
		res.send(0);
	});

	app.get('/oled_condition', function(req,res){
		
		res.send('70'); // 1 ~ 100 까지 숫자 전달 <- oled에 전체적 '식물'과 '환경'의 적합도 이미지로 출력
	});

	app.get('/tlcd_water', function(req,res){
		var max_weight = 2000;
		var len = data.length;
		if(len > 0){
			while(len >= 1){
				if(data[len-1]['weight']){

					res.send(parseInt(parseInt(data[len-1]['weight']) * 100 / max_weight) + "%");
				}
				len--;
			}

		}
		res.send('OUT'); // 'xx%' 형태로 전송 <- 물이 차있는 정도를 출력한다.
	});

	app.get('/tlcd_default', function(req,res){
		var len = data.length;
		var temp = 0;
		var humi = 0;
		if(len > 0){
			while(len >= 1){
				if(data[len-1]['temp']){
					temp = parseInt(data[len-1]['temp']);
					break;
				}
				len--;
			}

			len = data.length;
			while(len >= 1){
				if(data[len-1]['humi']){
					humi = parseInt(data[len-1]['humi']);
					break;
				}
				len--;
			}			
		}
		res.send(temp + 'C/' + humi + '%'); // '18C/xx%' <- 온도와 습도 출력
	});

	// app.get('/dot_soil/:id', function(req,res){
	// 	var id = req.params.id;
	// 	// DB에서 id 넘겨주고 상태 물어보기
	// 	// 0~10 까지 숫자 전달 <- 결과물 dot matrix에 1~9 하트를 출력함으로써 각 화분 토양 상태 파악
	// 	res.send(id);
	// });

	// app.get('/oled_condition', function(req,res){

	// 	res.send('70'); // 1 ~ 100 까지 숫자 전달 <- oled에 전체적 '식물'과 '환경'의 적합도 이미지로 출력
	// });

	// app.get('/tlcd_water', function(req,res){
		
	// 	res.send('40%'); // 'xx%' 형태로 전송 <- 물이 차있는 정도를 출력한다.
	// });

	// app.get('/tlcd_default', function(req,res){
	// 	res.send('18C/42%'); // '18C/xx%' <- 온도와 습도 출력
	// });
 // request 전달 주시적으로 살아있는지 확인
}


