function SimpleWand() {
	this.wand = true;
	this.recoverTime = 2000;
}

SimpleWand.prototype.getPatches = function(field, x, y) {
	if (!field.patches[x][y].seed)
		return [[x, y]];

	return [];
}

function Player() {
	this.w = 40;
	this.h = 40;
	this.x = 100;
	this.y = 100;
	this.vx = 0;
	this.vy = 0;

	this.up = false;
	this.down = false;
	this.left = false;
	this.right = false;

	this.speed = 100; // pixels per second

	this.wand = new SimpleWand();
	this.plantSomething = false;
	this.lastPlant = 0;
};

Player.prototype.render = function(view, debug) {
 	var screenX = this.x - view.l;
 	var screenY = this.y - view.b;

	btx.triangleColor(252, 208, 89);
	btx.rect(screenY + this.h / 2, screenX + this.w / 2, screenY - this.h / 2, screenX - this.w / 2);

	if (debug) {

	}
};

Player.prototype.event = function(e) {
	if (!e) return;

	if (e.key) {
		var dirs = ["left", "up", "right", "down"];

		if (dirs.indexOf(e.key.name) > -1) {
			this[e.key.name] = e.key.state == "pressed";
		} else if (e.key.name == "space" && e.key.state == "pressed") {
			this.plantSomething = true;
		}
	}
}

Player.prototype.logic = function(field) {
	this.vx = 0;
	this.vy = 0;
	if (this.up) {
		this.vy += 1;
	}
	if (this.right) {
		this.vx += 1;
	}
	if (this.down) {
		this.vy -= 1;
	}
	if (this.left) {
		this.vx -= 1;
	}

	// Normalize speed...
	var dist = Math.sqrt(this.vx * this.vx + this.vy * this.vy);
	if (dist) {
		this.vx /= dist;
		this.vy /= dist;
		this.vx *= this.speed;
		this.vy *= this.speed;
	}

	// Move x
	this.x += this.vx * dt;

	var bb = {
		t: this.y + this.h / 2,
		r: this.x + this.w / 2,
		b: this.y - this.h / 2,
		l: this.x - this.w / 2
	}

	// Keep bounding box in the field
	if (bb.l < 0) {
		bb.l = 0;
		bb.r = this.w;
	}
	if (bb.r >= (field.width - 1) * field.groundWidth) {
		bb.r = (field.width - 1) * field.groundWidth - 1;
		bb.l = bb.r - this.w;
	}

	var topRow = Math.floor(bb.t / field.groundHeight);
	var botRow = Math.floor(bb.b / field.groundHeight);
	var leftCol = Math.floor(bb.l / field.groundWidth);
	var rightCol = Math.floor(bb.r / field.groundWidth);

	if (this.vx > 0) {
		var tr = field.patches[rightCol][topRow].bb;
		var br = field.patches[rightCol][botRow].bb;

		if (tr || br) {
			bb.r = rightCol * field.groundWidth - 1;
			bb.l = bb.r - this.w;
		}
	} else if (this.vx < 0) {
		var tl = field.patches[leftCol][topRow].bb;
		var bl = field.patches[leftCol][botRow].bb;

		if (tl || bl) {
			bb.l = (leftCol + 1 ) * field.groundWidth;
			bb.r = bb.l + this.w;
		}
	}

	// Move y
	this.y += this.vy * dt;

	bb.t = this.y + this.h / 2;
	bb.b = this.y - this.h / 2;

	if (bb.b < 0) {
		bb.b = 0;
		bb.t = this.h;
	}
	if (bb.t >= (field.height - 1) * field.groundHeight) {
		bb.t = (field.height - 1) * field.groundHeight - 1;
		bb.b = bb.t - this.h;
	}

	topRow = Math.floor(bb.t / field.groundHeight);
	botRow = Math.floor(bb.b / field.groundHeight);
	leftCol = Math.floor(bb.l / field.groundWidth);
	rightCol = Math.floor(bb.r / field.groundWidth);
	
	if (this.vy > 0) {
		var tl = field.patches[leftCol][topRow].bb;
		var tr = field.patches[rightCol][topRow].bb;

		if (tl || tr) {
			bb.t = topRow * field.groundHeight - 1;
			bb.b = bb.t - this.h;
		}
	} else if (this.vy < 0) {
		var bl = field.patches[leftCol][botRow].bb;
		var br = field.patches[rightCol][botRow].bb;

		if (bl || br) {
			bb.b = topRow * field.groundHeight;
			bb.t = bb.b + this.h;
		}
	}

	this.x = bb.l * 0.5 + bb.r * 0.5;
	this.y = bb.b * 0.5 + bb.t * 0.5;

	topRow = Math.floor(bb.t / field.groundHeight);
	botRow = Math.floor(bb.b / field.groundHeight);
	leftCol = Math.floor(bb.l / field.groundWidth);
	rightCol = Math.floor(bb.r / field.groundWidth);

	// Now that moving should be done, let's see if we can plant something
	var patchX = Math.floor(this.x / field.groundWidth);
	var patchY = Math.floor(this.y / field.groundHeight);
	
	var timeSinceLastPlant = btx.getTicks() - this.lastPlant;

	if (this.plantSomething && timeSinceLastPlant >= this.wand.recoverTime) {
		var patches = this.wand.getPatches(field, patchX, patchY);
		
		for (var i = 0; i < patches.length; i++) {
			field.plant(patches[i][0], patches[i][1]);
		}

		this.lastPlant = btx.getTicks();
	}

	this.plantSomething = false;

	var possibleCells = [[leftCol, botRow], [leftCol, topRow], [rightCol, topRow], [rightCol, botRow]];
	var sugars = field.takeGrownSeeds(possibleCells);
}

