Options = {}
Options.Particles = 150
Options.Coordinates = 2
Options.Colors = 1
Options.Sizes = 1
Options.Duration = 2000
Options.Looping = true
Options.Texture = "../particles/64x64_default.bmp"
initParticleSystem(Options)
size = 5
life = 2000
pos = {}
for i = 1, Options.Particles do
	pos[1] = {x = 0, y = 0, z = 0}
	pos[2] = {}
	pos[2].x = math.random(-size, size)
	pos[2].y = math.random(-size, size)
	pos[2].z = math.random(-size, size)
	color = {rgb(math.random(0, 255), math.random(0, 255), math.random(0, 255))}
	setParticle(i, life, 0, pos, color)
end
sizes = {.3}
setDefaultSize(sizes)