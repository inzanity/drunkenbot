Options = {}
Options.Particles = 6
Options.Coordinates = 20
Options.Colors = 3
Options.Sizes = 1
Options.Duration = 2000
Options.Looping = true
Options.Texture = "../particles/64x64_default.bmp"
initParticleSystem(Options)
size = 0.01
life = 2000
pos = {}
for i = 1, Options.Particles do
	for j = 1, Options.Coordinates do
		pos[j] = {}
		pos[j].x = math.cos(2*3.1415*(j - 1)/(Options.Coordinates - 1)) * size
		pos[j].y = 0
		pos[j].z = math.sin(2*3.1415*(j - 1)/(Options.Coordinates - 1)) * size
	end
	f = math.mod(4 * (i - 1) / Options.Particles, 1)
	color = {rgb(255, 0, 0),rgb(0, 0, 0),rgb(0, 0, 0)}
	setParticle(i, life, 0, pos, color)
end
sizes = {.02}
setDefaultSize(sizes)