var express = require('express');
var router = express.Router();
var f_flower = require('../model/flower');
var f_arduino = require('../model/sensor');
var f_standard = require('../model/plant');

router.get('/search', function(req,res){
	// var date = ; // hour, day, month (button value)
	// var sensor1 = ; // soil or temp button value
	// var sensor2 = ; // mois or ph button value
	// query1 : month and soil status. -> standard data
	dataSet = [{"year": "2017-08", "data": 1024, "fits": 800},
		   {"year": "2017-07", "data": 984, "fits": 800},
	     	   {"year": "2017-06", "data": 876, "fits": 805},
		   {"year": "2017-05", "data": 788, "fits": 877},
		   {"year": "2017-04", "data": 889, "fits": 900},
		   {"year": "2017-03", "data": 442, "fits": 884},
		   {"year": "2017-02", "data": 560, "fits": 854},
		   {"year": "2017-01", "data": 777, "fits": 800}];
	console.log("Section 1");
	f_standard.find({'sort' : 'Basil'})
		  //.where('time').gt('2017-06-01-06').lt('2017-11-30-23')
		  .where('time').in(['2017-06'])
		  .exec(function(err, data){
			if(err) console.log(err);
			console.log(data);
			res.render('stats', {'data':dataSet});
		});
/*	f_standard
			.find({'sort' : 'Basil'})
//			.find({'sort' : 'Basil', 'temp' : {$gt : 17.55, $lt : 17.57}})
			.limit(30)
			.where('time').in(['2017-06-01-11', '2017-06-02-11', '2017-06-03-11'])
//			.where('time').gt('2017-06-01-07').lt('2017-06-02-01')
//			.where('sun').in([1])
			.sort({'time' : 'asc'})
			.exec(function(err, data){
				if(err) console.log(err);
				console.log(data);
				res.render('stats', {'data':dataSet});
			});
*/
		//for(var each in data){
		//	console.log(data[each]);
		//	console.log(data[each].soil);
		//	console.log(data[each].time);
		//	console.log(data[each].PH);
		//}
	
	console.log(dataSet);

//	res.render('stats', {'data':dataSet});

})
router.get('/backup', function(req,res){
	dataSet = [
                {"year": "2017-08", "data": 1024, "fits": 800},
	        {"year": "2017-07", "data": 984, "fits": 800},
	        {"year": "2017-06", "data": 876, "fits": 805},
	        {"year": "2017-05", "data": 788, "fits": 877},
	        {"year": "2017-04", "data": 889, "fits": 900},
	        {"year": "2017-03", "data": 442, "fits": 884},
	        {"year": "2017-02", "data": 560, "fits": 854},
	        {"year": "2017-01", "data": 777, "fits": 800}
            ];
	res.render('stats_Backup', {'data':dataSet});
})


module.exports = router;
