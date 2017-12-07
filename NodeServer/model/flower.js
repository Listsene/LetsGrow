var mongoose = require('mongoose');
require('mongoose-double')(mongoose);
var Schema = mongoose.Schema;
var SchemaTypes = mongoose.Schema.Types;

var flowerSchema = new Schema({
	name: String,
	
});

module.exports = mongoose.model('Flower', flowerSchema);
