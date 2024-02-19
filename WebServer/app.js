const path = require('path');
const mongoConnect = require('./util/database').mongoConnect

const express = require('express');
const bodyParser = require('body-parser');

const app = express();

// const adminData = require('./routes/admin');
const robotRoutes = require('./routes/robot');

app.use(bodyParser.urlencoded({ extended: false }));
app.use(express.static(path.join(__dirname, 'public')));

// app.use('/admin', adminData.routes);
app.use('/', robotRoutes.routes);

app.use((req, res, next) => {
    //  res.status(404).render('404', { pageTitle: 'Page Not Found' });
});

mongoConnect(() => {
    app.listen(3000);
});
