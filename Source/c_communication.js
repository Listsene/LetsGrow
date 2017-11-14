var express = require('express');
var app = express();

if (app.get('env') === 'development') {
  app.locals.pretty = true;		//html 소스 pretty 하게 만들기
}

app.set('view engine', 'jade');		//jade 사용(template)
app.set('views', './views');		//jade 디렉토리

var bodyParser = require('body-parser');			//post를 위해서 body-parser사용
app.use(bodyParser.urlencoded({extended:false}));	//post를 위해서 body-parser사용

app.get('/', function(req,res){
	res.send("hello");
});

app.get('/rcv', function(req,res){
	var a = {
		name : "crazy",
		change : 3,
		date : new Date()
	}
	res.send(a);
});

app.listen(3000, () => {
	console.log('3000 port Server Started');
});


