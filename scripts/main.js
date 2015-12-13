btx.include("sanity.js"); sanity();
btx.include("utils.js");
btx.include("Field.js");
btx.include("Player.js");

var SCR_W = 800;
var SCR_H = 600;
btx.createWindow("Sugarcane", SCR_W, SCR_H);

var playGame = true;
var mx = 0;
var my = 0;
var dt = 0;

var SEED_TIME = 15 * 1000;
var RECOVER_TIME = 1 * 1000;

print(btx.uniform());
print(btx.randrange(0, 5));

player = new Player();
field = new Field();

// Create grey landing plane
for (var x = 0; x < 4; x++) {
	for (var y = 3; y < 6; y++) {
		var type = -1;
		if (y % 2 == 1) {
			type = 2;
		} else {
			type = 3;
		}
		field.patches[x][y].type = type;
		field.setCollidable(x, y, true);
	}
}

field.patches[4][4].type = 2;
field.setCollidable(4, 4, true);

var view = {
	l : 0,
	t : 600,
	r : 800,
	b : 0
};

btx.setClearColor(162, 215, 216);

var previousFrame = btx.getTicks();
while(playGame) {
	var currentFrame = btx.getTicks();
	dt = (currentFrame - previousFrame) / 1000.0;
	previousFrame = currentFrame;

	var e;
	while (e = btx.pollEvent()) {
		if (e.quit) {
			playGame = false;
		} else if (e.key) {
			if (e.key.name == "escape") {
				playGame = false;
			}
		} else if (e.motion) {
			mx = e.motion.x;
			my = e.motion.y;
		}

		player.event(e)
	}

	player.logic(field);

	view = adjustViewToPlayer(view, player, 0.1);

	field.render(view);
	player.render(view);

	btx.flip();
}
