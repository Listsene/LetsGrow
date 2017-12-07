var mongoose = require('mongoose');
require('mongoose-double')(mongoose);
var Schema = mongoose.Schema;
var SchemaTypes = mongoose.Schema.Types;

var plantSchema = new Schema({
	sort: { type: Schema.Types.ObjectId, ref: 'Flower' },
	time: Date,
	soil: { type: SchemaTypes.Double },
	temp: { type: SchemaTypes.Double },
	humi: { type: SchemaTypes.Double },
	ph: { type: SchemaTypes.Double },
	sun: { type : Number }
});

module.exports = mongoose.model('Plant', plantSchema);
