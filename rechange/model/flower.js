var mongoose = require('mongoose');
require('mongoose-double')(mongoose);
var Schema = mongoose.Schema;
var SchemaTypes = mongoose.Schema.Types;

var flowerSchema = new Schema({
	Name: String,
	Leaf_number: { type : Number },
	Leaf_size : { type : Number },
	Is_flower : { type : Number },
	flower_color : String
});

module.exports = mongoose.model('f_flower', flowerSchema);
