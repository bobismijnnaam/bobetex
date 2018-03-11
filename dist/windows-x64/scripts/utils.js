// Error addon
Error.prototype.toString = function () {
	var line;
	print('Error.prototype.toString() called:', typeof this);

	line = (this || {}).lineNumber;
	print('Linenumber type is:', typeof line);

	// A useful replacement could be something like this
	return this.name + ': ' + this.message + ' (at line ' + this.lineNumber + ')';

	//return this.name + ': ' + this.message;
};

function adjustViewToPlayer(view, player, c) {
	var idealView = {
		l: player.x - SCR_W / 2,
		r: player.x + SCR_W / 2,
		t: player.y + SCR_H / 2,
		b: player.y - SCR_H / 2
	};

	var newView = {
		l: c * view.l + (1 - c) * idealView.l,
		r: c * view.r + (1 - c) * idealView.r,
		t: c * view.t + (1 - c) * idealView.t,
		b: c * view.b + (1 - c) * idealView.b,
	};

	newView = {
		t: view.t + (newView.t - view.t) * dt,
		r: view.r + (newView.r - view.r) * dt,
		b: view.b + (newView.b - view.b) * dt,
		l: view.l + (newView.l - view.l) * dt
	};

	return newView;
}

function lerp(a, b, c) {
	return a * (1 - c) + b * c;
}
