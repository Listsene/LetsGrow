var mongoose = require('mongoose');
require('mongoose-double')(mongoose);
var Schema = mongoose.Schema;
var SchemaTypes = mongoose.Schema.Types;

var sensorSchema = new Schema({
	time: { type: Date, default : Date.now },
	soil1: { type: SchemaTypes.Double },
	// soil2: { type: SchemaTypes.Double },
	// soil3: { type: SchemaTypes.Double },
	
	section1: { type: Schema.Types.ObjectId, ref: 'Flower' },
	// section2: { type: Schema.Types.ObjectId, ref: 'Flower' },
	// section3: { type: Schema.Types.ObjectId, ref: 'Flower' },

	temp: { type: SchemaTypes.Double },
	humi: { type: SchemaTypes.Double },
	ph: { type: SchemaTypes.Double },
	sun: { type : Number }
});

module.exports = mongoose.model('Sensor', sensorSchema);
