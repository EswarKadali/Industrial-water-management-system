const admin = require("firebase-admin");
const serviceAccount = require("C:/Users/Eswar/Documents/sem5/ICPS_PROJECT/watermanagementsystem-92386-firebase-adminsdk-2en1c-90caa71a75.json"); // Replace with your own service account key file

// Initialize Firebase Admin SDK
admin.initializeApp({
  credential: admin.credential.cert(serviceAccount),
  databaseURL: "https://watermanagementsystem-92386-default-rtdb.asia-southeast1.firebasedatabase.app"
});


const db = admin.database(); // Use Realtime Database, change as needed

// Example function to get data from Firebase
function getDataFromFirebase() {
  const dataRef = db.ref("/"); // Replace with your database path
  dataRef.on("value", (snapshot) => {
    const data = snapshot.val();
    console.log("Data from Firebase:", data);
  });
}

getDataFromFirebase();
