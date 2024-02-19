const express = require('express');
const router = express.Router();
const getDbClient = require('../util/database').getDbClient;

const robots = [];

router.get('/testConnection', (req, res, next) => {
  console.log('test Successful')
  res.send('<h1>test Successful</h1>')
})

router.get('/findCell', (req, res, next) => {
  const db = getDbClient()
  const collection = db.collection('robots')
  const name = req.query.name;
  let cells = db.collection('robots').find({}).toArray(function(err, result) {
    if (err) throw err;
  }).then(function(result){
    console.log(result)})
  //console.log(cells.JsonLength());
 /* if (!) {
    console.log("No Documents found");
    collection.insertOne({id: 0, name: name, available: 1, coords: [0, 0], orders: []})
    console.log(JSON.stringify(robots))
  } else {
    
    collection.findOneAndUpdate()
    console.log("Documents Found")
  }*/
  res.send('<h1>test</h1>')
}
)

exports.routes = router;
exports.robots = robots;
