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
	orders: [{code: Number, sent: Boolean}],
	lastUpdate: Number

});
const beaconSchema = new mongoose.Schema({
	id: Number,
	name: String,
	distance: Number,
	available: Boolean,
	lastUpdate: Number

});

const Robot = mongoose.model('Robots', robotSchema);
const Beacon = mongoose.model('Beacons', beaconSchema);


// Middleware to parse JSON in requests
app.use(bodyParser.json());

//Check if still connected
setInterval(async () => {
	try {
		let robots = await Robot.find({available: {$eq: 0}});
		let beacons = await Beacon.find({available: {$eq: 0}});

		robots.map(async robot => {
			if ((Date.now() - robot.lastUpdate) > 10000) {
				console.log("Lost connection with robot " + robot.id);
				await Robot.findOneAndUpdate({id: { $eq: robot.id}}, {available: 1})
			}
		})
		beacons.map(async beacon => {
			if ((Date.now() - beacon.lastUpdate) > 10000) {
				console.log("Lost connection beacon " + beacon.id);
				await Beacon.findOneAndUpdate({id: { $eq: beacon.id}}, {available: 1})
			}
		})
	} catch (error) {
		console.log("error 500: " + error.message)
		console.log(error.message);
	}
}, 5000)


//Robot Routes
app.post('/findCell', async (req, res) => {
	let num;
	try {
		const robots = await Robot.find({available: { $eq: 1 }});
		res.json(robots);
		num = robots.length;
	} catch (error) {
		res.status(500).json({ error: error.message });
		console.log(error.message);
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
								orders: [],
								lastUpdate: Date.now()
							});
		} catch (error) {
			res.status(500).json({ error: error.message });
			console.log(error.message);
		}
	} else {
		try {
			console.log("Cells found, editing document")
			await Robot.findOneAndUpdate({available: {$eq: 1}}, {
											name: req.body.name,
											available: 0,
											coords: [req.body.coords[0], req.body.coords[1]],
											orders: [0],
											lastUpdate: Date.now()
										});
		} catch (error) {
			res.status(500).json({ error: error.message });
			console.log(error.message);
		}
	}
});

app.get('/TestConnection', async (req, res) => {
	let callerId = req.query.id
	let type = req.query.type

	try {
		if (type == 0) {
			console.log("Updating lastUpdate of robot " + callerId)
			let robot = await Robot.find({id: {$eq: callerId}})
			if (robot.available == 1) {
				console.log("Lost Connection with robot " + callerId)
				res.json(0)
			} else {
				res.json(1)
				await Robot.findOneAndUpdate({id: {$eq: callerId}}, {lastUpdate: Date.now()})
			}
		} else if (type == 1) {
			console.log("Updating lastUpdate of beacon " + callerId)
			let beacon = await Beacon.find({id: {$eq: callerId}})
			if (beacon.available == 1) {
				console.log("Lost Connection with beacon " + callerId)
				res.json(0)
			} else {
				await Beacon.findOneAndUpdate({id: {$eq: callerId}}, {lastUpdate: Date.now()})
				res.json(1)
			}
		}
		
	} catch (error){
		res.status(500).json({ error: error.message });
		console.log(error.message);
	}
})

app.get('/getId', async (req, res) => {
	let callerName = req.query.name;
	let callerType = req.query.type;
	try {
		if (callerType == 0)  {
			console.log("Getting id of Robot " + callerName)
			let robot = await Robot.find({name: {$eq: req.query.name}});
			console.log(robot)
			res.json(robot[0].id)
		} else if (callerType == 1) {
			console.log("Getting id of Beacon " + callerName)
			let beacon = await Beacon.find({name: {$eq: req.query.name}});
			res.json(beacon[0].id)
		}
	} catch (error){
		res.status(500).json({ error: error.message });
		console.log(error.message);
	}
});


app.post('/updateLocation',  async (req, res) => {
	let callerId = req.body.id;
	try {
		console.log("Updating position of robot " + callerId)
		await Robot.findOneAndUpdate({id: {$eq: callerId}}, {coords: [req.body.coords[0], req.body.coords[1]], lastUpdate: Date.now()})
	} catch (error) {
		res.status(500).json({ error: error.message });
		console.log(error.message);
	}
})

app.get('/findOthers', async (req, res) => {
	let callerId = req.query.id

	try{
		console.log("Finding other robots for robot " + callerId)
		await Robot.findOneAndUpdate({id: {$eq: callerId}}, {lastUpdate: Date.now()})
		let robots = await Robot.find({available: {$eq: 0}, id: {$ne: callerId}})
		res.json(robots)

	} catch (error) {
		res.status(500).json({ error: error.message });
		console.log(error.message);
	}
})

