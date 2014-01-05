Pebble.addEventListener("ready", function(e) {

	var intervalID = setInterval(function() {
		navigator.geolocation.getCurrentPosition(successCallback, errorCallback);
	}, 10000);

	navigator.geolocation.getCurrentPosition(successCallback, errorCallback);
});

function successCallback(position) {
	var latitude = position.coords.latitude;
	var longitude = position.coords.longitude;
	console.log(latitude);
	console.log(longitude);

	getPlacesData(latitude, longitude);
}

function getPlacesData(latitude, longitude) {
	var queryString = "https://api.foursquare.com/v2/venues/search?ll=" + latitude + "," + longitude + "&limit=1&&client_id=OF3EV2ITEIATHGZENKA4FN1T4MHGZHYMV2QBXEQNFQNKHDF1&client_secret=DJPPRVPLHP0KWRINZAVUPTNVM4H5EULQC40ZRQFPDHVW0P1S&v=20131016";
	console.log("queryString: " + queryString);

	var req = new XMLHttpRequest();
	req.open('GET', queryString, true);
	req.onload = function(e) {
		if (req.readyState == 4 && req.status == 200) {
			if (req.status == 200) {
				var response = JSON.parse(req.responseText);

				data = {
					"1": String(response.response.venues[0].name),
					"2": String(response.response.venues[0].location.address),
					"3": String(response.response.venues[0].specials.items[0].title)
				};
				Pebble.sendAppMessage(data, ackHandler, nackHandler)

			} else {
				console.log("Error");
			}
		}
	}
	req.send(null);
}

function errorCallback(error) {
	console.log("Error:");
	console.log(error);
}

function ackHandler(e) {
	console.log("Success:");
	console.log("Message sent to pebble");
}

function nackHandler(e) {
	console.log("Error sending message to pebble");
	console.log(JSON.stringify(e, null, 4));
}
