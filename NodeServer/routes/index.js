var express = require('express');
var router = express.Router();
// router.get('/busled_sun', function(req,res){
// 	// 0~8 까지 숫자 전달 <- 결과물 Busled 발광
// 	res.send('3'); 
// });

// router.get('/dot_soil/:id', function(req,res){
// 	var id = req.params.id;
// 	// DB에서 id 넘겨주고 상태 물어보기
// 	// 0~10 까지 숫자 전달 <- 결과물 dot matrix에 1~9 하트를 출력함으로써 각 화분 토양 상태 파악
// 	res.send(id);
// });

// router.get('/oled_condition', function(req,res){

// 	res.send('70'); // 1 ~ 100 까지 숫자 전달 <- oled에 전체적 '식물'과 '환경'의 적합도 이미지로 출력
// });

// router.get('/tlcd_water', function(req,res){
	
// 	res.send('40%'); // 'xx%' 형태로 전송 <- 물이 차있는 정도를 출력한다.
// });

// router.get('/tlcd_default', function(req,res){
// 	res.send('18C/42%'); // '18C/xx%' <- 온도와 습도 출력
// });

 // request 전달 주시적으로 살아있는지 확인

module.exports = router;
