var mongoose = require('mongoose');
require('mongoose-double')(mongoose);
var Schema = mongoose.Schema;
var SchemaTypes = mongoose.Schema.Types;

var sensorSchema = new Schema({
	time: String,
	soil1: { type : Number },
	soil2: { type : Number },
	soil3: { type : Number },
	temp: { type : Number },
	humi: { type : Number },
	ph: { type: SchemaTypes.Double },
	sun: { type : Number },
	section1: String,
	section2: String,
	section3: String
});

module.exports = mongoose.model('f_arduino', sensorSchema);
