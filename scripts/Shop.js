function Shop() {
	this.isVisible = false;
	this.exitShop = true;
	this.exitButtonSize = 50;

	this.mx = -1;
	this.my = -1;
	this.mouseReleased = false;

	this.products = [ {}, {}, {}, {} ];

	// move speed
	var mov = this.products[0];
	mov.cost = 10;
	mov.acquired = 0;
	mov.max = 4;
	mov.buy = function(player, field) {
		if (mov.acquired == mov.max || player.sugars < mov.cost)
			return false;

		player.sugars -= mov.cost;
		mov.cost += 50;
		mov.acquired += 1;
		player.speed += 20;

		return true; 
	}

	// grow speed
	var speed = this.products[1];
	speed.cost = 100;
	speed.acquired = 0;
	speed.max = 4;
	speed.buy = function(player, field) {
		if (speed.acquired == speed.max || player.sugars < speed.cost) {
			return false;
		}
		
		player.sugars -= speed.cost;
		speed.cost += 200;
		speed.acquired += 1;
		SEED_TIME -= 3 * 1000;

		return true;
	}

	// planting behaviour
	var plant = this.products[2];
	plant.cost = 1000;
	plant.acquired = 0;
	plant.max = 4;
	plant.buy = function(player, field) {
		if (plant.acquired == 0 && plant.cost <= player.sugars) {
			player.wand = new CrossWand();
			plant.acquired += 1;
			plant.cost += 1000;
			return true;
		}

		return false;
	}

	// statics
	var stat = this.products[3];
	stat.cost = 10000;
	stat.acquired = 0;
	stat.max = 100;
	stat.buy = function(player, field) {
		// Give player a thing to put down
		// Or spawn it randomly
		// Also, only raise the cost after 5 have been bought
		
		return false;
	}
}

Shop.prototype.event = function(e) {
	if (this.isVisible) {
		if (e.button) {
			if (e.button.button == "left" && e.button.state == "released") {
				var x = e.button.x;
				var y = e.button.y;
				if (x > SCR_W - this.exitButtonSize && y > SCR_H - this.exitButtonSize) {
					this.exitShop = true;
					print("Exit the shop!");
				}

				this.mouseReleased = true;
			}
		} else if (e.motion) {
			this.mx = e.motion.x;
			this.my = e.motion.y;
		} else if (e.button) {
		}
	}
}

Shop.prototype.logic = function(player, field) {
	if (this.exitShop) {
		this.exitShop = false;
		player.unfreeze();
		this.isVisible = false;
	}

	if (player.hasCollided) {
		this.isVisible = true;
		player.freeze();
	}

	// Layout settings
	var slotS = 150;
	var hSpace = 40;
	var vSpace = 25
	var vPad = 225;
	var buttonH = 75;
	var vTrans = 100;

	for (var i = 0; i < 4; i++) {
		var left = (hSpace + slotS) * i + 40;
		var bot = 100;

		if (this.mx >= left && this.mx < left + slotS) {
			if (this.my >= bot && this.my < bot + buttonH) {
				if (this.mouseReleased) {
					print("Something is going to be bought!");
					if (this.products[i].buy(player, field)) {
						// TODO: Maybe flash the logo or something?
						print(i + " was bought!");
					}
				}
			}
		}
	}
	
	this.mouseReleased = false;
}

