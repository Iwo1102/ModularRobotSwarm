const express = require('express');
const { CancellationToken } = require('mongodb');
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
  let number;
  const cells =db.collection('robots').count({}, (err, num) => {
    if(err) return callback(err);
  }).then((err, number) =>{
    if (err) {
      return console.log(err.message)
    }
  }).catch((num) => {
    number = num
  })

  console.log(number)

  

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
