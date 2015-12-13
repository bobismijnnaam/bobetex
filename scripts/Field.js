function Field() {
	this.patches = [];	
	this.width = 100;
	this.height = 100;

	this.groundWidth = 50;
	this.groundHeight = 50;

	for (var x = 0; x < this.width; x++) {
		var col = []
		for (var y = 0; y < this.height; y++) {
			var color = btx.uniform();
			if (color < 0.98) {
				col.push({type:0});
			} else {
				col.push({type:1});
			}
		}
		this.patches.push(col);
	}
};

Field.prototype.render = function(view) {
	var leftBound = Math.max(0, Math.floor(view.l / this.groundWidth));
	var rightBound = Math.min(this.width - 1, Math.ceil(view.r / this.groundWidth));

	var topBound = Math.min(this.height - 1, Math.ceil(view.t / this.groundHeight));
	var bottomBound = Math.max(0, Math.floor(view.b / this.groundHeight));

	for (var x = leftBound; x < rightBound; x++) {
		for (var y = bottomBound; y < topBound; y++) {
			var grassType = this.patches[x][y].type;

			if (grassType == 0) {
				btx.triangleColor(191, 225, 191);
			} else if (grassType == 1) {
				btx.triangleColor(237, 237, 234);
			} else if (grassType == 2) {
				btx.triangleColor(177, 177, 175);
			} else if (grassType == 3) {
				btx.triangleColor(120, 120, 118);
			}

			worldX = x * this.groundWidth;
			worldY = y * this.groundHeight;
			screenX = worldX - view.l;
			screenY = worldY - view.b;
			
			btx.rect(screenY + this.groundHeight, screenX + this.groundWidth, screenY, screenX);

			if (this.patches[x][y].pickedUp) {
				if (btx.getTicks() - this.patches[x][y].pickedUp >= RECOVER_TIME) {
					delete this.patches[x][y].pickedUp;
				} else {
					btx.triangleColor(180, 71, 39);
					var ds = 0.5 * this.groundHeight * ((btx.getTicks() - this.patches[x][y].pickedUp) / RECOVER_TIME);

					btx.rect(screenY + this.groundHeight - ds, screenX + this.groundWidth - ds, screenY + ds, screenX + ds);
				}
			} else if (this.patches[x][y].seed) {
				var dt = (btx.getTicks() - this.patches[x][y].seed.plantTime) / SEED_TIME;
				dt = Math.min(dt, 1.0);

				btx.triangleColor(lerp(180, 153, dt),
					   	lerp(71, 255, dt),
						lerp(39, 102, dt)
				);
				btx.rect(screenY + this.groundHeight, screenX + this.groundWidth, screenY, screenX);
			}
		}
	}
};

Field.prototype.setCollidable = function(x, y, collidable) {
	if (collidable) {
		if (!this.patches[x][y].bb) {
			this.patches[x][y].bb = {};
		}
	} else {
		if (this.patches[x][y].bb) {
			delete this.patches[x][y].bb;
		}
	}
}

Field.prototype.plant = function(x, y) {
	if (!this.patches[x][y].seed) {
		this.patches[x][y].seed = {
			plantTime: btx.getTicks()
		}
		delete this.patches[x][y].pickedUp;
	}
}

Field.prototype.takeGrownSeeds = function(possiblePatches) {
	var total = 0;

	for (var i = 0; i < possiblePatches.length; i++) {
		var x = possiblePatches[i][0];
		var y = possiblePatches[i][1];

		if (this.patches[x][y].seed) {
			if (btx.getTicks() - this.patches[x][y].seed.plantTime >= SEED_TIME) {
				total += 1;			
				delete this.patches[x][y].seed;
				this.patches[x][y].pickedUp = btx.getTicks();
			}
		}
	}

	return total;
}

