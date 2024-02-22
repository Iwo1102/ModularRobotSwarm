const express = require('express');
const mongoose = require('mongoose');
const bodyParser = require('body-parser');

const app = express();

// Connect to MongoDB
mongoose.connect('mongodb://localhost:27017/MRS', {
}).then(() => {
	console.log('Connected to MongoDB');
}).catch((error) => {
	console.error('MongoDB connection error:', error);
});

// Create a simple schema and model (replace with your own schema)
const robotSchema = new mongoose.Schema({
	id: Number,
	name: String,
	coords: [Number],
	available: Boolean,
	orders: [Number]

});
const beaconSchema = new mongoose.Schema({
	id: Number,
	name: String,
	distance: Number,
	available: Boolean,

});

const Robot = mongoose.model('Robots', robotSchema);
const Beacon = mongoose.model('Beacons', beaconSchema);


// Middleware to parse JSON in requests
app.use(bodyParser.json());

// Routes
/*app.post('/users', async (req, res) => {
try {
	const { name, email } = req.body;
	const newUser = new User({ name, email });
	const savedUser = await newUser.save();
	res.json(savedUser);
} catch (error) {
	res.status(500).json({ error: error.message });
}
});*/

app.post('/findCell', async (req, res) => {
	let num;
	try {
		const robots = await Robot.find({available: { $eq: 1 }});
		res.json(robots);
		num = robots.length;
	} catch (error) {
		res.status(500).json({ error: error.message });
	}
	console.log(num)
	if(num == 0) {
		try {
			console.log("Available cells not found, creating new document")
			let setId = (await Robot.find()).length;
			await Robot.create({
								id: setId,
								name: req.body.name, 
								coords: [req.body.coords[0], req.body.coords[1]], 
								available: 0, 
								orders: [0]
							});
		} catch (error) {
			res.status(500).json({ error: error.message });
		}
	} else {
		try {
			console.log("Cells found, editing document")
			await Robot.findOneAndUpdate({available: {$eq: 1}}, {
											name: req.body.name,
											available: 0,
											coords: [req.body.coords[0], req.body.coords[1]],
											orders: [0]
										});
		} catch (error) {
			res.status(500).json({ error: error.message });
		}
	}
});

app.get('/TestConnection', (req, res) => {
	res.json(1);
})

app.get('/', (req, res) => {
	res.send("egg");
})

// Start the server
app.listen(3000, () => {
	console.log(`Server is running on http://localhost:3000`);
});
