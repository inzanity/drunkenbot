Options = {}
Options.Particles = 25
Options.Coordinates = 40
Options.Colors = 1
Options.Sizes = 1
Options.Duration = 150
Options.Looping = true
Options.Texture = "../particles/16x16_default.bmp"
initParticleSystem(Options)
size = 1
life = 1000
pos = {}
for i = 1, Options.Particles do
	for j = 1, Options.Coordinates do
		pos[j] = {}
		pos[j].x = math.cos(2*3.1415*j/(Options.Coordinates-1)) * 0.01 * i
		pos[j].y = 0
		pos[j].z = math.sin(2*3.1415*j/(Options.Coordinates-1)) * 0.01 * i
	end
	f = math.mod(1* (i - 1) / Options.Particles, 1)
	color = {rgb(50*f, 50*f, 255*f)}
	setParticle(i, life, 0, pos, color)
end
sizes = {.05}
setDefaultSize(sizes)