print("Running main.js");

btx.sanity();

btx.createWindow("DonkeyTurd 2.0", 800, 600);

while(true) {
	var e = btx.pollEvent();

	if (e) {
		print(JSON.stringify(e));

		if (e.quit) {
			break;
		}
	}
}

// function main() {
// 	var counterVar = 0;
// 	var stateObj = {
// 		printAndInc : function() {
// 			print("Counter value is: " + counterVar + " (" + SomeGlobalTestVar + ")");
// 			counterVar += 1;
// 		}
// 	};
// 
// 	return stateObj;
// }
