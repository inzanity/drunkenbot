Options = {}
Options.Particles = 5
Options.Coordinates = 2
Options.Colors = 4
Options.Sizes = 1
Options.Duration = 4000
Options.Looping = true
Options.Texture = "../particles/64x64_default.bmp"
initParticleSystem(Options)
size = 0
life = 1000
pos = {}
for i = 1, Options.Particles do
	for j = 1, Options.Coordinates do
		pos[j] = {}
		pos[j].x = math.cos(2*3.1415*(j - 1)/(Options.Coordinates - 1)) * size
		pos[j].y = 0
		pos[j].z = math.sin(2*3.1415*(j - 1)/(Options.Coordinates - 1)) * size
	end
	f = math.mod(1 * (i - 1) / Options.Particles, 1)
	color = {rgb(255, 0, 0),rgb(50*f, 0, 0),rgb(0, 0, 0),rgb(255, 0, 0)}
	setParticle(i, life, 2, pos, color)
end
sizes = {.2}
setDefaultSize(sizes)