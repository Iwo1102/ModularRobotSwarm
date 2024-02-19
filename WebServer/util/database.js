const mongodb = require('mongodb');
const MongoClient = mongodb.MongoClient;

let db = null

const mongoConnect = theCallback => {
    MongoClient.connect(
        'mongodb://localhost:27017/mrs'
    )
        .then(client => {
            console.log('Connected!');
            db = client.db();
            theCallback();
        })
        .catch(err => {
            console.log(err);
        });
};

const getDbClient = () =>  {
    if(db) {
    return db
    } else 
    throw 'No database exists'
}

exports.mongoConnect = mongoConnect
exports.getDbClient = getDbClient