Shop.prototype.render = function() {
	if (!this.isVisible) return;

	btx.triangleColor(177, 177, 175);
	btx.rect(SCR_H, SCR_W, 0, 0);

	btx.triangleColor.apply(null, progressColors[0]);
	btx.rect(SCR_H, SCR_W, SCR_H - this.exitButtonSize, SCR_W - this.exitButtonSize);

	// Render the buy slots
	var slotS = 150;
	var hSpace = 40;
	var vSpace = 25
	var vPad = 225;
	var buttonH = 75;
	var vTrans = 100;

	var drawSugar = function(x, y) {
		var s = 5;
		var w = 5;

		var left = x - s * 1.5 - w;
		var bot = y - s * 1.5 - w;

		btx.triangleColor(255, 255, 255);

		for (var i = 0; i < 3; i++) {
			for (var j = 0; j < 3; j++) {
				var inLeft = left + (s + w) * i;
				var inBot = bot + (s + w) * j;

				btx.rect(inBot + s, inLeft + s, inBot, inLeft);
			}
		}			

		left = x - 2.5 * s - 2 * w;
		bot = y - 0.5 * s;
		btx.rect(bot + s, left + s, bot, left);

		left = x - 0.5 * s;
		bot = y + 1.5 * s + 2 * w;
		btx.rect(bot + s, left + s, bot, left);

		left = x + 1.5 * s + 2 * w;
		bot = y - 0.5 * s;
		btx.rect(bot + s, left + s, bot, left);

		left = x - 0.5 * s;
		bot = y - 2.5 * s - 2 * w;
		btx.rect(bot + s, left + s, bot, left);

	}

	for (var i = 0; i < 4; i++) {
		btx.triangleColor(200, 200, 200);
		var left = (hSpace + slotS) * i + 40;
		
		btx.rect(SCR_H - vPad + vTrans, left + slotS, vPad + vTrans, left);
		
		var bot = 100;

		if (this.mx >= left && this.mx < left + slotS) {
			if (this.my >= bot && this.my < bot + buttonH) {
				btx.triangleColor(225, 225, 225);
			}
		}

		btx.rect(bot + buttonH, left + slotS, bot, left);
		drawSugar(left + slotS * 0.5, bot + buttonH * 0.5);

		bot += 75 + 25;

		btx.triangleColor(255, 255, 255);
		btx.rect(bot + buttonH, left + slotS, bot, left);

		var border = 10;
		var df = (this.products[i].acquired * 1.0) / this.products[i].max;

		btx.triangleColor.apply(null, progressColors[5]);
		btx.rect(bot + buttonH - border, left + border + (slotS - 2 * border) * df,
				bot + border, left + border);
	}

	// "Statics"
	// Buy better wand
	// Speed improvement
	// Grow time improvement

	// Draw speed symbol
	var left = (hSpace + slotS) * 0 + 40 + 20;
	var top = SCR_H - vPad + vTrans - 20;

	var h = 10;
	var spacing = (110 - 3 * h) / 2;
	var whitespace = 50;

	btx.triangleColor(255, 255, 255);
	for (var i = 0; i < 3; i++) {
		var inTop = top - i * (h + spacing);
		var inLeft = left + i * (whitespace / 2);	

		btx.rect(inTop, inLeft + (110 - whitespace), inTop - h, inLeft);
	}

	// Draw grow time symbol
	left = (hSpace + slotS) * 1 + 40;
	top = SCR_H - vPad + vTrans;
	left += 20; 
	top -= 20;

	var right = left + 110;
	var bot = top - 110;

	btx.triangleColor(255, 255, 255);

	btx.quad(left, (top + bot) / 2,
			(left + right) / 2, top,
			right, (top + bot) / 2,
			(left + right) / 2, bot);

	btx.triangleColor(200, 200, 200);
	btx.rect(top - 20, left + 110 - 53, (top + bot) / 2, left + 53);

	// Draw planting symbol
	var left = (hSpace + slotS) * 2 + 40;
	var top = SCR_H - vPad + vTrans;
	
	btx.triangleColor(255, 255, 255);
	for (var i = 0; i < 3; i++) {
		btx.rect(top - 20,
				left + i * (30 + 10) + 30 + 10,
				top - 20 - 80,
				left + i * (30 + 10) + 30);
	}

	btx.rect(top - 20 - 80 - 20,
			left + 3 * (30 + 10),
			top - 20 - 80 - 20 - 10,
			left + 30);

	// Draw Block symbol
	left = (hSpace + slotS) * 3 + 40;
	top = SCR_H - vPad + vTrans - 30;

	btx.triangleColor(255, 255, 255);
	btx.rect(top - 50, left + slotS - 30, top - 50 - 50, left + 30);
	btx.triangleColor(200, 200, 200);
	var border = 12;
	btx.rect(top - 50 - border, left + slotS - 30 - border, top - 50 - 50 + border, left + 30 + border);

}
