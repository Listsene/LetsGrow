var express = require('express');
var router = express.Router();


router.get('/search', function(req,res){
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
	res.render('stats', {'data':dataSet});
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
