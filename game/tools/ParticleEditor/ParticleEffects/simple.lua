Options = {}
Options.Particles = 100
Options.Coordinates = 2
Options.Colors = 1
Options.Sizes = 1
Options.Texture = "../particles/32x32_default.bmp"
Options.Looping = true
initParticleSystem(Options)
size = 5
life = 2000
pos = {}
for i = 1, Options.Particles do
	pos[1] = {x = 0, y = 0, z = 0}
	pos[2] = {}
	pos[2].x = frand() * size
	pos[2].y = frand() * size
	pos[2].z = frand() * size
	setParticle(i, life, pos)
end
colors = {rgb(128, 10, 255)}
setDefaultColor(colors)
sizes = {.3}
setDefaultSize(sizes)