Options = {}
Options.Particles = 150
Options.Coordinates = 20
Options.Colors = 1
Options.Sizes = 1
Options.Duration = 2000
Options.Looping = true
Options.Texture = "../particles/64x64_default.bmp"
initParticleSystem(Options)
size = 1
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
	color = {rgb(50*f, 50*f, 255*f)}
	setParticle(i, life, i/Options.Particles*life, pos, color)
end
sizes = {.2}
setDefaultSize(sizes)