app.get('/getDistance', async (req, res) => {
	try {
		let beacon = await Beacon.findOne({id: 0})
		res.json(beacon.distance)
		console.log("distance: " + beacon.distance)
	} catch (error) {
		res.status(500).json({ error: error.message });
		console.log(error.message);
	}
})


//Beacon Routes
app.post('/findBeaconCell', async (req, res) => {
	let num;
	try {
		const beacons = await Beacon.find({available: { $eq: 1 }});
		res.json(beacons);
		num = beacons.length;
	} catch (error) {
		res.status(500).json({ error: error.message });
		console.log(error.message);
	}
	console.log(num)
	if(num == 0) {
		try {
			console.log("Available beacons not found, creating new document")
			let setId = (await Beacon.find()).length;
			await Beacon.create({
								id: setId,
								name: req.body.name, 
								available: 0,
								distance: req.body.distance,
								lastUpdate: Date.now()
							});
		} catch (error) {
			res.status(500).json({ error: error.message });
			console.log(error.message);
		}
	} else {
		try {
			console.log("Beacons found, editing document")
			await Beacon.findOneAndUpdate({available: {$eq: 1}}, {
											name: req.body.name,
											available: 0,
											distance: req.body.distance,
											lastUpdate: Date.now()
										});
		} catch (error) {
			res.status(500).json({ error: error.message });
			console.log(error.message);
		}
	}
});

app.post('/updateDistance', async (req, res) => {
	let callerId = req.body.id;
	let callerDistance = req.body.distance;
	try {
		if (callerDistance != 0) {
			console.log("updating distance of beacon id " + callerId);
			await Beacon.findOneAndUpdate({id: {$eq: callerId}}, { distance: callerDistance, lastUpdate: Date.now() });
			console.log("caller distance: " + callerDistance)
		}
	} catch (error) {
		res.status(500).json({ error: error.message });
		console.log(error.message);
	}
});

app.post('/sendOrder', async (req, res) => {
	let callerName = req.body.name;
	let callerOrders = req.body.orders
	console.log("Caller Orders length:", callerOrders.length);
	try {
		let robot = await Robot.findOne({name: {$eq: callerName}})
		console.log("robot name: " + robot.name + " available: " + robot.available)
		if (robot != null) {
			if (!robot.available) {
				for (let i = 0; i < callerOrders.length; i++) {
					await Robot.findOneAndUpdate({name: {$eq: callerName}}, {$push: {orders: {code: callerOrders[i], sent: 0}}});
					console.log("Orders sent")
				}
				res.status(200).json({ message: "Orders sent successfully" });
			} else {
				console.log("Robot Not Available")
				res.status(400).json({ message: "Robot Not Available" });
			}
		} else {
			console.log("Robot Not found");
			res.status(404).json({ message: "Robot Not Found" });
		}
	} catch (error) {
		res.status(500).json({ error: error.message });
		console.log(error.message);
	}
});

app.get('/getOrders', async (req, res) => {
	try {
		let callerId = req.query.id;
		let robot = await Robot.findOne({id: {$eq: callerId}})
		await Robot.findOneAndUpdate({id: {$eq: callerId}}, {lastUpdate: Date.now()})
		let orders = [];
		for (let i = 0; i < robot.orders.length; i++) {
			if (!robot.orders[i].sent) {
				console.log(robot.orders[i].code)
				orders.push(robot.orders[i].code);
				await Robot.findOneAndUpdate({id: {$eq: callerId}}, {lastUpdate: Date.now(),  $set: {[`orders.${i}.sent`]: true}})		
			} else {
				orders.push(0);
			}
		}

		console.log("Orders sent:" + orders);
		res.send(orders)
	} catch (error) {
		res.status(500).json({ error: error.message });
		console.log(error.message);
	}
});

app.post('/completeOrder', async (req, res) => {
	try {
        let callerId = req.body.id;
        let robot = await Robot.findOne({ id: callerId });
        if (robot) {
            await Robot.findOneAndUpdate( {id: callerId}, {$pop: {orders: -1}, $set: {lastUpdate: Date.now()}});
            res.send(robot);
        } else {
            res.status(404).json({ message: "Robot Not Found" });
        }
    } catch (error) {
        res.status(500).json({ error: error.message });
        console.log(error.message);
    }
});


app.get('/', (req, res) => {
	console.log("egg");
	res.json("egg");
})

// Start the server
app.listen(3000, () => {
	console.log(`Server is running on http://localhost:3000`);
});
