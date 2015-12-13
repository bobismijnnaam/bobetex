var progressColors = [
	[154, 51, 41],
	[154, 98, 41],
	[255, 170, 83],
	[28, 81, 96],
	[66, 163, 191],
	[31, 116, 50],
	[69, 214, 101]
];

function Hud() {

}

Hud.prototype.render = function(player, field) {
	var offsetTop = 30;
	var offsetLeft = offsetTop;
	var border = 10;
	var width = 100;
	var height = 30;

	btx.triangleColor(255, 255, 255);
	btx.rect(SCR_H - offsetTop,
			offsetLeft + border + width + border,
			SCR_H - offsetTop - border - height - border,
			offsetLeft);

	var df = (btx.getTicks() - player.wand.lastUse) / player.wand.recoverTime;
	df = Math.min(df, 1.0);
	df *= df * df;

	var r = lerp(120, 0, df);
	var g = lerp(0, 120, df);
	var b = 50;
	btx.triangleColor(r, g, b);

	btx.rect(SCR_H - offsetTop - border,
			offsetLeft + border + width * df,
			SCR_H - offsetTop - border - height,
			offsetLeft + border);

	var prevLvl;
	var lvl;

	if (player.sugars <= 1) {
		lvl = 0;
		df = player.sugars * 0.1;
	} else {
		var logVal = Math.log(player.sugars) / Math.log(10);
		lvl = Math.floor(logVal);
		df = logVal - lvl;
	}

	lvl = Math.min(6, lvl);
	if (lvl == 6) df = 1;
	
	prevLvl = lvl - 1;

	var offsetRight = offsetTop;
	var spacing = offsetTop
	width = SCR_W - offsetLeft - border - width - border - spacing - border - border - offsetRight;

	btx.triangleColor(255, 255, 255);
	btx.rect(SCR_H - offsetTop,
			SCR_W - offsetRight,
			SCR_H - offsetTop - border - height - border,
			SCR_W - offsetRight - border - width - border);

	if (prevLvl > -1) {
		var color = progressColors[prevLvl];
		btx.triangleColor.apply(null, color);
		btx.rect(SCR_H - offsetTop - border,
				SCR_W - offsetRight - border,
				SCR_H - offsetTop - border - height,
				SCR_W - offsetRight - border - width);
	}

	var color = progressColors[lvl];
	btx.triangleColor.apply(null, color);
	btx.rect(SCR_H - offsetTop - border,
			SCR_W - offsetRight - border - width + width * df,
			SCR_H - offsetTop - border - height,
			SCR_W - offsetRight - border - width);

};
