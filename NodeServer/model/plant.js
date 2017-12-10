var mongoose = require('mongoose');
require('mongoose-double')(mongoose);
var Schema = mongoose.Schema;
var SchemaTypes = mongoose.Schema.Types;

var plantSchema = new Schema({
	sort: String,
	time: String,
	soil: { type : Number },
	temp: { type: SchemaTypes.Double },
	humi: { type: SchemaTypes.Double },
	PH: { type: SchemaTypes.Double },
	sun: { type : Number }
});

module.exports = mongoose.model('f_standard', plantSchema);
