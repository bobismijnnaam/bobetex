function main() {
	var counterVar = 0;
	var stateObj = {
		printAndInc : function() {
			print("Counter value is: " + counterVar + " (" + SomeGlobalTestVar + ")");
			counterVar += 1;
		}
	};

	return stateObj;
}
